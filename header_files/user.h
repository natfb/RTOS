#ifndef USER_H
#define USER_H

#include "types.h"

extern uint8_t CURRENT_TEMPERATURE;

// void config_user(void);
// float ler_temperatura(uint16_t dados_adc);

// TASK read_sensor_data(void);
// TASK sensor_data_processor(void);

extern uint8_t CURRENT_TEMPERATURE;
extern uint8_t ALARM_ACTIVE;
extern uint8_t EVENT_COUNTER;

void config_user(void);

TASK task_sensor(void);
TASK task_controller(void);
TASK task_display(void);
TASK task_alarm(void);
TASK task_logger(void);

#endif /* USER_H */
