#include <msp430.h>
#include <stdint.h>
#include "hal_sys.h"

void hal_sys_software_reset(void* param)
{
    WDTCTL = 0xDEAD;
}
