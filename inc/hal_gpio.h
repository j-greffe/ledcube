#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include <msp430.h>
#include <stdint.h>

#define HAL_IO_PACK(_port,_pin) ((gpio_t){ .bf.port = ((_port)-1), .bf.pin = (_pin) })

#define HAL_IO_OUT              (BIT0)
#define HAL_IO_IN               (0)

#define HAL_IO_SET              (BIT1)
#define HAL_IO_CLR              (0)

#define HAL_IO_ALT              (BIT2)
#define HAL_IO_GPIO             (0)

#define HAL_IO_PU               (BIT3)
#define HAL_IO_PD               (BIT4)
#define HAL_IO_NOP              (0)

#define HAL_IO_NC               HAL_IO_OUT | HAL_IO_CLR

#define HAL_IO_NONE_BYTE        (0xFF)
#define HAL_IO_NONE             ((gpio_t){ .byte = HAL_IO_NONE_BYTE })

typedef union {
    struct {
        uint8_t pin     : 5;
        uint8_t port    : 3;
    } bf;
    uint8_t byte;
} gpio_t;

void hal_gpio_set(gpio_t io);
void hal_gpio_clr(gpio_t io);
void hal_gpio_tog(gpio_t io);
uint8_t hal_gpio_get(gpio_t io);
uint8_t hal_gpio_get_port(gpio_t io);
void hal_gpio_cfg(gpio_t io, uint8_t cfg);

#endif
