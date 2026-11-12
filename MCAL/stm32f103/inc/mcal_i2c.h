#ifndef MCAL_I2C_H
#define MCAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f103xb.h"
#include "mcal_gpio.h"
#include "mcal_rcc.h"

/* =========================================================================================================================== */
/* ================                                   I2C Pin Definitions                                     ================ */
/* =========================================================================================================================== */

/* 
 * 默认使用 STM32F103 硬件 I2C1 对应的物理引脚:
 * SCL -> PB6
 * SDA -> PB7
 * 用户可在此根据实际硬件连接随意更换为其他 GPIO (如 PB8/PB9 或 PB10/PB11)
 */
#define I2C_PORT               GPIOB
#define I2C_RCC_PERIPH         RCC_APB2ENR_IOPBEN
#define I2C_SCL_PIN            GPIO_PIN_6
#define I2C_SDA_PIN            GPIO_PIN_7

/* =========================================================================================================================== */
/* ================                                        I2C APIs                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief 初始化 I2C 总线 GPIO (SCL/SDA 配置为开漏输出模式)
 */
void I2C_Bus_Init(void);

/**
 * @brief 产生 I2C 起始信号
 */
void I2C_Start(void);

/**
 * @brief 产生 I2C 停止信号
 */
void I2C_Stop(void);

/**
 * @brief 发送一个字节并等待应答
 * @param byte 要发送的字节
 * @return 0: 收到 ACK (成功), 1: 收到 NACK (无应答/失败)
 */
uint8_t I2C_SendByte(uint8_t byte);

/**
 * @brief 读取一个字节
 * @param ack 1: 发送 ACK, 0: 发送 NACK
 * @return 读取到的字节数据
 */
uint8_t I2C_ReadByte(uint8_t ack);

/**
 * @brief 检测指定 I2C 地址是否有从机 ACK 应答
 * @param devAddr 8位或7位地址
 * @return 0: 收到 ACK (设备在线), 1: 无应答
 */
uint8_t I2C_CheckDevice(uint8_t devAddr);

/**
 * @brief 扫描总线上的所有有效 I2C 设备地址 (0x08 ~ 0x77)
 * @return 找到的第一个设备 8 位写地址 (例如 0x78)，若未找到返回 0
 */
uint8_t I2C_Scan(void);

/**
 * @brief 向指定从机设备寄存器写入连续数据
 * @param devAddr 7位从机地址 (未左移或左移后均可，函数内部会自动处理或直接传入如 0x78)
 * @param regAddr 寄存器地址 / 命令标识 (如 SSD1306 0x00 为命令，0x40 为数据)
 * @param data 数据指针
 * @param len 数据长度
 * @return 0 成功, 非0 失败
 */
uint8_t I2C_WriteRegBytes(uint8_t devAddr, uint8_t regAddr, const uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* MCAL_I2C_H */
