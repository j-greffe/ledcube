#ifndef _HAL_PWM_A1_H_
#define _HAL_PWM_A1_H_

#include <stdint.h>
#include "hal_pwm.h"
#include "hal_gpio.h"

int hal_pwm_A1_open(gpio_t io);
int hal_pwm_A1_cfg(uint16_t freq, uint16_t ratio);
int hal_pwm_A1_start(void);
int hal_pwm_A1_stop(void);

#endif // _HAL_PWM_A1_H_
