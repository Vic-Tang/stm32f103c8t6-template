#include "main.h"
#include "Hardware_Init.h"
#include "bsp_led.h"
#include "oled.h"

int main(void)
{
    /* 1. 硬件层与板级层初始化 (72MHz时钟、SysTick定时器、LED、I2C与SH1106 OLED) */
    Hardware_EarlyInit();
    LED_Init();
    OLED_Init();

    /* 2. 进入应用层主逻辑 */
    return App_Main();
}

int App_Main(void)
{
    uint32_t counter = 0;

    /* while(1) 前完成首屏显示内容设置并开启 LED */
    OLED_Clear();
    OLED_ShowString(0, 0,  "STM32F103 SH1106", 16);  // 标题
    OLED_ShowString(0, 20, "System Status: OK", 12); // 系统状态
    OLED_ShowString(0, 36, "HelloWorld: LED ON ", 12);    // LED 指示
    OLED_ShowString(0, 50, "Cycle Count: ", 12);     // 循环计数
    OLED_ShowNum(78, 50, counter, 4, 12);
    OLED_Update();

    LED_On();

    /* while(1) 中每隔 1s 改变一次屏幕显示内容和 LED 状态 */
    while (1)
    {
        /* 延时 1 秒 */
        Delay_Ms(1000);

        counter++;

        /* 翻转板载 LED 状态 */
        LED_Toggle();

        /* 同步改变屏幕上的显示内容 */
        if (LED_GetState())
        {
            OLED_ShowString(0, 36, "HelloWorld: LED ON ", 12);
        }
        else
        {
            OLED_ShowString(0, 36, "HelloWorld: LED OFF", 12);
        }

        OLED_ShowNum(78, 50, counter, 4, 12);

        /* 刷新显示到屏幕 */
        OLED_Update();
    }

    return 0;
}
