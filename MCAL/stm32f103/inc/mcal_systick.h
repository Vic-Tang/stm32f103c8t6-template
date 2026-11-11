#ifndef MCAL_SYSTICK_H
#define MCAL_SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f103xb.h"

/**
 * @brief Initialize SysTick timer for 1ms tick interrupt
 * @param sysclk_freq System clock frequency in Hz (e.g. 72000000UL)
 */
void SysTick_Init(uint32_t sysclk_freq);

/**
 * @brief Accurate millisecond delay using SysTick
 * @param ms Milliseconds to delay
 */
void Delay_Ms(uint32_t ms);

/**
 * @brief Get total milliseconds elapsed since startup
 */
uint32_t Get_Tick(void);

#ifdef __cplusplus
}
#endif

#endif /* MCAL_SYSTICK_H */
