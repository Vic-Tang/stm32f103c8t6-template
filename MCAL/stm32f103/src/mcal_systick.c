#include "mcal_systick.h"

static volatile uint32_t g_tickCount = 0;

void SysTick_Init(uint32_t sysclk_freq)
{
    /* Configure SysTick to generate interrupt every 1ms */
    SysTick->LOAD = (sysclk_freq / 1000UL) - 1UL;
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    g_tickCount++;
}

uint32_t Get_Tick(void)
{
    return g_tickCount;
}

void Delay_Ms(uint32_t ms)
{
    uint32_t startTick = g_tickCount;
    while ((g_tickCount - startTick) < ms)
    {
        __asm__ volatile ("nop");
    }
}
