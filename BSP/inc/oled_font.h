#ifndef OLED_FONT_H
#define OLED_FONT_H

#include <stdint.h>

/* 6x8 ASCII 字符点阵字库 (ASCII 32 ' ' ~ 126 '~') */
extern const uint8_t OLED_F6x8[][6];

/* 8x16 ASCII 字符点阵字库 (ASCII 32 ' ' ~ 126 '~') */
extern const uint8_t OLED_F8x16[][16];

#endif /* OLED_FONT_H */
