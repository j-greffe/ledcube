#ifndef _HAL_TIMER_A0_H_
#define _HAL_TIMER_A0_H_

#include <stdint.h>
#include "hal_timer.h"

void hal_timer_A0_open(void);
void hal_timer_A0_start(uint8_t idx, uint8_t mode, hal_isr_t action, void* param, uint16_t ti);
void hal_timer_A0_stop(uint8_t idx);

#endif // _HAL_TIMER_A0_H_
