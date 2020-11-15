#include <msp430.h>

#include "hal.h"
#include "hal_lpm.h"

#if HAL_LPM_ENABLE

volatile uint16_t g_hal_lpm_bits;

void hal_lpm_enter(uint8_t lpm)
{
    switch (lpm)
    {
    case LPM_0:
        g_hal_lpm_bits = LPM0_bits;
        break;
    case LPM_1:
        g_hal_lpm_bits = LPM1_bits;
        break;
    case LPM_2:
        g_hal_lpm_bits = LPM2_bits;
        break;
    case LPM_3:
        g_hal_lpm_bits = LPM3_bits;
        break;
    case LPM_4:
        g_hal_lpm_bits = LPM4_bits;
        break;
    default:
        g_hal_lpm_bits = 0;
        break;
    }

    // LPM with interrupts enabled
    if (g_hal_lpm_bits)
    {
        __bis_SR_register(g_hal_lpm_bits | GIE);
    }
}

#endif // HAL_LPM_ENABLE
