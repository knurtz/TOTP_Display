#include <stdio.h>
#include "string.h"

#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "tusb.h"
#include "flash_functions.h"
#include "fat.h"

#include "hardware.h"
#include "shell.h"

#include "lcd.h"
#include "ui/bmp.h"
#include "ui/font.h"

#include "rtc.h"
#include "dcf.h"

uint32_t next_blink_ms = 0;
uint16_t blink_interval_ms = 500;
uint16_t blink_interval_fast_ms = 100;

uint16_t colors[] = {RGB(255, 0, 0), RGB(255, 255, 0), RGB(0, 255, 0)};
uint8_t color_index = 0;

static void blink(void) {
    if (time_us_32() / 1000 > next_blink_ms) {
        next_blink_ms += blink_interval_ms;
        gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
    }
}

int main() {
    stdio_init_all();
    tusb_init();

    Hardware_Init();
    Shell_Init();

    DCF_Init();
    RTC_Init();

    Flash_Init();
    FAT_Init();

    //Font_Init();
    LCD_Init(HORIZONTAL);
    
    while (1) {
        tud_task();
        Shell_CheckCommand();
        Flash_WriteCycle(false);
        DCF_DecodeTelegram();

        //blink();
    }

    return 1;
}

void _close(void) {}
void _lseek(void) {}