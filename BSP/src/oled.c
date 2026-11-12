#include "oled.h"
#include "oled_font.h"
#include "mcal_i2c.h"
#include "mcal_systick.h"

/* 128x64 屏幕显存缓存: 8 行(Page), 每行 128 列 */
static uint8_t s_oledBuffer[8][128];
static uint8_t s_oledAddr = 0x78;

/* 
 * SH1106 I2C 协议标准：
 * - 发送命令时：控制字节为 0x00 (Co=0, D/C#=0)
 * - 发送单字节数据时：控制字节为 0x40 (Co=0, D/C#=1)
 */
static void SH1106_WriteCmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(s_oledAddr);
    I2C_SendByte(0x00); // Control byte: D/C# = 0 (Command)
    I2C_SendByte(cmd);
    I2C_Stop();
}

static void SH1106_WriteData(const uint8_t *data, uint16_t len)
{
    I2C_Start();
    I2C_SendByte(s_oledAddr);
    I2C_SendByte(0x40); // Control byte: D/C# = 1 (Data)
    for (uint16_t i = 0; i < len; i++)
    {
        I2C_SendByte(data[i]);
    }
    I2C_Stop();
}

void OLED_Init(void)
{
    /* 1. 初始化 I2C 总线 GPIO */
    I2C_Bus_Init();
    Delay_Ms(200); /* 必须等待 OLED 模块完全上电稳定 */

    /* 2. 探测真实 I2C 地址 (0x78 或 0x7A) */
    if (I2C_CheckDevice(0x78) == 0)
    {
        s_oledAddr = 0x78;
    }
    else if (I2C_CheckDevice(0x7A) == 0)
    {
        s_oledAddr = 0x7A;
    }
    else
    {
        s_oledAddr = 0x78; // 默认 fallback
    }

    /* 3. SH1106 官方数据手册标准上电复位初始化序列 */
    SH1106_WriteCmd(0xAE); // 1. 关闭显示 (Display OFF)
    
    SH1106_WriteCmd(0x02); // 2. 列起始低4位 (SH1106需偏移2列，低4位填0x02)
    SH1106_WriteCmd(0x10); // 3. 列起始高4位 (0x10)
    SH1106_WriteCmd(0x40); // 4. 起始行 (0)
    SH1106_WriteCmd(0xB0); // 5. 页地址 (0)

    SH1106_WriteCmd(0x81); // 6. 对比度控制
    SH1106_WriteCmd(0x80); // 对比度设为中等

    SH1106_WriteCmd(0xA1); // 7. 段重映射 (0xA0=左右反，0xA1=正常)
    SH1106_WriteCmd(0xC8); // 8. COM扫描方向 (0xC0=上下反，0xC8=正常)

    SH1106_WriteCmd(0xA6); // 9. 正常显示 (0xA6=正常点亮, 0xA7=反色)
    SH1106_WriteCmd(0xA8); // 10. 多路复用比
    SH1106_WriteCmd(0x3F); // 1/64 duty (128x64)

    /* 11. SH1106 最核心指令：开启内部 DC-DC 升压泵 */
    SH1106_WriteCmd(0xAD); // Set DC-DC
    SH1106_WriteCmd(0x8B); // 8B = 开启内置 DC-DC 升压

    SH1106_WriteCmd(0x33); // 12. 设置泵压输出为 9.0V (SH1106专用: 0x30~0x33)

    SH1106_WriteCmd(0xD3); // 13. 设置显示偏移
    SH1106_WriteCmd(0x00);

    SH1106_WriteCmd(0xD5); // 14. 设置时钟分频
    SH1106_WriteCmd(0x80);

    SH1106_WriteCmd(0xD9); // 15. 预充电周期
    SH1106_WriteCmd(0x1F);

    SH1106_WriteCmd(0xDA); // 16. COM引脚硬件配置
    SH1106_WriteCmd(0x12);

    SH1106_WriteCmd(0xDB); // 17. VCOMH 电平
    SH1106_WriteCmd(0x40);

    SH1106_WriteCmd(0xAF); // 18. 开启显示 (Display ON)

    /* 延时 100ms 等待内部 9V 升压建立稳定 */
    Delay_Ms(100);

    /* 清屏 */
    OLED_Clear();
}

void OLED_Clear(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        for (uint8_t col = 0; col < 128; col++)
        {
            s_oledBuffer[page][col] = 0x00;
        }
    }
    OLED_Update();
}

void OLED_ClearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    for (uint8_t i = 0; i < width; i++)
    {
        for (uint8_t j = 0; j < height; j++)
        {
            OLED_DrawPoint(x + i, y + j, 0);
        }
    }
}

void OLED_Update(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        /* 1. 设置当前目标页面地址 (0xB0 ~ 0xB7) */
        SH1106_WriteCmd(0xB0 + page);

        /* 2. SH1106 显存列偏移 2 列 (内部 132 列，显示 128 列) */
        SH1106_WriteCmd(0x02); // 列低4位 = 2
        SH1106_WriteCmd(0x10); // 列高4位 = 0

        /* 3. 分两包写入该页的 128 字节显存数据 (每包 64 字节) */
        SH1106_WriteData(&s_oledBuffer[page][0], 64);
        SH1106_WriteData(&s_oledBuffer[page][64], 64);
    }
}

void OLED_DisplayOn(void)
{
    SH1106_WriteCmd(0xAD);
    SH1106_WriteCmd(0x8B);
    SH1106_WriteCmd(0xAF);
}

void OLED_DisplayOff(void)
{
    SH1106_WriteCmd(0xAE);
    SH1106_WriteCmd(0xAD);
    SH1106_WriteCmd(0x8A);
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    uint8_t page = y / 8;
    uint8_t bit = y % 8;

    if (color)
    {
        s_oledBuffer[page][x] |= (1U << bit);
    }
    else
    {
        s_oledBuffer[page][x] &= ~(1U << bit);
    }
}

void OLED_ShowChar(uint8_t x, uint8_t y, char ch, uint8_t size, uint8_t mode)
{
    if (ch < ' ' || ch > '~')
    {
        ch = ' ';
    }

    uint8_t c = (uint8_t)(ch - ' ');

    if (size == 16)
    {
        /* 8x16 字体 */
        for (uint8_t i = 0; i < 16; i++)
        {
            uint8_t temp = OLED_F8x16[c][i];
            for (uint8_t j = 0; j < 8; j++)
            {
                uint8_t drawColor = (temp & 0x01) ? mode : !mode;
                uint8_t drawX = x + (i % 8);
                uint8_t drawY = y + (i / 8) * 8 + j;
                OLED_DrawPoint(drawX, drawY, drawColor);
                temp >>= 1;
            }
        }
    }
    else
    {
        /* 6x8 字体 (size == 12) */
        for (uint8_t i = 0; i < 6; i++)
        {
            uint8_t temp = OLED_F6x8[c][i];
            for (uint8_t j = 0; j < 8; j++)
            {
                uint8_t drawColor = (temp & 0x01) ? mode : !mode;
                OLED_DrawPoint(x + i, y + j, drawColor);
                temp >>= 1;
            }
        }
    }
}

void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size)
{
    uint8_t xStep = (size == 16) ? 8 : 6;
    uint8_t yStep = (size == 16) ? 16 : 8;

    while (*str != '\0')
    {
        if (x + xStep > OLED_WIDTH)
        {
            x = 0;
            y += yStep;
        }
        if (y + yStep > OLED_HEIGHT)
        {
            break;
        }

        OLED_ShowChar(x, y, *str, size, 1);
        x += xStep;
        str++;
    }
}

static uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t xStep = (size == 16) ? 8 : 6;
    uint8_t enshow = 0;

    for (uint8_t t = 0; t < len; t++)
    {
        uint8_t temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + t * xStep, y, ' ', size, 1);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }
        OLED_ShowChar(x + t * xStep, y, temp + '0', size, 1);
    }
}
