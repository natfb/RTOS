#include "./header_files/user.h"
#include <pic18f46k22.h>
#include <xc.h>

#include "./header_files/sync.h"
#include "./header_files/com.h"
#include "./header_files/io.h"
#include "./header_files/io_ei.h"
#include "./header_files/kernel.h"
#include "./header_files/io_ei.h"
pipe_t p;
sem_t s;

uint8_t CURRENT_TEMPERATURE;
mutex_t temp_mutex;

#define LED_OK      PORTCbits.RC6
#define LED_LOG     PORTCbits.RC7
#define LED_ALARM   PORTDbits.RD7

#define TEMP_LIMITE_ALERTA 30

void config_user(void)
{
    TRISCbits.RC6       = 0;
    TRISCbits.RC7       = 0;
    TRISDbits.RD0       = 0;
    ANSELDbits.ANSD0    = 0;
    ANSELCbits.ANSC6    = 0;
    ANSELCbits.ANSC7    = 0;
    TRISDbits.RD7       = 0;
    ANSELDbits.ANSD7 = 0;
    
    asm("global _read_sensor_data, _sensor_data_processor, _Task_Dashboard");

    adc_config();
    adc_on();

    pwm_config();
    pwm_off();

    pipe_init(&p);

    // Semáforo do alarme começa em 0 para Task_Alert ficar bloqueada
    sem_init(&s, 2);

    mutex_init(&temp_mutex);

    // config_interruption();

    LED_OK = 0;
    LED_LOG = 0;
    LED_ALARM = 0;
}

TASK read_sensor_data() {
    // codigo og
    uint16_t sensor_data = 0;

    while(1) {
        // sensor_data = adc_read();
        // 61V = 30ºC, ou seja >61 acende led <61 apaga
        sensor_data = 75;
        pipe_write(&p, (char)sensor_data);
    }
    // while(1) {
    //     sensor_data = 50;      // LED deve apagar
    //     pipe_write(&p, (char)sensor_data);
    //     os_delay(50);

    //     sensor_data = 75;      // LED deve acender
    //     pipe_write(&p, (char)sensor_data);
    //     os_delay(50);
    // }
}

TASK sensor_data_processor() {
    uint16_t sensor_data = 0;
    while(1) {
        pipe_read(&p, (char *)&sensor_data);

        CURRENT_TEMPERATURE = (uint8_t)((sensor_data * 5.0 / 1023.0) * 100.0);

        if (CURRENT_TEMPERATURE > 30) {
            PORTDbits.RD7 = 1;
        } else {
            PORTDbits.RD7 = 0;
        }
        os_delay(100);
    }
}
TASK Task_Dashboard(void)
{
    uint8_t temp_snapshot;

    while (1)
    {
        mutex_lock(&temp_mutex);
        temp_snapshot = CURRENT_TEMPERATURE;
        mutex_unlock(&temp_mutex);

        if (temp_snapshot > TEMP_LIMITE_ALERTA)
        {
            LED_OK = 0;
            LED_ALARM = 1;
        }
        else
        {
            LED_OK = 1;
            LED_ALARM = 0;
        }

        os_delay(50);
    }
}

// TASK Task_Alert(void)
// {
//     uint16_t duty = 0;
//     int8_t step = 80;

//     while (1)
//     {
//         sem_wait(&s);

//         // Alarme PWM com variação de intensidade
//         for (uint8_t i = 0; i < 20; i++)
//         {
//             pwm_set_duty(duty);

//             if (duty >= 1000)
//                 step = -80;
//             else if (duty <= 80)
//                 step = 80;

//             duty += step;

//             os_delay(5);
//         }

//         pwm_off();
//     }
// }

// TASK Task_Emergency(void)
// {
//     // Amarelo indica evento one-shot/emergência
//     LED_LOG = 1;

//     // Desliga atuadores/status de alarme
//     pwm_off();
//     LED_ALARM = 0;
//     LED_OK = 0;

//     // Limpa flags de erro/alarme da aplicação
//     CURRENT_TEMPERATURE = 0;

//     // Limpa flag da interrupção externa
//     INTCONbits.INT0F = 0;

//     os_delay(20);

//     LED_LOG = 0;

//     // Auto-extermina
//     os_task_terminate();
// }