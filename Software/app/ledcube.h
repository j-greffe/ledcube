#ifndef _LEDCUBE_H_
#define _LEDCUBE_H_

#include <stdint.h>
#include <stdbool.h>

#define LEDCUBE_PIN(_byte,_bit) (ledcube_pin_t){ .byte = _byte, .bit = (1 << _bit) }

typedef struct {
    uint8_t byte; // index of shift register
    uint8_t bit; // bit corresponding to the pin on the shift register
} ledcube_pin_t;

typedef struct {
    uint8_t size_x;
    uint8_t size_y;
    uint8_t size_z;
    const ledcube_pin_t** pin_xy;
    const ledcube_pin_t* pin_z;
    uint8_t shift_register_nb;
    void* shift_register_hwcfg;
} ledcube_hwcfg_t;

typedef struct  {
    uint16_t step;
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t ticks;
} anim_buffer_t;

void ledcube_open(void* hwcfg);
void ledcube_update_layer(uint8_t z);
void ledcube_set(void);
void ledcube_clr(void);
void ledcube_led_set(uint8_t x, uint8_t y, uint8_t z);
void ledcube_led_clr(uint8_t x, uint8_t y, uint8_t z);
bool ledcube_led_get(uint8_t x, uint8_t y, uint8_t z);
bool ledcube_layer_full(uint8_t z);
void ledcube_line_x_set(uint8_t x1, uint8_t x2, uint8_t y, uint8_t z);
void ledcube_line_y_set(uint8_t y1, uint8_t y2, uint8_t x, uint8_t z);
void ledcube_line_z_set(uint8_t z1, uint8_t z2, uint8_t x, uint8_t y);
void ledcube_rectangle_x_set(uint8_t y1, uint8_t z1, uint8_t y2, uint8_t z2, uint8_t x);
void ledcube_rectangle_y_set(uint8_t x1, uint8_t z1, uint8_t x2, uint8_t z2, uint8_t y);
void ledcube_rectangle_z_set(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t z);
void ledcube_plane_x_set(uint8_t x);
void ledcube_plane_y_set(uint8_t y);
void ledcube_plane_z_set(uint8_t z);
void ledcube_wireframe_set(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2);
void ledcube_shift_layers_down(void);

#endif // _LEDCUBE_H_
