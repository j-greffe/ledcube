#include <msp430.h>
#include <stdint.h>

#include "hal_gpio.h"

#define GPIO_PORT_NB    (sizeof(g_pdir)/sizeof(uint8_t*))

static volatile uint8_t* g_pdir[] = { &P1DIR, &P2DIR, &P3DIR };
static volatile uint8_t* g_pout[] = { &P1OUT, &P2OUT, &P3OUT };
static volatile uint8_t* g_psel[] = { &P1SEL, &P2SEL, &P3SEL };
static volatile const uint8_t* g_pin[] = { &P1IN, &P2IN, &P3IN };
static volatile uint8_t* g_pren[] = { &P1REN, &P2REN, &P3REN };

void hal_gpio_set(gpio_t* io)
{
	*(g_pout[io->bf.port]) |= (uint8_t)(1 << io->bf.pin);
}

void hal_gpio_clr(gpio_t* io)
{
	*(g_pout[io->bf.port]) &= (uint8_t)(~(1 << io->bf.pin));
}

void hal_gpio_tog(gpio_t* io)
{
	*(g_pout[io->bf.port]) ^= (uint8_t)(1 << io->bf.pin);
}

uint8_t hal_gpio_get(gpio_t* io)
{
    return !!(*(g_pin[io->bf.port]) & (1 << io->bf.pin));
}

uint8_t hal_gpio_get_port(gpio_t* io)
{
    return (*(g_pin[io->bf.port]));
}

void hal_gpio_cfg(gpio_t* io, uint8_t cfg)
{
	// clear registers
	*(g_psel[io->bf.port]) &= (uint8_t)(~(1 << io->bf.pin));
	*(g_pdir[io->bf.port]) &= (uint8_t)(~(1 << io->bf.pin));
	*(g_pout[io->bf.port]) &= (uint8_t)(~(1 << io->bf.pin));
	*(g_pren[io->bf.port]) &= (uint8_t)(~(1 << io->bf.pin));

	// configure IO
	if (cfg & IO_ALT)
	{
		*(g_psel[io->bf.port]) |= (uint8_t)(1 << io->bf.pin);
	}

	if (cfg & IO_OUT)
	{
		*(g_pdir[io->bf.port]) |= (uint8_t)(1 << io->bf.pin);
	}

	if (cfg & IO_SET)
	{
		hal_gpio_set(io);
	}

	if ((cfg & IO_PU) || (cfg & IO_PD))
	{
		*(g_pren[io->bf.port]) |= (uint8_t)(1 << io->bf.pin);
		if ((cfg & IO_PU))
		{
			hal_gpio_set(io);
		}
		else
		{
			hal_gpio_clr(io);
		}
	}

	// TODO: other options

}
