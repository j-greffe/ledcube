#include <msp430.h>
#include <stdlib.h>
#include <stdint.h>

#include "hwcfg.h"
#include "hal.h"
#include "kal.h"
#include "main.h"
#include "ledcube.h"

#define CUBE_SIDE   4
#define CUBE_HEIGHT 8

kal_timer_t g_tim_button_debounce;

void button_enable(__unused void* param)
{
    hal_irq_en(IO_BUTTON);
}

void button_debounce(__unused void* param)
{
    hal_irq_dis(IO_BUTTON);
    kal_event_set(EVENT_BUTTON_PUSH);
    kal_timer_start(&g_tim_button_debounce, button_enable, NULL, 2000000);
}

int main() {
    uint8_t event;

    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t z = 0;
    uint8_t l = 0;
    uint8_t w = 0;
    uint8_t h = 0;

    kal_timer_t tim_layer_update;
    kal_timer_t tim_animation_tick;

    // Stop watchdog
    hal_wdg_stop();

    hal_open(&k_hal_hwcfg);

    // Set unused pins
    hal_gpio_cfg(IO_RX, HAL_IO_NC);
    hal_gpio_cfg(IO_TX, HAL_IO_NC);

    // Open KAL
    kal_dbg_open(IO_ENABLE);
    kal_event_open(EVENT_QUANTITY);

    // Configure button
    hal_gpio_cfg(IO_BUTTON, HAL_IO_IN | HAL_IO_PU);
    hal_irq_set(IO_BUTTON, HAL_IRQ_FALLING, button_debounce, NULL);
    hal_irq_en(IO_BUTTON);

    hal_gpio_cfg(IO_ENABLE, HAL_IO_OUT);
    hal_gpio_clr(IO_ENABLE);

    ledcube_open((void*)&k_ledcube_hwcfg);

    __enable_interrupt();

    kal_timer_open();

//    hal_pwm_open(HAL_PWM_1, IO_ENABLE);
//    hal_pwm_cfg(HAL_PWM_1, 1024, intensity);
//    hal_pwm_start(HAL_PWM_1);

    kal_event_set(EVENT_UPDATE_LAYER);
    kal_event_set(EVENT_ANIMATION_TICK);

    while(1) {

        event = kal_event_wait(EVENT_ALL);

        if (EVENT_BUTTON_PUSH == event)
        {
            hal_gpio_tog(IO_ENABLE);
        }
        else if (EVENT_UPDATE_LAYER == event)
        {
//            ledcube_update_layer();

//            kal_timer_start(&tim_layer_update, (hal_isr_t)kal_event_set, (void*)EVENT_UPDATE_LAYER, 25000);
        }
        else if (EVENT_ANIMATION_TICK == event)
        {
            static uint32_t tick_nb;
            uint32_t nb;

            ledcube_clr();
//            ledcube_led_set(x, y, 0);
//            ledcube_line_y_set(y, x, 0, l+1);
//            ledcube_rectangle_z_set(0, 0, x, y, 0);
            ledcube_rectangle_z_set(x, y, k_ledcube_hwcfg.size_x-1, k_ledcube_hwcfg.size_y-1, 0);
            ledcube_update_layer(0);

            x++;

            if (x >= k_ledcube_hwcfg.size_x)
            {
                y++;
                x = 0;
            }

            if (y >= k_ledcube_hwcfg.size_y)
            {
                z++;
                y = 0;
            }


//            if (tick_nb < 4)
//            {
//                nb = tick_nb % 4;
//
//                led_cube_clr();
//                ledcube_wireframe_set(0, 0, 0, nb+1, nb+1, (nb+1)*2);
//            }
//            else if (tick_nb < 8)
//            {
//                nb = tick_nb % 4;
//
//                led_cube_clr();
//                ledcube_wireframe_set(k_ledcube_hwcfg.size_x-(nb+1), 0, 0, nb+1, nb+1, (nb+1)*2);
//            }
//            else if (tick_nb < 12)
//            {
//                nb = tick_nb % 4;
//
//                led_cube_clr();
//                ledcube_wireframe_set(0, nb, 0, nb+1, nb+1, (nb+1)*2);
//            }
//            else if (tick_nb < 16)
//            {
//                nb = tick_nb % 4;
//
//                led_cube_clr();
//                ledcube_wireframe_set(nb, nb, 0, nb+1, nb+1, (nb+1)*2);
//            }
//            else if (tick_nb < 20)
//            {
//                nb = tick_nb % 4;
//
//                led_cube_clr();
//                ledcube_wireframe_set(0, 0, (nb+1)*2, nb+1, nb+1, (nb+1)*2);
//            }



            kal_timer_start(&tim_animation_tick, (hal_isr_t)kal_event_set, (void*)EVENT_ANIMATION_TICK, 1000000);
        }
    }
}
