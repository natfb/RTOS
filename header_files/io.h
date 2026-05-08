#ifndef IO_H
#define	IO_H

#include <xc.h>
#include <stdint.h>

void adc_config(void);
void adc_on(void);
uint16_t adc_read(void);

void pwm_config(void);
void pwm_set_duty(uint16_t duty);
void pwm_off(void);

#endif	/* IO_H */

