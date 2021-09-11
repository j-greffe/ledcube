#include <stdlib.h>
#include <string.h>

#include "hal.h"
#include "kal.h"
#include "shift_register.h"
#include "ledcube.h"

static ledcube_hwcfg_t* g_hwcfg;
static volatile uint8_t** g_ledcube_pin;

//static const uint16_t k_intensity_lut[16] = {
//    0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960
//};

void ledcube_open(void* hwcfg)
{
    uint8_t i;

    g_hwcfg = (ledcube_hwcfg_t*)hwcfg;

    g_ledcube_pin = (volatile uint8_t**)malloc(g_hwcfg->size_z * sizeof(uint8_t*));

    for (i = 0; i < g_hwcfg->size_z; i++)
    {
        g_ledcube_pin[i] = (volatile uint8_t*)malloc(g_hwcfg->shift_register_nb * sizeof(uint8_t));
    }

    shift_register_open(g_hwcfg->shift_register_hwcfg);
}

void ledcube_update_layer(uint8_t z)
{
    uint8_t i;

    z = min_u8(g_hwcfg->size_z - 1, z);

    // Select z pin
    g_ledcube_pin[z][g_hwcfg->pin_z[z].byte] |= g_hwcfg->pin_z[z].bit;

    for (i = 0; i < g_hwcfg->shift_register_nb; i++)
    {
        shift_register_push_byte(g_ledcube_pin[z][i] ^ 0xFF);
    }

    shift_register_update();
}

void ledcube_set(void)
{
    uint8_t i;

    for (i = 0; i < g_hwcfg->size_z; i++)
    {
        memset((void*)g_ledcube_pin[i], 0xFF, g_hwcfg->shift_register_nb);
    }
}

void ledcube_clr(void)
{
    uint8_t i;

    for (i = 0; i < g_hwcfg->size_z; i++)
    {
        memset((void*)g_ledcube_pin[i], 0x00, g_hwcfg->shift_register_nb);
    }
}

void ledcube_led_set(uint8_t x, uint8_t y, uint8_t z)
{
    x = min_u8(g_hwcfg->size_x - 1, x);
    y = min_u8(g_hwcfg->size_y - 1, y);
    z = min_u8(g_hwcfg->size_z - 1, z);

    g_ledcube_pin[z][g_hwcfg->pin_xy[x][y].byte] |= g_hwcfg->pin_xy[x][y].bit;
}

void ledcube_led_clr(uint8_t x, uint8_t y, uint8_t z)
{
    x = min_u8(g_hwcfg->size_x - 1, x);
    y = min_u8(g_hwcfg->size_y - 1, y);
    z = min_u8(g_hwcfg->size_z - 1, z);

    g_ledcube_pin[z][g_hwcfg->pin_xy[x][y].byte] &=~ g_hwcfg->pin_xy[x][y].bit;
}

// Draw a line in specified AXIS
void ledcube_line_x_set(uint8_t x1, uint8_t x2, uint8_t y, uint8_t z)
{
    uint8_t i;

    x2 = max_u8(x1, x2);

    for (i = x1; i <= x2; i++)
    {
        ledcube_led_set(i, y, z);
    }
}

void ledcube_line_y_set(uint8_t y1, uint8_t y2, uint8_t x, uint8_t z)
{
    uint8_t i;

    y2 = max_u8(y1, y2);

    for (i = y1; i <= y2; i++)
    {
        ledcube_led_set(x, i, z);
    }
}

void ledcube_line_z_set(uint8_t z1, uint8_t z2, uint8_t x, uint8_t y)
{
    uint8_t i;

    z2 = max_u8(z1, z2);

    for (i = z1; i <= z2; i++)
    {
        ledcube_led_set(x, y, i);
    }
}

// Draw a rectangle in specified PLANE
void ledcube_rectangle_x_set(uint8_t y1, uint8_t z1, uint8_t y2, uint8_t z2, uint8_t x)
{
    y2 = max_u8(y1, y2);
    z2 = max_u8(z1, z2);

    ledcube_line_y_set(y1, y2, x, z1);
    ledcube_line_y_set(y1, y2, x, z2);
    ledcube_line_z_set(z1, z2, x, y1);
    ledcube_line_z_set(z1, z2, x, y2);
}

void ledcube_rectangle_y_set(uint8_t x1, uint8_t z1, uint8_t x2, uint8_t z2, uint8_t y)
{
    x2 = max_u8(x1, x2);
    z2 = max_u8(z1, z2);

    ledcube_line_x_set(x1, x2, y, z1);
    ledcube_line_x_set(x1, x2, y, z2);
    ledcube_line_z_set(z1, z2, x1, y);
    ledcube_line_z_set(z1, z2, x2, y);
}

void ledcube_rectangle_z_set(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t z)
{
    x2 = max_u8(x1, x2);
    y2 = max_u8(y1, y2);

    ledcube_line_x_set(x1, x2, y1, z);
    ledcube_line_x_set(x1, x2, y2, z);
    ledcube_line_y_set(y1, y2, x1, z);
    ledcube_line_y_set(y1, y2, x2, z);
}

void ledcube_wireframe_set(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2)
{
    x2 = max_u8(x1, x2);
    y2 = max_u8(y1, y2);
    z2 = max_u8(z1, z2);

    ledcube_rectangle_z_set(x1, y1, x2, y2, z1);
    ledcube_rectangle_z_set(x1, y1, x2, y2, z2);
    ledcube_line_z_set(z1, z2, x1, y1);
    ledcube_line_z_set(z1, z2, x1, y2);
    ledcube_line_z_set(z1, z2, x2, y1);
    ledcube_line_z_set(z1, z2, x2, y2);
}





