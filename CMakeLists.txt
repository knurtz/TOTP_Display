cmake_minimum_required(VERSION 3.12)

# Set custom board file
set(PICO_BOARD_HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/board")
set(PICO_BOARD mini_display)

# Import Pico SDK
include(pico_sdk_import.cmake)

project(MiniDisplay C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

set(PICO_UART_ENABLE_CRLF_SUPPORT 0)

# Add all source code files
add_executable(MiniDisplay
    # Main
    main.c

    # Hardware
    hardware/hardware.c
    hardware/lcd.c

    # Shell
    system/shell.c
    system/shell_commands.c

    # UI
    system/ui/bmp.c
    system/ui/font.c
)

target_compile_definitions(MiniDisplay PRIVATE
    # bootloader activity LED
    PICO_BOOTSEL_VIA_DOUBLE_RESET_ACTIVITY_LED=25
)

target_include_directories(MiniDisplay PRIVATE
    .
    hardware
    system
)

# Enable Pico MSC mass storage module
include(pico_msc/pico_msc.cmake)
enable_pico_msc(MiniDisplay)

# Import pico_stdlib which imports commonly used features
target_link_libraries(MiniDisplay 
    pico_stdlib
    hardware_spi
    hardware_timer
    hardware_dma
    hardware_flash
    hardware_sync
    tinyusb_device
    pico_msc
)

pico_enable_stdio_usb(MiniDisplay 1)
pico_enable_stdio_uart(MiniDisplay 0)

pico_set_program_name(MiniDisplay "MiniDisplay")
pico_set_program_version(MiniDisplay "0.1")

# Create map/bin/hex file etc.
pico_add_extra_outputs(MiniDisplay)

# Enable a summary of used memory at the end of linking process
target_link_options(MiniDisplay PUBLIC "-Wl,--print-memory-usage,--no-warn-rwx-segments")
add_custom_command(TARGET MiniDisplay
    POST_BUILD
    COMMENT "Code size:"
    COMMAND arm-none-eabi-size MiniDisplay.elf
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/modify_uf2.py ${CMAKE_CURRENT_SOURCE_DIR}/build/MiniDisplay.uf2 0x10020000
    VERBATIM
)