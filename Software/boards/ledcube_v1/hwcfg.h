#include "hal.h"
#include "ledcube.h"

// Port 1
#define IO_ENABLE       HAL_IO_PACK(2, 1) // Pin 2
#define IO_RX           HAL_IO_PACK(1, 1) // Pin 3
#define IO_TX           HAL_IO_PACK(1, 2) // Pin 4
#define IO_BUTTON       HAL_IO_PACK(1, 3) // Pin 5
#define IO_RCK          HAL_IO_PACK(1, 4) // Pin 6
#define IO_SCL          HAL_IO_PACK(1, 5) // Pin 7
#define IO_SER          HAL_IO_PACK(1, 6) // Pin 8
#define IO_SCK          HAL_IO_PACK(1, 7) // Pin 9

// Port 2
#define IO_XIN          HAL_IO_PACK(2, 6) // Pin 12 XXX Do not modify
#define IO_XOUT         HAL_IO_PACK(2, 7) // Pin 13 XXX Do not modify

extern const hal_hwcfg_t                        k_hal_hwcfg;
extern const ledcube_hwcfg_t                    k_ledcube_hwcfg;
