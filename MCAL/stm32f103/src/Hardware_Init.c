#include "Hardware_Init.h"

void SystemInit(void)
{
    /* Reset the RCC clock configuration to default reset state */
    RCC->CR |= RCC_CR_HSION;
    RCC->CFGR &= 0xF8FF0000U;
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_HSEBYP);
    RCC->CIR = 0x009F0000U; /* Clear all interrupt flags and disable RCC interrupts */
}

void Hardware_EarlyInit(void)
{
    /* 1. Configure System Clock to 72MHz */
    RCC_SystemClock_Config_72MHz();

    /* 2. Configure SysTick for 1ms timebase based on actual Clock frequency */
    SysTick_Init(RCC_GetSystemClockFreq());
}
