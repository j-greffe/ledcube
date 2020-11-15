#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include <msp430.h>
#include <stdint.h>

#define IO_PACK(_port,_pin) {.bf.port = ((_port)-1), .bf.pin = (_pin)}

#define IO_OUT              (BIT0)
#define IO_IN               (0)

#define IO_SET              (BIT1)
#define IO_CLR              (0)

#define IO_ALT              (BIT2)
#define IO_GPIO             (0)

#define IO_PU               (BIT3)
#define IO_PD               (BIT4)
#define IO_NOP              (0)

#define IO_NC               IO_OUT | IO_CLR

typedef union {
    struct {
        uint8_t pin     : 5;
        uint8_t port    : 3;
    } bf;
    uint8_t byte;
} gpio_t;

void hal_gpio_set(gpio_t* io);
void hal_gpio_clr(gpio_t* io);
void hal_gpio_tog(gpio_t* io);
uint8_t hal_gpio_get(gpio_t* io);
uint8_t hal_gpio_get_port(gpio_t* io);
void hal_gpio_cfg(gpio_t* io, uint8_t cfg);

#endif
