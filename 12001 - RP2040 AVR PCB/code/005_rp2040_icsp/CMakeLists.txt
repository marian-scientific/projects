cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(my_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(rp2040_icsp rp2040_icsp.c)
pico_generate_pio_header(rp2040_icsp ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
pico_add_extra_outputs(rp2040_icsp)
target_link_libraries(rp2040_icsp pico_stdlib hardware_pio pico_platform hardware_spi)

# Enable USB serial output
pico_enable_stdio_usb(rp2040_icsp 1)
pico_enable_stdio_uart(rp2040_icsp 0)
