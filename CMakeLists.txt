# Simple CAN logger with Dual Canbus FD Board
#
# Runs on the Raspberry Pi Pico connected to a Dual Canbus FD Board


# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)
#set(PICO_BOARD pico)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(DualCan-FD-Tester_V1.0 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DMCP25xxFD)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    main.c
    app.c
    led.c
	drv_canfdspi_api.c
	drv_spi.c
    pcf85063.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_i2c
)


if (${PICO_BOARD} MATCHES "pico_w") 
    target_link_libraries(${PROJECT_NAME} 
        pico_cyw43_arch_none
)
endif()


pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
