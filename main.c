/*
 RTOS - Turma 2026/1
 */

#include "./header_files/kernel.h"
#include "./header_files/user.h"

#define _XTAL_FREQ 4000.0

int main()
{
    os_config();

    os_create_task(1, task_sensor, 2);
    os_create_task(3, task_controller, 2);
    os_create_task(4, task_display, 1);
    os_create_task(5, task_alarm, 3);

    os_start();

    while (1)
    {
    }

    return 0;
}
