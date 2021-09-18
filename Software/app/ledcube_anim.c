#include <stdlib.h>

#include "hwcfg.h"
#include "ledcube.h"

#define SIZE_X   LEDCUBE_SIZE_X
#define SIZE_Y   LEDCUBE_SIZE_Y
#define SIZE_Z   LEDCUBE_SIZE_Z

static bool ledcube_anim_rand_led_from_layer(uint8_t* x, uint8_t* y, uint8_t z, bool set)
{
    uint32_t led_nb = SIZE_X * SIZE_Y;
    (*x) = rand()%SIZE_X;
    (*y) = rand()%SIZE_Y;

    do
    {
        if (ledcube_led_get(*x, *y, z) != set)
        {
            // Search for next
            (*x)++;
            if ((*x) >= SIZE_X)
            {
                (*x) = 0;
                (*y)++;
            }
            if ((*y) >= SIZE_Y)
            {
                (*y) = 0;
            }

            led_nb--;
        }
        else
        {
            // Found it
            return true;
        }

    } while (led_nb);

    return false;
}

static bool ledcube_anim_rand_led_from_cube(uint8_t* x, uint8_t* y, uint8_t *z, bool set)
{
    uint32_t led_nb = SIZE_X * SIZE_Y * SIZE_Z;
    (*x) = rand()%SIZE_X;
    (*y) = rand()%SIZE_Y;
    (*z) = rand()%SIZE_Z;

    do
    {
        if (ledcube_led_get(*x, *y, *z) != set)
        {
            // Search for next
            (*x)++;
            if ((*x) >= SIZE_X)
            {
                (*x) = 0;
                (*y)++;
            }
            if ((*y) >= SIZE_Y)
            {
                (*y) = 0;
                (*z)++;
            }
            if ((*z) >= SIZE_Z)
            {
                (*z) = 0;
            }

            led_nb--;
        }
        else
        {
            // Found it
            return true;
        }

    } while (led_nb);

    return false;
}

bool ledcube_anim_rain(anim_buffer_t* a)
{
    if (!a->step)
    {
        ledcube_set();
        a->ticks = 4;
    }

    ledcube_shift_layers_down();
    ledcube_led_set(rand()%SIZE_X, rand()%SIZE_Y, SIZE_Z-1);

    if (a->step++ > 100)
    {
        a->step = 0;
        return true;
    }

    return false;
}

bool ledcube_anim_appear(anim_buffer_t* a)
{
    if (!a->step)
    {
        ledcube_clr();
        a->ticks = 1;
    }

    if (ledcube_anim_rand_led_from_cube(&(a->x), &(a->y), &(a->z), false))
    {
        ledcube_led_set(a->x, a->y, a->z);
        a->step++;
    }
    else
    {
        a->step = 0;
        return true;
    }

    return false;
}

bool ledcube_anim_fade(anim_buffer_t* a)
{
    if (!a->step)
    {
        ledcube_set();
        a->ticks = 1;
    }

    if (ledcube_anim_rand_led_from_cube(&(a->x), &(a->y), &(a->z), true))
    {
        ledcube_led_clr(a->x, a->y, a->z);
        a->step++;
    }
    else
    {
        a->step = 0;
        return true;
    }

    return false;
}

bool ledcube_anim_fallin_rand(anim_buffer_t* a)
{
    if (!a->step)
    {
        ledcube_clr();
        a->ticks = 2;
    }

    // Shift every LED down by 1, keep bottom layers
    for (a->x = 0; a->x < SIZE_X; a->x++)
    {
        for (a->y = 0; a->y < SIZE_Y; a->y++)
        {
            for (a->z = 0; a->z < SIZE_Z-1; a->z++)
            {
                if (!ledcube_led_get(a->x, a->y, a->z) && ledcube_led_get(a->x, a->y, a->z+1))
                {
                    ledcube_led_set(a->x, a->y, a->z);
                    ledcube_led_clr(a->x, a->y, a->z+1);
                }
            }
        }
    }

    if (ledcube_anim_rand_led_from_layer(&(a->x), &(a->y), SIZE_Z-1, false))
    {
        ledcube_led_set(a->x, a->y, SIZE_Z-1);
    }
    else
    {
        a->step = 0;
        return true;
    }

    a->step++;

    return false;
}

bool ledcube_anim_fallin_layer(anim_buffer_t* a)
{
    uint8_t fall = false;

    if (!a->step)
    {
//        ledcube_clr();
        a->ticks = 2;
    }

    // Shift every LED down by 1, keep bottom layers
    for (a->x = 0; a->x < SIZE_X; a->x++)
    {
        for (a->y = 0; a->y < SIZE_Y; a->y++)
        {
            for (a->z = 0; a->z < SIZE_Z-1; a->z++)
            {
                if (!ledcube_led_get(a->x, a->y, a->z) && ledcube_led_get(a->x, a->y, a->z+1))
                {
                    ledcube_led_set(a->x, a->y, a->z);
                    ledcube_led_clr(a->x, a->y, a->z+1);
                    fall = true;
                }
            }
        }
    }

    if (!fall)
    {
        a->z = 0;
        while (a->z < SIZE_Z)
        {
            if (ledcube_anim_rand_led_from_layer(&(a->x), &(a->y), a->z, false))
            {
                break;
            }
            a->z++;
        }

        if (a->z < SIZE_Z)
        {
            ledcube_led_set(a->x, a->y, SIZE_Z-1);
        }
        else
        {
            a->step = 0;
            return true;
        }
    }

    a->step++;

    return false;
}

bool ledcube_anim_planes(anim_buffer_t* a)
{
    if (!a->step)
    {
        a->x = 0;
        a->y = 0;
        a->z = 0;
        a->ticks = 4;
    }

    ledcube_clr();

    if (a->step < SIZE_X)
    {
        ledcube_plane_x_set(a->x++);
    }
    else if (a->step < (2*SIZE_X))
    {
        ledcube_plane_x_set(--a->x);
    }
    else if (a->step < ((2*SIZE_X) + SIZE_Y))
    {
        ledcube_plane_y_set(a->y++);
    }
    else if (a->step < ((2*SIZE_X) + (2*SIZE_Y)))
    {
        ledcube_plane_y_set(--a->y);
    }
    else if (a->step < ((2*SIZE_X) + (2*SIZE_Y) + SIZE_Z))
    {
        a->ticks = 2;
        ledcube_plane_z_set(a->z++);
    }
    else if (a->step < ((2*SIZE_X) + (2*SIZE_Y) + (2*SIZE_Z)))
    {
        ledcube_plane_z_set(--a->z);
    }
    else
    {
        a->step = 0;
        return true;
    }

    a->step++;

    return false;
}

bool ledcube_anim_spiral(anim_buffer_t* a)
{
    if (!a->step)
    {
//        ledcube_clr();
        a->x = 0;
        a->y = 0;
        a->ticks = 4;
    }

    ledcube_shift_layers_down();
    ledcube_led_set(a->x, a->y, SIZE_Z-1);

    if (0 == a->x && a->y < SIZE_Y-1)
    {
        a->y++;
    }
    else if (SIZE_Y-1 == a->y && a->x < SIZE_X-1)
    {
        a->x++;
    }
    else if (SIZE_X-1 == a->x && a->y > 0)
    {
        a->y--;
    }
    else if (0 == a->y && a->x > 0)
    {
        a->x--;
    }

    if (a->step++ > 100)
    {
        a->step = 0;
        return true;
    }

    return false;
}

bool ledcube_anim_heli(anim_buffer_t* a)
{
    if (!a->step)
    {
//        ledcube_clr();
//        a->x = 0;
//        a->y = 0;
        a->ticks = 4;
    }

    ledcube_shift_layers_down();
    ledcube_led_set(a->x, a->y, SIZE_Z-1);
    ledcube_led_set((SIZE_X-1) - a->x, (SIZE_Y-1) - a->y, SIZE_Z-1);

    if (0 == a->x && a->y < SIZE_Y-1)
    {
        a->y++;
    }
    else if (SIZE_Y-1 == a->y && a->x < SIZE_X-1)
    {
        a->x++;
    }
    else if (SIZE_X-1 == a->x && a->y > 0)
    {
        a->y--;
    }
    else if (0 == a->y && a->x > 0)
    {
        a->x--;
    }

    if (a->step++ > 100)
    {
        a->step = 0;
        return true;
    }

    return false;
}

