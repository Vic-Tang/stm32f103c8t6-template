#include "bsp_led.h"

void LED_Init(void)
{
    /* 1. 使能 GPIOC 时钟 */
    RCC_EnableAPB2Periph(LED_RCC_PERIPH);

    /* 2. 配置 PC13 为推挽输出 2MHz */
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_2MHZ);

    /* 3. 默认熄灭 (高电平) */
    LED_Off();
}

void LED_On(void)
{
    GPIO_ResetPin(LED_PORT, LED_PIN);
}

void LED_Off(void)
{
    GPIO_SetPin(LED_PORT, LED_PIN);
}

void LED_Toggle(void)
{
    GPIO_TogglePin(LED_PORT, LED_PIN);
}

void LED_Set(uint8_t state)
{
    if (state)
    {
        LED_On();
    }
    else
    {
        LED_Off();
    }
}

uint8_t LED_GetState(void)
{
    /* PC13 低电平代表点亮 */
    return (GPIO_ReadPin(LED_PORT, LED_PIN) == 0) ? 1U : 0U;
}
