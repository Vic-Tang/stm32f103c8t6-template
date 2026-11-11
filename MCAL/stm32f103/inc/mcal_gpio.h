#ifndef MCAL_GPIO_H
#define MCAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f103xb.h"

/* GPIO Pin definitions */
#define GPIO_PIN_0                 ((uint16_t)0x0001)
#define GPIO_PIN_1                 ((uint16_t)0x0002)
#define GPIO_PIN_2                 ((uint16_t)0x0004)
#define GPIO_PIN_3                 ((uint16_t)0x0008)
#define GPIO_PIN_4                 ((uint16_t)0x0010)
#define GPIO_PIN_5                 ((uint16_t)0x0020)
#define GPIO_PIN_6                 ((uint16_t)0x0040)
#define GPIO_PIN_7                 ((uint16_t)0x0080)
#define GPIO_PIN_8                 ((uint16_t)0x0100)
#define GPIO_PIN_9                 ((uint16_t)0x0200)
#define GPIO_PIN_10                ((uint16_t)0x0400)
#define GPIO_PIN_11                ((uint16_t)0x0800)
#define GPIO_PIN_12                ((uint16_t)0x1000)
#define GPIO_PIN_13                ((uint16_t)0x2000)
#define GPIO_PIN_14                ((uint16_t)0x4000)
#define GPIO_PIN_15                ((uint16_t)0x8000)
#define GPIO_PIN_ALL               ((uint16_t)0xFFFF)

/* GPIO Mode */
typedef enum
{
    GPIO_MODE_ANALOG            = 0x00, /* Input Analog */
    GPIO_MODE_INPUT_FLOATING    = 0x04, /* Input Floating */
    GPIO_MODE_INPUT_PULLDOWN    = 0x28, /* Input Pull-down */
    GPIO_MODE_INPUT_PULLUP      = 0x48, /* Input Pull-up */
    GPIO_MODE_OUTPUT_PP         = 0x00, /* General purpose output push-pull */
    GPIO_MODE_OUTPUT_OD         = 0x04, /* General purpose output open-drain */
    GPIO_MODE_AF_PP             = 0x08, /* Alternate function output push-pull */
    GPIO_MODE_AF_OD             = 0x0C  /* Alternate function output open-drain */
} GPIO_Mode_t;

/* GPIO Output Speed */
typedef enum
{
    GPIO_SPEED_10MHZ            = 0x01,
    GPIO_SPEED_2MHZ             = 0x02,
    GPIO_SPEED_50MHZ            = 0x03
} GPIO_Speed_t;

/* Function Prototypes */
void GPIO_Init(GPIO_TypeDef *port, uint16_t pinMask, GPIO_Mode_t mode, GPIO_Speed_t speed);
void GPIO_SetPin(GPIO_TypeDef *port, uint16_t pinMask);
void GPIO_ResetPin(GPIO_TypeDef *port, uint16_t pinMask);
void GPIO_TogglePin(GPIO_TypeDef *port, uint16_t pinMask);
void GPIO_WritePin(GPIO_TypeDef *port, uint16_t pinMask, uint8_t state);
uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pinMask);

#ifdef __cplusplus
}
#endif

#endif /* MCAL_GPIO_H */
