#include <msp430.h>
#include <stdlib.h>
#include <stdint.h>

#include "hwcfg.h"
#include "hal.h"
#include "kal.h"
#include "main.h"
#include "ledcube.h"
#include "ledcube_anim.h"


#define SIZE_X   k_ledcube_hwcfg.size_x
#define SIZE_Y   k_ledcube_hwcfg.size_y
#define SIZE_Z   k_ledcube_hwcfg.size_z

#define REFRESH_TICK    2
#define ANIMATION_TICK  32

enum {
    ANIM_FALLIN_RAND,
    ANIM_FADE,
    ANIM_APPEAR,
    ANIM_RAIN,
    ANIM_FALLIN_LAYER,
    ANIM_PLANES,
    ANIM_SPIRAL,
    ANIM_HELI,

    ANIM_QTY,
};

static volatile kal_timer_t g_tim_button_debounce;
static volatile kal_timer_t g_tim_refresh;
static volatile kal_timer_t g_tim_animation_tick;
static volatile uint32_t g_anim_mult = 1;

static anim_buffer_t anim_buffer = {
    .step = 0,
    .ticks = 4,
};

void button_enable(__unused void* param)
{
    hal_irq_en(IO_BUTTON);
}

void button_debounce(__unused void* param)
{
    hal_irq_dis(IO_BUTTON);
    kal_event_set(EVENT_BUTTON_PUSH);
    kal_timer_start((kal_timer_t*)&g_tim_button_debounce, button_enable, NULL, 2000000);
}

void refresh_callback(__unused void* param)
{
    kal_event_set(EVENT_REFRESH);
    kal_timer_start((kal_timer_t*)&g_tim_refresh, (hal_isr_t)refresh_callback, (void*)NULL, REFRESH_TICK);
}

void animation_callback(__unused void* param)
{
    kal_event_set(EVENT_ANIMATION_TICK);
    kal_timer_start((kal_timer_t*)&g_tim_animation_tick, (hal_isr_t)animation_callback, (void*)NULL, ANIMATION_TICK * anim_buffer.ticks);
}

int main() {
    uint8_t event;

    uint8_t layer = 0;
    uint32_t anim = 0;

    // Stop watchdog
    hal_wdg_stop();

    hal_open(&k_hal_hwcfg);

    // Set unused pins
    hal_gpio_cfg(IO_RX, HAL_IO_NC);
    hal_gpio_cfg(IO_TX, HAL_IO_NC);

    // Open KAL
    kal_dbg_open(IO_ENABLE);
//    kal_event_open(EVENT_QUANTITY);
    kal_timer_open();

    // Configure button
    hal_gpio_cfg(IO_BUTTON, HAL_IO_IN | HAL_IO_PU);
    hal_irq_set(IO_BUTTON, HAL_IRQ_FALLING, button_debounce, NULL);
    hal_irq_en(IO_BUTTON);

    hal_gpio_cfg(IO_ENABLE, HAL_IO_OUT);
    hal_gpio_clr(IO_ENABLE);

    anim_buffer.step = 0;
    anim_buffer.ticks = 4;
    ledcube_open((void*)&k_ledcube_hwcfg);
//    ledcube_led_set(0, 0, SIZE_Z-1);
//    ledcube_set();
//    ledcube_wireframe_set(0, 0, 0, SIZE_X-1, SIZE_Y-1, SIZE_Z-1);

    refresh_callback(0);
    animation_callback(0);

    while(1) {

        event = kal_event_wait(EVENT_ALL);

        if (EVENT_BUTTON_PUSH == event)
        {
            hal_gpio_tog(IO_ENABLE);
        }
        else if (EVENT_REFRESH == event)
        {
            if (layer >= SIZE_Z)
            {
                layer = 0;
            }

            ledcube_update_layer(layer++);
        }
        else if (EVENT_ANIMATION_TICK == event)
        {
            switch (anim)
            {
            case ANIM_PLANES:
                anim += ledcube_anim_planes(&anim_buffer);
                break;
            case ANIM_RAIN:
                anim += ledcube_anim_rain(&anim_buffer);
                break;
            case ANIM_SPIRAL:
                anim += ledcube_anim_spiral(&anim_buffer);
                break;
            case ANIM_HELI:
                anim += ledcube_anim_heli(&anim_buffer);
                break;
            case ANIM_APPEAR:
                anim += ledcube_anim_appear(&anim_buffer);
                break;
            case ANIM_FADE:
                anim += ledcube_anim_fade(&anim_buffer);
                break;
            case ANIM_FALLIN_RAND:
                anim += ledcube_anim_fallin_rand(&anim_buffer);
                break;
            case ANIM_FALLIN_LAYER:
                anim += ledcube_anim_fallin_layer(&anim_buffer);
                break;
            case ANIM_QTY:
            default:
                anim_buffer.step = 0;
                anim = 0;

                break;
            }
        }
    }
}
