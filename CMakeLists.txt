# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
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

target_compile_definitions(MiniDisplay PRIVATE
    
)

target_include_directories(MiniDisplay PRIVATE
    .
    hardware
    system
    usb
    fatfs
)

pico_set_linker_script(MiniDisplay ${CMAKE_CURRENT_SOURCE_DIR}/custom_linker_script.ld)

pico_set_program_name(MiniDisplay "MiniDisplay")
pico_set_program_version(MiniDisplay "0.1")

# Import pico_stdlib which imports commonly used features
target_link_libraries(MiniDisplay 
    pico_stdlib
    hardware_uart
    hardware_spi
    hardware_timer
    hardware_dma
    hardware_flash
    hardware_sync
    tinyusb_device
)

pico_enable_stdio_uart(MiniDisplay 0)
pico_enable_stdio_usb(MiniDisplay 1)

target_compile_definitions(MiniDisplay PRIVATE
    # bootloader activity LED
    PICO_BOOTSEL_VIA_DOUBLE_RESET_ACTIVITY_LED=25
)

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