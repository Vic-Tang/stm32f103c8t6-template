#ifndef STM32F103XB_H
#define STM32F103XB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define __IO volatile
#define __I  volatile const
#define __O  volatile

/* =========================================================================================================================== */
/* ================                              Memory Map & Base Addresses                                  ================ */
/* =========================================================================================================================== */

#define FLASH_BASE            (0x08000000UL)
#define SRAM_BASE             (0x20000000UL)
#define PERIPH_BASE           (0x40000000UL)

#define APB1PERIPH_BASE       (PERIPH_BASE)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)

/* Core Private Peripheral Bus (PPB) */
#define SCS_BASE              (0xE000E000UL)
#define SysTick_BASE          (SCS_BASE + 0x0010UL)
#define NVIC_BASE             (SCS_BASE + 0x0100UL)
#define SCB_BASE              (SCS_BASE + 0x0D00UL)

/* APB2 Peripherals */
#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400UL)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000UL)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400UL)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800UL)

/* AHB Peripherals */
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000UL)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x2000UL)

/* =========================================================================================================================== */
/* ================                               Peripheral Structure Types                                  ================ */
/* =========================================================================================================================== */

/* SysTick */
typedef struct
{
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __I  uint32_t CALIB;
} SysTick_TypeDef;

/* FLASH Interface */
typedef struct
{
    __IO uint32_t ACR;
    __IO uint32_t KEYR;
    __IO uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t AR;
    __IO uint32_t RESERVED;
    __IO uint32_t OBR;
    __IO uint32_t WRPR;
} FLASH_TypeDef;

/* RCC */
typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t APB1RSTR;
    __IO uint32_t AHBENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t APB1ENR;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
} RCC_TypeDef;

/* GPIO */
typedef struct
{
    __IO uint32_t CRL;
    __IO uint32_t CRH;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
} GPIO_TypeDef;

/* AFIO */
typedef struct
{
    __IO uint32_t EVCR;
    __IO uint32_t MAPR;
    __IO uint32_t EXTICR[4];
    __IO uint32_t RESERVED0;
    __IO uint32_t MAPR2;
} AFIO_TypeDef;

/* =========================================================================================================================== */
/* ================                                Peripheral Declarations                                    ================ */
/* =========================================================================================================================== */

#define SysTick               ((SysTick_TypeDef *) SysTick_BASE)
#define FLASH                 ((FLASH_TypeDef *) FLASH_R_BASE)
#define RCC                   ((RCC_TypeDef *) RCC_BASE)
#define AFIO                  ((AFIO_TypeDef *) AFIO_BASE)
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD                 ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                 ((GPIO_TypeDef *) GPIOE_BASE)

/* =========================================================================================================================== */
/* ================                                Bit Definitions & Masks                                    ================ */
/* =========================================================================================================================== */

/* FLASH ACR bits */
#define FLASH_ACR_LATENCY_POS        (0U)
#define FLASH_ACR_LATENCY_MSK        (0x07UL << FLASH_ACR_LATENCY_POS)
#define FLASH_ACR_LATENCY_0          (0x00UL)
#define FLASH_ACR_LATENCY_1          (0x01UL)
#define FLASH_ACR_LATENCY_2          (0x02UL)
#define FLASH_ACR_PRFTBE             (1UL << 4U)

/* RCC CR bits */
#define RCC_CR_HSION                 (1UL << 0U)
#define RCC_CR_HSIRDY                (1UL << 1U)
#define RCC_CR_HSEON                 (1UL << 16U)
#define RCC_CR_HSERDY                (1UL << 17U)
#define RCC_CR_HSEBYP                (1UL << 18U)
#define RCC_CR_CSSON                 (1UL << 19U)
#define RCC_CR_PLLON                 (1UL << 24U)
#define RCC_CR_PLLRDY                (1UL << 25U)

/* RCC CFGR bits */
#define RCC_CFGR_SW_POS              (0U)
#define RCC_CFGR_SW_HSI              (0x00UL << RCC_CFGR_SW_POS)
#define RCC_CFGR_SW_HSE              (0x01UL << RCC_CFGR_SW_POS)
#define RCC_CFGR_SW_PLL              (0x02UL << RCC_CFGR_SW_POS)
#define RCC_CFGR_SWS_POS             (2U)
#define RCC_CFGR_SWS_MSK             (0x03UL << RCC_CFGR_SWS_POS)
#define RCC_CFGR_SWS_HSI             (0x00UL << RCC_CFGR_SWS_POS)
#define RCC_CFGR_SWS_HSE             (0x01UL << RCC_CFGR_SWS_POS)
#define RCC_CFGR_SWS_PLL             (0x02UL << RCC_CFGR_SWS_POS)
#define RCC_CFGR_HPRE_DIV1           (0x00UL << 4U)
#define RCC_CFGR_PPRE1_DIV2          (0x04UL << 8U)
#define RCC_CFGR_PPRE2_DIV1          (0x00UL << 11U)
#define RCC_CFGR_PLLSRC_HSE          (1UL << 16U)
#define RCC_CFGR_PLLMUL9             (0x07UL << 18U)

/* RCC APB2ENR bits */
#define RCC_APB2ENR_AFIOEN           (1UL << 0U)
#define RCC_APB2ENR_IOPAEN           (1UL << 2U)
#define RCC_APB2ENR_IOPBEN           (1UL << 3U)
#define RCC_APB2ENR_IOPCEN           (1UL << 4U)
#define RCC_APB2ENR_IOPDEN           (1UL << 5U)
#define RCC_APB2ENR_IOPEEN           (1UL << 6U)

/* SysTick bits */
#define SysTick_CTRL_ENABLE_Msk      (1UL << 0U)
#define SysTick_CTRL_TICKINT_Msk     (1UL << 1U)
#define SysTick_CTRL_CLKSOURCE_Msk   (1UL << 2U)
#define SysTick_CTRL_COUNTFLAG_Msk   (1UL << 16U)

#ifdef __cplusplus
}
#endif

#endif /* STM32F103XB_H */
