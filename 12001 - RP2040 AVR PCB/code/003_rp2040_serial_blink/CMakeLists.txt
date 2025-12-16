cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(my_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(serial_blink serial_blink.c)
pico_add_extra_outputs(serial_blink)
target_link_libraries(serial_blink pico_stdlib)

# Enable USB serial output
pico_enable_stdio_usb(serial_blink 1)
pico_enable_stdio_uart(serial_blink 0)
