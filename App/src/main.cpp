#include "main.h"
#include "Hardware_Init.h"

/**
 * @brief 板载 LED 指示灯配置 (PC13)
 * - STM32F103C8T6 核心板板载 LED 通常接在 PC13，低电平点亮，高电平熄灭。
 */
#define LED_GPIO_PORT          GPIOC
#define LED_GPIO_PIN           GPIO_PIN_13

/* 常见板载 LED 为低电平点亮（灌电流接法），若为高电平点亮可对调电平宏 */
#define LED_ON()               GPIO_ResetPin(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_OFF()              GPIO_SetPin(LED_GPIO_PORT, LED_GPIO_PIN)

int main(void)
{
    /* 1. Low level MCAL Hardware Initialization (72MHz SysClk, SysTick 1ms) */
    Hardware_EarlyInit();

    /* 2. Run Application logic */
    return App_Main();
}

int App_Main(void)
{
    /* 1. 使能 GPIOC 外设时钟 */
    RCC_EnableAPB2Periph(RCC_APB2ENR_IOPCEN);

    /* 2. 初始化 PC13 为推挽输出模式 (最高速率 2MHz 足够 LED/信号指示且低功耗) */
    GPIO_Init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_2MHZ);

    /* 初始状态：灭 */
    LED_OFF();

    /* Application loop: 周期 6s (亮 3s，灭 3s) */
    while (1)
    {
        /* 点亮 LED (低电平 0V)，保持 3 秒 */
        LED_ON();
        Delay_Ms(3000);

        /* 熄灭 LED (高电平 3.3V)，保持 3 秒 */
        LED_OFF();
        Delay_Ms(1000);
    }

    return 0;
}
