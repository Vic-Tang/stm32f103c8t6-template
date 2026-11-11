#ifndef MCAL_RCC_H
#define MCAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f103xb.h"

extern uint32_t SystemCoreClock;

/**
 * @brief Initialize system clock to 72MHz using 8MHz HSE + PLL.
 *        If HSE fails/not present, falls back gracefully to HSI.
 */
void RCC_SystemClock_Config_72MHz(void);

/**
 * @brief Enable APB2 peripheral clocks (e.g. RCC_APB2ENR_IOPDEN)
 */
void RCC_EnableAPB2Periph(uint32_t periph);

/**
 * @brief Get current System Core Clock frequency in Hz
 */
uint32_t RCC_GetSystemClockFreq(void);

#ifdef __cplusplus
}
#endif

#endif /* MCAL_RCC_H */
