#ifndef BSP_LED_H
#define BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f103xb.h"
#include "mcal_gpio.h"
#include "mcal_rcc.h"

/* 板载 LED 引脚定义 (STM32F103C8T6 PC13) */
#define LED_PORT               GPIOC
#define LED_PIN                GPIO_PIN_13
#define LED_RCC_PERIPH         RCC_APB2ENR_IOPCEN

/**
 * @brief 初始化板载 LED 引脚
 */
void LED_Init(void);

/**
 * @brief 点亮 LED (输出低电平)
 */
void LED_On(void);

/**
 * @brief 熄灭 LED (输出高电平)
 */
void LED_Off(void);

/**
 * @brief 翻转 LED 状态
 */
void LED_Toggle(void);

/**
 * @brief 设置 LED 状态 (1: 点亮, 0: 熄灭)
 */
void LED_Set(uint8_t state);

/**
 * @brief 获取 LED 当前状态 (1: 点亮中, 0: 熄灭中)
 */
uint8_t LED_GetState(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_LED_H */
