#include "hal.h"
#include "ledcube.h"

// Port 1
#define IO_ENABLE                               HAL_IO_PACK(2, 1) // Pin 2
#define IO_RX                                   HAL_IO_PACK(1, 1) // Pin 3
#define IO_TX                                   HAL_IO_PACK(1, 2) // Pin 4
#define IO_BUTTON                               HAL_IO_PACK(1, 3) // Pin 5
#define IO_RCK                                  HAL_IO_PACK(2, 5) // Pin 6
#define IO_SCL                                  HAL_IO_PACK(2, 3) // Pin 7
#define IO_SER                                  HAL_IO_PACK(1, 6) // Pin 8
#define IO_SCK                                  HAL_IO_PACK(2, 4) // Pin 9

// Port 2
#define IO_XIN                                  HAL_IO_PACK(2, 6) // Pin 12 XXX Do not modify
#define IO_XOUT                                 HAL_IO_PACK(2, 7) // Pin 13 XXX Do not modify

extern const hal_hwcfg_t                        k_hal_hwcfg;

#define LEDCUBE_SIZE_X                          4
#define LEDCUBE_SIZE_Y                          4
#define LEDCUBE_SIZE_Z                          8
#define LEDCUBE_SHIFT_REGISTER_NB               3
extern const ledcube_hwcfg_t                    k_ledcube_hwcfg;
