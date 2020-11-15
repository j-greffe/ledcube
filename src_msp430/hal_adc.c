#include <msp430.h>
#include <stdint.h>
#include "hal_adc.h"

static hal_isr_t g_action;

void hal_adc_start(uint16_t channel, hal_isr_t action)
{
    g_action = action;

    // ADC Channel, CLK/4
    ADC10CTL1 = channel + ADC10DIV_3;
    // Ref -> AVCC, 64 CLK S&H, Interrupt Enable
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
    // Sampling and conversion start
    ADC10CTL0 |= ENC + ADC10SC;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    uint16_t adcValue = ADC10MEM;                 // Fetch ADC conversion result

    // Call user callback
    if (g_action)
    {
        g_action((void*)adcValue);
    }
}
