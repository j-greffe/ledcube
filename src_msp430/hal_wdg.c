#include <msp430.h>

void hal_wdg_stop(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
}
