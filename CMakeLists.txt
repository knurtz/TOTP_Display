cmake_minimum_required(VERSION 3.12)

# Import Pico SDK
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
include(pico_extras_import.cmake)

# Set custom board file for TinySound v2
set(PICO_BOARD_HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/board")
set(PICO_BOARD totp_display)

project(TOTP_Display C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

set(PICO_UART_ENABLE_CRLF_SUPPORT 0)

# Add all source code files
add_executable(TOTP_Display
    # Main
    main.c

    # Hardware
    hardware/hardware.c
    hardware/lcd.c
    hardware/rtc.c

    # Shell
    system/shell.c
    system/shell_commands.c
    system/totp.c
    system/TOTP-MCU/TOTP.c
    system/TOTP-MCU/sha1.c

    # UI
    system/ui/bmp.c
    system/ui/font.c

    # Mass storage    
    usb/msc_disk.c
    usb/flash_functions.c
    
    # FatFS
    system/fat.c
    fatfs/ff.c
    fatfs/ffunicode.c
    fatfs/diskio.c

    # USB
    usb/usb_descriptors.c
    usb/usb_callbacks.c
)

target_compile_definitions(TOTP_Display PRIVATE
    
)

target_include_directories(TOTP_Display PRIVATE
    .
    hardware
    system
    usb
    fatfs
)

pico_set_linker_script(TOTP_Display ${CMAKE_CURRENT_SOURCE_DIR}/custom_linker_script.ld)

pico_set_program_name(TOTP_Display "TOTP_Display")
pico_set_program_version(TOTP_Display "0.1")

# Import pico_stdlib which imports commonly used features
target_link_libraries(TOTP_Display 
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_i2c
    hardware_timer
    hardware_dma
    hardware_flash
    hardware_sync
    tinyusb_device
)

pico_enable_stdio_uart(TOTP_Display 0)
pico_enable_stdio_usb(TOTP_Display 1)

target_compile_definitions(TOTP_Display PRIVATE
    # bootloader activity LED
    PICO_BOOTSEL_VIA_DOUBLE_RESET_ACTIVITY_LED=17
)

# Create map/bin/hex file etc.
pico_add_extra_outputs(TOTP_Display)

# Enable a summary of used memory at the end of linking process
target_link_options(TOTP_Display PUBLIC "-Wl,--print-memory-usage,--no-warn-rwx-segments")
add_custom_command(TARGET TOTP_Display
    POST_BUILD
    COMMENT "Code size:"
    COMMAND arm-none-eabi-size TOTP_Display.elf
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/modify_uf2.py ${CMAKE_CURRENT_SOURCE_DIR}/build/TOTP_Display.uf2 0x10100000
    VERBATIM
)