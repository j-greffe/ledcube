#ifndef _HAL_LPM_H_
#define _HAL_LPM_H_

#define HAL_LPM_ENABLE 0

extern volatile uint16_t g_hal_lpm_bits;

enum {
    LPM_0,
    LPM_1,
    LPM_2,
    LPM_3,
    LPM_4,
    LPM_NONE,
};

#if HAL_LPM_ENABLE
void hal_lpm_enter(uint8_t lpm);
#define hal_lpm_exit()  do { if (g_hal_lpm_bits) { __bic_SR_register_on_exit(g_hal_lpm_bits); g_hal_lpm_bits = 0; } } while (0)
#else
#define hal_lpm_enter(...);
#define hal_lpm_exit();
#endif

#endif // _HAL_LPM_H_
