/*
 RTOS - Turma 2026/1
 */

#include "./header_files/kernel.h"
#include "./header_files/user.h"

int main()
{
    os_config();
    
    //os_create_task(2, acionaMotor, 5);
    //os_create_task(3, ligaLed, 5);
    //os_create_task(4, apagaLed, 5);
    
    os_create_task(2, teste_mutex, 5);
    os_create_task(3, teste_mutex2, 5);
   
    os_start();
    
    while (1) {
        
    }
    
    return 0;
}
