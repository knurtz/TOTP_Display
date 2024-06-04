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

#ifndef _BOARDS_TOTPDISPLAY_H
#define _BOARDS_TOTPDISPLAY_H

#define kHz * 1000
#define MHz * 1000 * 1000

// For board detection
#define TOTP_DISPLAY

// --- UART ---
#define PICO_STDIO_DEFAULT_CRLF 0

// PICO_DEFAULT_LED_PIN ---
#define PICO_DEFAULT_LED_PIN 17

// Display
#define DISP_SPI        spi1
#define DISP_CLK        14      // Function F1: SPI1 SCK
#define DISP_DIN        15      // Function F1: SPI1 TX

#define DISP_BL         9       // GPIO
#define DISP_RST        12      // GPIO
#define DISP_DC         16      // GPIO
#define DISP_CS	        13      // GPIO

// RTC
#define RTC_I2C         i2c0
#define RTC_INT         11
#define RTC_SCL         1
#define RTC_SDA         0

// DCF
#define DCF_PON         28
#define DCF_TCO         3

// --- FLASH ---

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (16 * 1024 * 1024)     // reserve 1M for actual program, rest of 16M for mass storage
#endif

#ifndef PICO_RP2040_B2_SUPPORTED
#define PICO_RP2040_B2_SUPPORTED 1
#endif

#endif  // _BOARDS_TOTPDISPLAY_H
