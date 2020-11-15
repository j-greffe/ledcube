#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_

#include <stdint.h>
#include "hal.h"

void hal_adc_start(uint16_t channel, hal_isr_t action);

#endif // _HAL_ADC_H_
