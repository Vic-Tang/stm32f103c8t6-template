# STM32F103C8T6 VS Code + CMake + Ninja 嵌入式开发工程模板

轻量化、模块化的 STM32F103C8T6 裸机/微控制器开发框架模版。基于 CMake + Ninja 构建系统，深度集成 VS Code 任务与一键烧录调试支持（ST-Link / J-Link）。

---

## 一、工程目录架构

```text
├── App/                         # 应用层逻辑
│   ├── inc/
│   │   └── main.h
│   └── src/
│       └── main.cpp             # 主业务逻辑入口
├── MCAL/                        # 微控制器抽象层 (驱动与硬件支持)
│   └── stm32f103/
│       ├── inc/
│       │   ├── stm32f103xb.h    # 寄存器与外设基地址定义
│       │   ├── mcal_rcc.h       # 72MHz 时钟树驱动
│       │   ├── mcal_gpio.h      # GPIO 驱动 (CRL/CRH配置/高低电平翻转)
│       │   ├── mcal_systick.h   # SysTick 1ms 硬件定时与微秒/毫秒精准延时
│       │   └── Hardware_Init.h  # 底层统一初始化头文件
│       └── src/
│           ├── mcal_rcc.c
│           ├── mcal_gpio.c
│           ├── mcal_systick.c
│           ├── Hardware_Init.c
│           └── startup_stm32f103xb.s # GCC 启动向量表文件
├── cmake/
│   └── arm_none_eabi_gcc.cmake  # 交叉编译工具链配置
├── linker/
│   └── STM32F103C8Tx_FLASH.ld   # 链接脚本 (64KB Flash, 20KB RAM)
├── utils/
│   └── flash.jlink              # J-Link 自动化烧录脚本
├── .vscode/                     # VS Code 开发环境配置文件
│   ├── c_cpp_properties.json    # IntelliSense 配置
│   ├── launch.json              # cortex-debug 调试配置
│   ├── settings.json            # 终端与状态栏按钮配置
│   └── tasks.json               # 编译、清除与烧录任务
├── CMakeLists.txt               # CMake 构建描述文件
└── .gitignore                   # Git 忽略配置
```

---

## 二、开发环境要求与安装配置

在全新电脑或新环境中拉取本模板后，需确保具备以下工具链与扩展：

### 1. 必备工具链 (编译器与构建工具)

推荐使用 **MSYS2 (MINGW64)** 或直接安装独立 Windows 二进制包：

| 工具 | 推荐版本 | 说明 | 验证命令 |
| :--- | :--- | :--- | :--- |
| **GNU Arm Embedded Toolchain** | 10.3 / 12.2+ | 提供 `arm-none-eabi-gcc`、`arm-none-eabi-g++`、`arm-none-eabi-objcopy` 等 | `arm-none-eabi-gcc --version` |
| **CMake** | 3.20+ | 跨平台项目构建生成器 | `cmake --version` |
| **Ninja** | 1.10+ | 高性能构建系统（比 make 更快） | `ninja --version` |

> **MSYS2 快速安装命令（MINGW64 环境下）：**
> ```bash
> pacman -S mingw-w64-x86_64-arm-none-eabi-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-openocd mingw-w64-x86_64-gdb-multiarch
> ```

### 2. 烧录与调试工具（根据拥有的调试器二选一或兼备）

* **J-Link 用户**：
  * 安装 [SEGGER J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/)。
  * 默认安装路径为 `C:\Program Files\SEGGER\JLink`。
  * 若安装路径不同，请在 [.vscode/tasks.json](.vscode/tasks.json) 和 [.vscode/launch.json](.vscode/launch.json) 中更新 `JLink.exe` / `JLinkGDBServerCL.exe` 路径。
* **ST-Link 用户**：
  * 安装 **OpenOCD**（MSYS2 自带或通过 [xPack OpenOCD](https://github.com/xpack-dev-tools/openocd-xpack) 安装）。
  * 确保终端中能够直接调用 `openocd`。

### 3. VS Code 插件列表

在 VS Code 插件商店中搜索并安装：
* **C/C++** (`ms-vscode.cpptools`)：代码高亮、智能补全与跳转。
* **Cortex-Debug** (`marus25.cortex-debug`)：ARM Cortex-M 芯片调试工具。
* **VS Code Task Buttons** (`spencerwmiles.vscode-task-buttons`)：在底部状态栏生成编译、清除、烧录便捷按钮。

---

## 三、快速开始与日常使用

1. **配置与编译**：
   * 点击 VS Code 底部状态栏的 **🔨 Build** 按钮，或在终端执行：
     ```bash
     cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm_none_eabi_gcc.cmake
     ninja -C build
     ```
   * 编译成功后将在 `build/` 目录下生成 `.elf`、`.hex`、`.bin` 固件。

2. **清除构建**：
   * 点击状态栏的 **🧹 Clean** 按钮，或运行 `rm -rf build`。

3. **固件烧录**：
   * 使用 **J-Link**：点击状态栏 **⚡ Flash (J-Link)**。
   * 使用 **ST-Link**：点击状态栏 **⚡ Flash (OpenOCD)**。

4. **硬件调试 (Debug)**：
   * 按键盘 **`F5`** 或打开 VS Code “运行和调试” 界面（`Ctrl+Shift+D`）。
   * 选择 `Debug (J-Link)` 或 `Debug (OpenOCD / ST-Link)`，即可打断点、单步调试、查看寄存器和变量。

---

## 四、功能模块使用示例

* **时钟与滴答定时**：MCU 启动后由 `Hardware_EarlyInit()` 自动将系统主频提升至 72MHz，并使能 SysTick 1ms 中断。
* **精准毫秒延时**：
  ```c
  Delay_Ms(1000); // 精确延时 1000 毫秒
  ```
* **GPIO 控制**：
  ```c
  RCC_EnableAPB2Periph(RCC_APB2ENR_IOPCEN);
  GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_2MHZ);
  GPIO_ResetPin(GPIOC, GPIO_PIN_13); // 输出低电平
  GPIO_SetPin(GPIOC, GPIO_PIN_13);   // 输出高电平
  GPIO_TogglePin(GPIOC, GPIO_PIN_13);// 电平翻转
  ```
