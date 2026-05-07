#ifndef USER_H
#define	USER_H

#include "types.h"

extern uint8_t CURRENT_TEMPERATURE;

void config_user(void);
float ler_temperatura(uint16_t dados_adc);

TASK read_sensor_data(void);
TASK sensor_data_processor(void);

#endif	/* USER_H */

