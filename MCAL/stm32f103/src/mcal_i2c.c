#include "mcal_i2c.h"

static inline void I2C_Delay(void)
{
    /* 72MHz 下循环 150 次约为 5~6 微秒 (对应 I2C 速率约 40~50kHz，极其稳健) */
    for (volatile uint32_t i = 0; i < 150; i++)
    {
        __asm__ volatile ("nop");
    }
}

static inline void I2C_SCL_High(void)
{
    GPIO_SetPin(I2C_PORT, I2C_SCL_PIN);
}

static inline void I2C_SCL_Low(void)
{
    GPIO_ResetPin(I2C_PORT, I2C_SCL_PIN);
}

static inline void I2C_SDA_High(void)
{
    GPIO_SetPin(I2C_PORT, I2C_SDA_PIN);
}

static inline void I2C_SDA_Low(void)
{
    GPIO_ResetPin(I2C_PORT, I2C_SDA_PIN);
}

static inline uint8_t I2C_SDA_Read(void)
{
    return GPIO_ReadPin(I2C_PORT, I2C_SDA_PIN);
}

static inline void I2C_SDA_In(void)
{
    /* 配置 SDA (PB7) 为上拉/下拉输入模式: CNF=10, MODE=00 */
    I2C_PORT->CRL &= ~(0x0FUL << 28U);
    I2C_PORT->CRL |= (0x08UL << 28U);
    I2C_PORT->ODR |= I2C_SDA_PIN; // 开启上拉电阻（ODR=1为上拉）
}

static inline void I2C_SDA_Out(void)
{
    /* 配置 SDA (PB7) 为推挽输出模式 (50MHz): CNF=00, MODE=11 */
    I2C_PORT->CRL &= ~(0x0FUL << 28U);
    I2C_PORT->CRL |= (0x03UL << 28U);
}

void I2C_Bus_Init(void)
{
    /* 1. 使能 GPIO 端口时钟 */
    RCC_EnableAPB2Periph(I2C_RCC_PERIPH);

    /* 2. 配置 SCL (PB6) 为推挽输出 50MHz, SDA (PB7) 为推挽输出 50MHz */
    GPIO_Init(I2C_PORT, I2C_SCL_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_50MHZ);
    I2C_SDA_Out();

    /* 3. 初始状态为总线空闲 (SCL 与 SDA 均为高电平) */
    I2C_SCL_High();
    I2C_SDA_High();
}

void I2C_Start(void)
{
    I2C_SDA_High();
    I2C_SCL_High();
    I2C_Delay();
    I2C_SDA_Low();
    I2C_Delay();
    I2C_SCL_Low();
    I2C_Delay();
}

void I2C_Stop(void)
{
    I2C_SDA_Low();
    I2C_SCL_Low();
    I2C_Delay();
    I2C_SCL_High();
    I2C_Delay();
    I2C_SDA_High();
    I2C_Delay();
}

uint8_t I2C_SendByte(uint8_t byte)
{
    uint8_t ack = 0;

    I2C_SDA_Out();

    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x80)
        {
            I2C_SDA_High();
        }
        else
        {
            I2C_SDA_Low();
        }
        byte <<= 1;
        I2C_Delay();
        I2C_SCL_High();
        I2C_Delay();
        I2C_SCL_Low();
        I2C_Delay();
    }

    /* 切换为输入模式读取 ACK */
    I2C_SDA_In();
    I2C_Delay();
    I2C_SCL_High();
    I2C_Delay();
    ack = I2C_SDA_Read();
    I2C_SCL_Low();
    I2C_Delay();
    I2C_SDA_Out();

    return ack; /* 0 表示接收到 ACK，1 表示 NACK */
}

uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t byte = 0;

    I2C_SDA_In();
    for (uint8_t i = 0; i < 8; i++)
    {
        byte <<= 1;
        I2C_SCL_High();
        I2C_Delay();
        if (I2C_SDA_Read())
        {
            byte |= 0x01;
        }
        I2C_SCL_Low();
        I2C_Delay();
    }

    I2C_SDA_Out();
    /* 发送应答信号 */
    if (ack)
    {
        I2C_SDA_Low();  /* ACK */
    }
    else
    {
        I2C_SDA_High(); /* NACK */
    }
    I2C_Delay();
    I2C_SCL_High();
    I2C_Delay();
    I2C_SCL_Low();
    I2C_Delay();
    I2C_SDA_High();

    return byte;
}

uint8_t I2C_CheckDevice(uint8_t devAddr)
{
    uint8_t writeAddr = (devAddr < 0x80) ? (devAddr << 1) : devAddr;
    uint8_t ack = 1;

    I2C_Start();
    ack = I2C_SendByte(writeAddr);
    I2C_Stop();

    return ack; /* 0: 收到从机 ACK, 1: 无应答 NACK */
}

uint8_t I2C_Scan(void)
{
    /* 扫描 7 位地址 0x08 ~ 0x77 */
    for (uint8_t addr = 0x08; addr <= 0x77; addr++)
    {
        if (I2C_CheckDevice(addr) == 0)
        {
            return (addr << 1); /* 返回有效 8 位写地址 */
        }
    }
    return 0;
}

uint8_t I2C_WriteRegBytes(uint8_t devAddr, uint8_t regAddr, const uint8_t *data, uint16_t len)
{
    uint8_t writeAddr = (devAddr < 0x80) ? (devAddr << 1) : devAddr;

    I2C_Start();
    I2C_SendByte(writeAddr);
    I2C_SendByte(regAddr);

    for (uint16_t i = 0; i < len; i++)
    {
        I2C_SendByte(data[i]);
    }

    I2C_Stop();
    return 0;
}
