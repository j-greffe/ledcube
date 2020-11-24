#include <msp430.h>
#include <stdint.h>
#include "hal_gpio.h"
#include "hal_pwm.h"

void hal_pwm_A1_open(gpio_t io)
{
    if (HAL_IO_NONE_BYTE == io.byte) return;

	TA1CTL = (TASSEL_2 | ID_0); // 16 bits, SMCLK, DIV/1
	TA1CCTL1 = (OUTMOD_7); // Reset/Set for CCR1
	hal_gpio_cfg(io, HAL_IO_ALT | HAL_IO_OUT);
}

void hal_pwm_A1_cfg(uint16_t freq, uint16_t ratio)
{
    uint32_t f = 100000;
    f /= freq;
	TA1CCR0 = f;

	uint32_t r = ratio;
	r *= f;
	r /= PWM_RATIO_MAX;
	TA1CCR1 = r;
}

void hal_pwm_A1_start(void)
{
    hal_pwm_A1_stop();
	TA1CTL |= MC_1; // UP mode
}

void hal_pwm_A1_stop(void)
{
	TA1CTL &= ~(MC0 | MC1);
}
