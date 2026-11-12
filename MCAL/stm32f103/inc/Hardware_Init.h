#ifndef HARDWARE_INIT_H
#define HARDWARE_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f103xb.h"
#include "mcal_rcc.h"
#include "mcal_gpio.h"
#include "mcal_systick.h"
#include "mcal_i2c.h"

/**
 * @brief Initialize low level MCU peripherals (72MHz Clock, SysTick, etc.)
 */
void Hardware_EarlyInit(void);

#ifdef __cplusplus
}
#endif

#endif // HARDWARE_INIT_H
