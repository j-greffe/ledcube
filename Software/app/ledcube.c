#include <stdlib.h>
#include <string.h>

#include "hal.h"
#include "kal.h"
#include "hwcfg.h"
#include "shift_register.h"
#include "ledcube.h"

#define SIZE_X   LEDCUBE_SIZE_X
#define SIZE_Y   LEDCUBE_SIZE_Y
#define SIZE_Z   LEDCUBE_SIZE_Z

static ledcube_hwcfg_t* g_hwcfg;
static uint8_t g_ledcube_layer[SIZE_Z][LEDCUBE_SHIFT_REGISTER_NB];


void ledcube_open(void* hwcfg)
{
    g_hwcfg = (ledcube_hwcfg_t*)hwcfg;

    shift_register_open(g_hwcfg->shift_register_hwcfg);

    ledcube_clr();
}

void ledcube_update_layer(uint8_t z)
{
    uint8_t i;

    z = min_u8(SIZE_Z - 1, z);

    // Select z pin
    g_ledcube_layer[z][g_hwcfg->pin_z[z].byte] |= g_hwcfg->pin_z[z].bit;

    for (i = 0; i < LEDCUBE_SHIFT_REGISTER_NB; i++)
    {
        shift_register_push_byte(g_ledcube_layer[z][i]);
    }

    shift_register_update();

    // Clear z pin
    g_ledcube_layer[z][g_hwcfg->pin_z[z].byte] &=~ g_hwcfg->pin_z[z].bit;
}

void ledcube_set(void)
{
    uint8_t i;
    uint8_t j;

    // Set all pins
    memset((void*)g_ledcube_layer, 0xFF, sizeof(g_ledcube_layer));

    // Clear z pins
    for (i = 0; i < SIZE_Z; i++)
    {
        for (j = 0; j < SIZE_Z; j++)
        {
            g_ledcube_layer[i][g_hwcfg->pin_z[j].byte] &=~ g_hwcfg->pin_z[j].bit;
        }
    }
}

void ledcube_clr(void)
{
    // Clear all pins
    memset((void*)g_ledcube_layer, 0x00, sizeof(g_ledcube_layer));
}

void ledcube_led_set(uint8_t x, uint8_t y, uint8_t z)
{
    x = min_u8(SIZE_X - 1, x);
    y = min_u8(SIZE_Y - 1, y);
    z = min_u8(SIZE_Z - 1, z);

    g_ledcube_layer[z][g_hwcfg->pin_xy[x][y].byte] |= g_hwcfg->pin_xy[x][y].bit;
}

void ledcube_led_clr(uint8_t x, uint8_t y, uint8_t z)
{
    x = min_u8(SIZE_X - 1, x);
    y = min_u8(SIZE_Y - 1, y);
    z = min_u8(SIZE_Z - 1, z);

    g_ledcube_layer[z][g_hwcfg->pin_xy[x][y].byte] &= ~(g_hwcfg->pin_xy[x][y].bit);
}

bool ledcube_led_get(uint8_t x, uint8_t y, uint8_t z)
{
    x = min_u8(SIZE_X - 1, x);
    y = min_u8(SIZE_Y - 1, y);
    z = min_u8(SIZE_Z - 1, z);

    return !!(g_ledcube_layer[z][g_hwcfg->pin_xy[x][y].byte] & g_hwcfg->pin_xy[x][y].bit);
}

bool ledcube_layer_full(uint8_t z)
{
    uint8_t i;
    uint8_t j;

    for (i = 0; i < SIZE_X; i++)
    {
        for (j = 0; j < SIZE_Y; j++)
        {
            if (!ledcube_led_get(i, j, z))
            {
                return false;
            }
        }
    }

    return true;
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

void ledcube_plane_x_set(uint8_t x)
{
    uint8_t i;

    for (i = 0; i < SIZE_Y; i++)
    {
        ledcube_line_z_set(0, SIZE_Z - 1, x, i);
    }
}

void ledcube_plane_y_set(uint8_t y)
{
    uint8_t i;

    for (i = 0; i < SIZE_X; i++)
    {
        ledcube_line_z_set(0, SIZE_Z - 1, i, y);
    }
}

void ledcube_plane_z_set(uint8_t z)
{
    uint8_t i;

    for (i = 0; i < SIZE_X; i++)
    {
        ledcube_line_y_set(0, SIZE_Y - 1, i, z);
    }
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

void ledcube_shift_layers_down(void)
{
    uint8_t i;

    // Shift layers down by 1
    for (i = 0; i < (SIZE_Z-1); i++)
    {
        memcpy((void*)g_ledcube_layer[i], (void*)g_ledcube_layer[i+1], LEDCUBE_SHIFT_REGISTER_NB);
//        g_ledcube_layer[i][0] = g_ledcube_layer[i+1][0];
//        g_ledcube_layer[i][1] = g_ledcube_layer[i+1][1];
//        g_ledcube_layer[i][2] = g_ledcube_layer[i+1][2];
    }

//    g_ledcube_layer[SIZE_Z-1][0] = 0x00;
//    g_ledcube_layer[SIZE_Z-1][1] = 0x00;
//    g_ledcube_layer[SIZE_Z-1][2] = 0x00;

    // Clear top layer
    memset((void*)g_ledcube_layer[SIZE_Z-1], 0x00, LEDCUBE_SHIFT_REGISTER_NB);

}





