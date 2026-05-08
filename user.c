#include "./header_files/user.h"
#include <pic18f46k22.h>
#include <xc.h>

#include "./header_files/sync.h"
#include "./header_files/com.h"
#include "./header_files/io.h"
#include "./header_files/io_ei.h"
#include "./header_files/kernel.h"

pipe_t temp_pipe;
sem_t alarm_sem;
mutex_t state_mutex;

uint8_t CURRENT_TEMPERATURE = 0;
uint8_t ALARM_ACTIVE = 0;
uint8_t EVENT_COUNTER = 0;

void config_user(void)
{
    TRISDbits.RD7 = 0;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 0;

    ANSELDbits.ANSD7 = 0;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;

    PORTDbits.RD7 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;

    asm("global _task_sensor, _task_controller, _task_display, _task_alarm, _task_logger");

    adc_config();
    adc_on();

    pwm_config();
    pwm_off();

    pipe_init(&temp_pipe);

    sem_init(&alarm_sem, 0);
    mutex_init(&state_mutex);

    config_interruption();
}

TASK task_sensor(void)
{
    uint8_t sensor_data = 50;

    while (1)
    {
        // Teste inicial:
        // 50 -> abaixo de 30 °C
        // 75 -> acima de 30 °C
        pipe_write(&temp_pipe, (char)sensor_data);

        if (sensor_data == 50)
            sensor_data = 75;
        else
            sensor_data = 50;

        os_delay(50);

        // Depois trocar por:
        // sensor_data = (uint8_t)adc_read();
    }
}

TASK task_controller(void)
{
    uint8_t sensor_data = 0;
    uint8_t temp = 0;
    uint8_t alarm_now = 0;
    uint8_t last_alarm = 0;

    while (1)
    {
        pipe_read(&temp_pipe, (char *)&sensor_data);

        temp = (uint8_t)((sensor_data * 5.0 / 1023.0) * 100.0);

        if (temp > 30)
            alarm_now = 1;
        else
            alarm_now = 0;

        mutex_lock(&state_mutex);
        CURRENT_TEMPERATURE = temp;
        ALARM_ACTIVE = alarm_now;
        mutex_unlock(&state_mutex);

        if (alarm_now && !last_alarm)
            sem_post(&alarm_sem);

        last_alarm = alarm_now;

        os_delay(10);
    }
}

TASK task_display(void)
{
    uint8_t alarm_copy;

    while (1)
    {
        mutex_lock(&state_mutex);
        alarm_copy = ALARM_ACTIVE;
        mutex_unlock(&state_mutex);

        if (alarm_copy)
        {
            PORTDbits.RD7 = 1;
            PORTCbits.RC6 = 0;
        }
        else
        {
            PORTDbits.RD7 = 0;
            PORTCbits.RC6 = 1;
        }

        os_delay(20);
    }
}

TASK task_alarm(void)
{
    uint8_t alarm_copy;
    uint8_t i;

    while (1)
    {
        sem_wait(&alarm_sem);

        for (i = 0; i < 30; i++)
        {
            mutex_lock(&state_mutex);
            alarm_copy = ALARM_ACTIVE;
            mutex_unlock(&state_mutex);

            if (!alarm_copy)
                break;

            pwm_set_duty(512);
            os_delay(2);

            pwm_set_duty(0);
            os_delay(2);
        }

        pwm_off();
    }
}

TASK task_logger(void)
{
    uint8_t i;

    mutex_lock(&state_mutex);
    EVENT_COUNTER++;
    mutex_unlock(&state_mutex);

    for (i = 0; i < 3; i++)
    {
        PORTCbits.RC7 = 1;
        os_delay(10);

        PORTCbits.RC7 = 0;
        os_delay(10);
    }

    os_task_exit();
}