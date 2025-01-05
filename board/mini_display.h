/* 
 * TOTP Display board file for Pico SDK
 *
 * Based on pico.h by Raspberry Pi (Trading) Ltd.
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------

// pico_cmake_set PICO_PLATFORM=rp2350

#ifndef _BOARDS_MINIDISPLAY_H
#define _BOARDS_MINIDISPLAY_H

#define kHz * 1000
#define MHz * 1000 * 1000

// For board detection
#define MINI_DISPLAY

// --- RP2350 VARIANT ---
#define PICO_RP2350A 1

// --- UART ---
#define PICO_STDIO_DEFAULT_CRLF 0

// PICO_DEFAULT_LED_PIN ---
#define PICO_DEFAULT_LED_PIN 25

// Display
#define DISP_SPI        spi1
#define DISP_CLK        10      // Function F1: SPI1 SCK
#define DISP_DIN        11      // Function F1: SPI1 TX

#define DISP_BL         6       // GPIO
#define DISP_RST        12      // GPIO
#define DISP_DC         8       // GPIO
#define DISP_CS	        9       // GPIO

// --- FLASH ---

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (128 * 1024)     // reserve 128 kB for actual program, rest for mass storage
#endif

// Drive high to force power supply into PWM mode (lower ripple on 3V3 at light loads)
#define PICO_SMPS_MODE_PIN 23

// The GPIO Pin used to read VBUS to determine if the device is battery powered.
#ifndef PICO_VBUS_PIN
#define PICO_VBUS_PIN 24
#endif

// The GPIO Pin used to monitor VSYS. Typically you would use this with ADC.
// There is an example in adc/read_vsys in pico-examples.
#ifndef PICO_VSYS_PIN
#define PICO_VSYS_PIN 29
#endif

#ifndef PICO_RP2350_A2_SUPPORTED
#define PICO_RP2350_A2_SUPPORTED 1
#endif

#endif  // _BOARDS_MINIDISPLAY_H
