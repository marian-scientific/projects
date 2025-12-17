cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(my_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(serial_rgb_blink serial_rgb_blink.c)
pico_generate_pio_header(serial_rgb_blink ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
pico_add_extra_outputs(serial_rgb_blink)
target_link_libraries(serial_rgb_blink pico_stdlib hardware_pio pico_platform)

# Enable USB serial output
pico_enable_stdio_usb(serial_rgb_blink 1)
pico_enable_stdio_uart(serial_rgb_blink 0)
