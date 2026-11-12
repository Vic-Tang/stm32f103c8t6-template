cmake_minimum_required(VERSION 3.20)

project(stm32f103c8t6_firmware C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Architecture and MCU Specific definitions for STM32F103C8T6 (Cortex-M3, 64KB Flash, 20KB RAM)
set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb")

# Compiler and Linker flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall -Wextra -fdata-sections -ffunction-sections -O2" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -Wall -Wextra -fdata-sections -ffunction-sections -fno-exceptions -fno-rtti -O2" CACHE INTERNAL "CXX compiler flags")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "ASM compiler flags")

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F103C8Tx_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -T${LINKER_SCRIPT} -Wl,--gc-sections -Wl,--no-warn-rwx-segments -specs=nano.specs -specs=nosys.specs -Wl,-Map=${PROJECT_NAME}.map" CACHE INTERNAL "Linker flags")

# Target Definitions
add_compile_definitions(STM32F103xB)

# Include Directories
include_directories(
    App/inc
    BSP/inc
    MCAL/stm32f103/inc
)

# Source files
file(GLOB_RECURSE APP_SOURCES "App/src/*.c" "App/src/*.cpp")
file(GLOB_RECURSE BSP_SOURCES "BSP/src/*.c" "BSP/src/*.cpp")
file(GLOB_RECURSE MCAL_SOURCES "MCAL/stm32f103/src/*.c" "MCAL/stm32f103/src/*.cpp" "MCAL/stm32f103/src/*.s")

set(ALL_SOURCES ${APP_SOURCES} ${BSP_SOURCES} ${MCAL_SOURCES})

# Target Executable
add_executable(${PROJECT_NAME}.elf ${ALL_SOURCES})

# Post-build actions: generate .hex, .bin and display code size
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf
    COMMENT "Building .hex and .bin, showing memory usage:"
)
