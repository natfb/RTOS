#include "./header_files/user.h"
#include <pic18f46k22.h>
#include <xc.h>
#include "./header_files/sync.h"
#include "./header_files/com.h"
#include "./header_files/io.h"
#include "./header_files/kernel.h"
pipe_t p;
sem_t s;

uint8_t CURRENT_TEMPERATURE;

void config_user()
{
    TRISCbits.RC6       = 0;
    TRISCbits.RC7       = 0;
    TRISDbits.RD0       = 0;
    ANSELDbits.ANSD0    = 0;
    ANSELCbits.ANSC6    = 0;
    ANSELCbits.ANSC7    = 0;
    TRISDbits.RD7       = 0;
    
    asm("global _read_sensor_data, _sensor_data_processor");

    adc_config();
    adc_on();

    pipe_init(&p);
    sem_init(&s, 2);
}

TASK read_sensor_data() {
    // codigo og
    uint16_t sensor_data = 0;

    while(1) {
        // sensor_data = adc_read();
        // 61V = 30ºC, ou seja >61 acende led <61 apaga
        sensor_data = 60;
        pipe_write(&p, (char)sensor_data);
    }
}

TASK sensor_data_processor() {
    uint16_t sensor_data = 0;
    while(1) {
        pipe_read(&p, (char *)&sensor_data);

        CURRENT_TEMPERATURE = (sensor_data * 5.0 / 1023.0) * 100.0;

        if (CURRENT_TEMPERATURE > 30) {
            sem_post(&s);
            PORTDbits.RD7 = 1;
        }
        os_delay(100);
    }
}