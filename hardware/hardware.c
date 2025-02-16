#include "hardware.h"

#include "hardware/gpio.h"
#include "hardware/spi.h"

void Hardware_Init(void) { 
    // Init onboard LED pin
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // Init LCD pins and SPI
    /*
    gpio_init(DISP_CS);
    gpio_set_dir(DISP_CS, GPIO_OUT);

    gpio_init(DISP_DC);
    gpio_set_dir(DISP_DC, GPIO_OUT);

    gpio_init(DISP_RST);
    gpio_set_dir(DISP_RST, GPIO_OUT);

    gpio_init(DISP_BL);
    gpio_set_dir(DISP_BL, GPIO_OUT);

    gpio_init(DISP_DIN);
    gpio_set_function(DISP_DIN, GPIO_FUNC_SPI);
    gpio_init(DISP_CLK);
    gpio_set_function(DISP_CLK, GPIO_FUNC_SPI);

    spi_init(DISP_SPI, 10 MHz);
    */
}