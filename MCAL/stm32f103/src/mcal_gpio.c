#include "mcal_gpio.h"

void GPIO_Init(GPIO_TypeDef *port, uint16_t pinMask, GPIO_Mode_t mode, GPIO_Speed_t speed)
{
    uint32_t currentMode = 0;

    /* Determine the 4-bit configuration for the pin */
    if ((mode == GPIO_MODE_OUTPUT_PP) || (mode == GPIO_MODE_OUTPUT_OD) ||
        (mode == GPIO_MODE_AF_PP) || (mode == GPIO_MODE_AF_OD))
    {
        currentMode = ((uint32_t)mode & 0x0CUL) | ((uint32_t)speed & 0x03UL);
    }
    else
    {
        /* Input modes: speed bits are 00 */
        if (mode == GPIO_MODE_INPUT_PULLUP || mode == GPIO_MODE_INPUT_PULLDOWN)
        {
            currentMode = 0x08UL; /* CNF = 10, MODE = 00 */
        }
        else
        {
            currentMode = ((uint32_t)mode & 0x0CUL);
        }
    }

    for (uint8_t pin = 0; pin < 16; pin++)
    {
        if (pinMask & (1U << pin))
        {
            if (pin < 8)
            {
                uint32_t shift = pin * 4U;
                port->CRL &= ~(0x0FUL << shift);
                port->CRL |= (currentMode << shift);
            }
            else
            {
                uint32_t shift = (pin - 8U) * 4U;
                port->CRH &= ~(0x0FUL << shift);
                port->CRH |= (currentMode << shift);
            }

            /* Handle pull-up / pull-down resistor selection via ODR */
            if (mode == GPIO_MODE_INPUT_PULLUP)
            {
                port->BSRR = (1U << pin);
            }
            else if (mode == GPIO_MODE_INPUT_PULLDOWN)
            {
                port->BRR = (1U << pin);
            }
        }
    }
}

void GPIO_SetPin(GPIO_TypeDef *port, uint16_t pinMask)
{
    port->BSRR = (uint32_t)pinMask;
}

void GPIO_ResetPin(GPIO_TypeDef *port, uint16_t pinMask)
{
    port->BRR = (uint32_t)pinMask;
}

void GPIO_TogglePin(GPIO_TypeDef *port, uint16_t pinMask)
{
    port->ODR ^= (uint32_t)pinMask;
}

void GPIO_WritePin(GPIO_TypeDef *port, uint16_t pinMask, uint8_t state)
{
    if (state)
    {
        port->BSRR = (uint32_t)pinMask;
    }
    else
    {
        port->BRR = (uint32_t)pinMask;
    }
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pinMask)
{
    return (port->IDR & pinMask) ? 1U : 0U;
}
