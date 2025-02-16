#include <stdio.h>
#include "string.h"

#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "pico_msc/pico_msc.h"

#include "hardware.h"
#include "shell.h"

//#include "lcd.h"
//#include "ui/bmp.h"
//#include "ui/font.h"

uint32_t last_blink = 0;
uint16_t blink_interval_ms = 500;

//uint16_t colors[] = {RGB(255, 0, 0), RGB(255, 255, 0), RGB(0, 255, 0)};
//uint8_t color_index = 0;

static void blink(void) {
    if (time_us_32() - last_blink > blink_interval_ms) {
        last_blink = time_us_32();
        gpio_xor_mask (1 << PICO_DEFAULT_LED_PIN);
    }
}

int main() {
    stdio_init_all();
    tusb_init();            // todo: replace by PicoMSC_Init();

    Hardware_Init();
    Shell_Init();

    Flash_Init();
    FAT_Init();

    //Font_Init();
    //LCD_Init(HORIZONTAL);
    
    while (1) 
    {
        blink();
        
        tud_task();
        Shell_CheckCommand();
        Flash_WriteCycle(false);
    }

    return 1;
}

void _close(void) {}
void _lseek(void) {}