#include <msp430.h>
#include "hal_clk_p.h"
#include "hal_timer.h"

#ifdef __MSP430_HAS_TA3__
static volatile uint16_t* g_tacctl[] = { &TA0CCTL0, &TA0CCTL1, &TA0CCTL2 };
static volatile uint16_t* g_taccr[] = { &TA0CCR0, &TA0CCR1, &TA0CCR2 };
#else
#error "Please define your timer type."
#endif

#define TIMER_NB (sizeof(g_tacctl)/sizeof(uint16_t*))
static hal_timer_t g_timer[TIMER_NB];

void hal_timer_A0_open(void)
{
    // ACKL / 8
    hal_clk_set(HAL_CLK_ACLK, HAL_CLK_BASE_ACLK/8);

    // Wait for fault bit to clear
    while (BCSCTL3 & LFXT1OF);

    // 16 bits, ACLK (32768Hz external oscillator), DIV/4, Continuous mode.
    // --> (2^16)/((32768/8)/4) = 64s before wrap
    // XXX To stay simple, this API does not handle wraps, limiting the timer time at 64s.
    // Timer unit is in Tick (1/1024th second)
	TA0CTL = (TASSEL_1 | ID_2 | MC_2);

	__enable_interrupt();
}

void hal_timer_A0_start(uint8_t idx, uint8_t mode, hal_isr_t action, void* param, uint16_t ti)
{
    // Configure timer
    g_timer[idx].time = ti;
    g_timer[idx].mode = mode;
	g_timer[idx].action = action;
	g_timer[idx].param = param;

	// Configure interrupt
	*(g_taccr[idx]) = (uint16_t)(ti+(uint16_t)TAR);
	*(g_tacctl[idx]) = (CCIE);
}

void hal_timer_A0_stop(uint8_t idx)
{
	*(g_tacctl[idx]) = 0;
	g_timer[idx].mode = HAL_TIMER_OFF;
}

// Timer A0 interrupt service routine
// ISR for CCR0
// high priority
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    uint8_t idx = 0;

    hal_lpm_exit();

    if (HAL_TIMER_CONTINOUS != g_timer[idx].mode)
    {
        // Disable interrupt on timer
        *(g_tacctl[idx]) = 0;
    }
    else
    {
        // Update CCR for next interrupt
        *(g_taccr[idx]) += g_timer[idx].time;
    }

	// Execute action
	if (g_timer[idx].action)
	{
	    g_timer[idx].action(g_timer[idx].param);
	}
}

// Timer A0 interrupt service routine
// ISR for CCR1+
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1(void)
{
    // Get idx, this clears all interrupt flags
    uint8_t idx = (uint8_t)(TA0IV >> 1);

    hal_lpm_exit();

    if (HAL_TIMER_CONTINOUS != g_timer[idx].mode)
    {
        // Disable interrupt on timer
        *(g_tacctl[idx]) = 0;
    }
    else
    {
        // Update CCR for next interrupt
        *(g_taccr[idx]) += g_timer[idx].time;
    }

    // Execute action
    if (g_timer[idx].action)
    {
        g_timer[idx].action(g_timer[idx].param);
    }
}
