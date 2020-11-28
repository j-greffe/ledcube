#ifndef _HAL_CLK_P_H_
#define _HAL_CLK_P_H_

// Private include

#include <stdint.h>

#define HAL_CLK_BASE_ACLK   (32768) // Hz

typedef enum {
    HAL_CLK_MCLK,
    HAL_CLK_SMCLK,
    HAL_CLK_ACLK,
} hal_clk_type_t;

void hal_clk_set(hal_clk_type_t clock, uint32_t frequency);
uint32_t hal_clk_get(hal_clk_type_t clock);

#endif // _HAL_CLK_P_H_
