#ifndef OLED_H
#define OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "mcal_i2c.h"

/* =========================================================================================================================== */
/* ================                                   OLED Configuration                                      ================ */
/* =========================================================================================================================== */

#define OLED_WIDTH             128
#define OLED_HEIGHT            64

/* 常见 0.96 寸 I2C OLED 地址为 0x78 或 0x7A (7位地址 0x3C / 0x3D) */
#define OLED_I2C_ADDR          0x78

/* =========================================================================================================================== */
/* ================                                     OLED APIs                                             ================ */
/* =========================================================================================================================== */

/**
 * @brief 初始化 OLED 屏幕 (完成 I2C 初始化、自适应地址探测与硬件点亮序列)
 */
void OLED_Init(void);

/**
 * @brief 清空显存缓冲区并刷新屏幕
 */
void OLED_Clear(void);

/**
 * @brief 清除指定矩形区域显存 (用于局部擦除刷新)
 */
void OLED_ClearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

/**
 * @brief 将内部显存缓冲区内容刷新到物理屏幕上
 */
void OLED_Update(void);

/**
 * @brief 开启 OLED 显示
 */
void OLED_DisplayOn(void);

/**
 * @brief 关闭 OLED 显示 (进入休眠省电模式)
 */
void OLED_DisplayOff(void);

/**
 * @brief 在显存指定位置打点
 * @param x 横坐标 (0 ~ 127)
 * @param y 纵坐标 (0 ~ 63)
 * @param color 1: 点亮像素, 0: 熄灭像素
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief 在指定位置显示单个字符
 * @param x 横坐标 (0 ~ 127)
 * @param y 纵坐标 (0 ~ 63)
 * @param ch 字符内容 (ASCII)
 * @param size 字体大小: 12 (6x8 点阵) 或 16 (8x16 点阵)
 * @param mode 1: 正常显示, 0: 反色显示
 */
void OLED_ShowChar(uint8_t x, uint8_t y, char ch, uint8_t size, uint8_t mode);

/**
 * @brief 在指定坐标显示字符串
 * @param x 起始横坐标 (0 ~ 127)
 * @param y 起始纵坐标 (0 ~ 63)
 * @param str 要显示的字符串指针
 * @param size 字体大小: 12 (6x8 点阵) 或 16 (8x16 点阵)
 */
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size);

/**
 * @brief 在指定坐标显示无符号整数
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param num 数字数值
 * @param len 显示位数 (不足高位补0)
 * @param size 字体大小 (12 或 16)
 */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* OLED_H */
