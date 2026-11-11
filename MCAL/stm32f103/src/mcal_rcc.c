#include "mcal_rcc.h"

uint32_t SystemCoreClock = 8000000UL;

void RCC_SystemClock_Config_72MHz(void)
{
    uint32_t timeout = 0x5000;

    /* 1. Turn on HSE */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY) && --timeout)
    {
    }

    if (timeout > 0)
    {
        /* Flash 2 wait states for 72MHz & Enable Prefetch Buffer */
        FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;

        /* HCLK = SYSCLK (AHB prescaler = 1) */
        /* PCLK2 = HCLK (APB2 prescaler = 1) */
        /* PCLK1 = HCLK/2 (APB1 prescaler = 2, max 36MHz) */
        RCC->CFGR |= (RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2);

        /* PLL config: HSE * 9 = 8MHz * 9 = 72MHz */
        RCC->CFGR &= ~( (0x0FUL << 18U) | (1UL << 16U) | (1UL << 17U) );
        RCC->CFGR |= (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL9);

        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;
        while (!(RCC->CR & RCC_CR_PLLRDY))
        {
        }

        /* Select PLL as system clock source */
        RCC->CFGR &= ~RCC_CFGR_SW_PLL;
        RCC->CFGR |= RCC_CFGR_SW_PLL;

        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR & RCC_CFGR_SWS_MSK) != RCC_CFGR_SWS_PLL)
        {
        }

        SystemCoreClock = 72000000UL;
    }
    else
    {
        /* If HSE startup failed, continue with 8MHz HSI */
        SystemCoreClock = 8000000UL;
    }
}

void RCC_EnableAPB2Periph(uint32_t periph)
{
    RCC->APB2ENR |= periph;
}

uint32_t RCC_GetSystemClockFreq(void)
{
    return SystemCoreClock;
}
