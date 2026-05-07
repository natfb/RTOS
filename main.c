/*
 RTOS - Turma 2026/1
 */

#include "./header_files/kernel.h"
#include "./header_files/user.h"

#define _XTAL_FREQ 4000.0

int main()
{
    os_config();
    
    os_create_task(1, sensor_data_processor, 4);
    os_create_task(2, read_sensor_data, 5);
   
    os_start();
    
    while (1) {
        
    }
    
    return 0;
}
