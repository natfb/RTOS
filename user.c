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
    
    asm("global _read_sensor_data, _sensor_data_processor");

    adc_config();
    adc_on();

    pipe_init(&p);
    sem_init(&s, 2);
}

TASK read_sensor_data() {
    // codigo og
    // uint16_t sensor_data = 0;

    // while(1) {
    //     sensor_data = adc_read();
    //     pipe_write(&p, (char)sensor_data);
    // }

    // teste sem pipe
    uint16_t sensor_data = 0;
    float test_temp = 0;

    while(1) {
        // zsensor_data = adc_read(); 

        test_temp = 45;//(sensor_data * 5.0 / 1023.0) * 100.0;

        if (test_temp > 30.0) {
            PORTDbits.RD7 = 1;  
        } else {
            PORTDbits.RD7 = 0;  
        }

        os_delay(200); 
    }
}

TASK sensor_data_processor() {
    // uint16_t sensor_data = 0;
    while(1) {
        // pipe_read(&p, (char *)&sensor_data);

        // CURRENT_TEMPERATURE = (sensor_data * 5.0 / 1023.0) * 100.0;

        // if (CURRENT_TEMPERATURE > 30) {
        //     sem_post(&s);
        //     PORTDbits.RD7 = 1;
        // }
        os_delay(100);
    }

}