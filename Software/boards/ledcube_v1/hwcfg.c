#include "hwcfg.h"
#include "shift_register_74hc595x.h"

const hal_hwcfg_t k_hal_hwcfg = {
    .xtal_32k = true,
};

const shift_register_74hc595x_hwcfg_t k_shift_register_hwcfg = {
    .io_ena = HAL_IO_NONE,
    .io_sck = IO_SCK,
    .io_ser = IO_SER,
    .io_rck = IO_RCK,
    .io_clr = IO_SCL
};

const ledcube_pin_t k_ledcube_pin_xy_0_hwcfg[LEDCUBE_SIZE_Y] = {
    LEDCUBE_PIN(2, 0),
    LEDCUBE_PIN(2, 1),
    LEDCUBE_PIN(2, 2),
    LEDCUBE_PIN(2, 3)
};

const ledcube_pin_t k_ledcube_pin_xy_1_hwcfg[LEDCUBE_SIZE_Y] = {
    LEDCUBE_PIN(2, 4),
    LEDCUBE_PIN(2, 5),
    LEDCUBE_PIN(2, 6),
    LEDCUBE_PIN(2, 7)
};

const ledcube_pin_t k_ledcube_pin_xy_2_hwcfg[LEDCUBE_SIZE_Y] = {
    LEDCUBE_PIN(1, 0),
    LEDCUBE_PIN(1, 1),
    LEDCUBE_PIN(1, 2),
    LEDCUBE_PIN(1, 3)
};

const ledcube_pin_t k_ledcube_pin_xy_3_hwcfg[LEDCUBE_SIZE_Y] = {
    LEDCUBE_PIN(1, 4),
    LEDCUBE_PIN(1, 5),
    LEDCUBE_PIN(1, 6),
    LEDCUBE_PIN(1, 7)
};

const ledcube_pin_t* k_ledcube_pin_xy_hwcfg[LEDCUBE_SIZE_X] = {
    k_ledcube_pin_xy_0_hwcfg,
    k_ledcube_pin_xy_1_hwcfg,
    k_ledcube_pin_xy_2_hwcfg,
    k_ledcube_pin_xy_3_hwcfg
};

const ledcube_pin_t k_ledcube_pin_z_hwcfg[LEDCUBE_SIZE_Z] = {
    LEDCUBE_PIN(0, 0),
    LEDCUBE_PIN(0, 1),
    LEDCUBE_PIN(0, 2),
    LEDCUBE_PIN(0, 3),
    LEDCUBE_PIN(0, 4),
    LEDCUBE_PIN(0, 5),
    LEDCUBE_PIN(0, 6),
    LEDCUBE_PIN(0, 7)
};

const ledcube_hwcfg_t k_ledcube_hwcfg = {
    .size_x = LEDCUBE_SIZE_X,
    .size_y = LEDCUBE_SIZE_Y,
    .size_z = LEDCUBE_SIZE_Z,
    .pin_xy = k_ledcube_pin_xy_hwcfg,
    .pin_z = k_ledcube_pin_z_hwcfg,
    .shift_register_nb = LEDCUBE_SHIFT_REGISTER_NB,
    .shift_register_hwcfg = (void*)&k_shift_register_hwcfg
};
