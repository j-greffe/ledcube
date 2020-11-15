#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#include <stdint.h>
#include "hal.h"

enum {
    HAL_TIMER_OFF,
    HAL_TIMER_ONE_SHOT,
    HAL_TIMER_CONTINOUS,
};

typedef struct
{
    uint16_t time;
    uint8_t mode;
    hal_isr_t action;
    void* param;
} hal_timer_t;

#include "hal_timer_A0.h"

#endif // _HAL_TIMER_H_
