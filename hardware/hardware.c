#include "hardware.h"

#include "pico/binary_info.h"

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"

void Hardware_Init(void)
{    
    bi_decl(bi_program_description("TOTP Display with animation"));    
    bi_decl(bi_2pins_with_names(DISP_CLK, "Display clock", DISP_DIN, "Display data out"));
    bi_decl(bi_4pins_with_names(DISP_CS, "Display chip select", DISP_DC, "Display data/command", DISP_RST, "Display reset", DISP_BL, "Display backlight"));
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "On-board LED"));


    // Init onboard LED pin
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);


    // Init LCD pins and SPI
    gpio_init(DISP_CS);
    gpio_set_dir(DISP_CS, GPIO_OUT);

    gpio_init(DISP_DC);
    gpio_set_dir(DISP_DC, GPIO_OUT);

    gpio_init(DISP_RST);
    gpio_set_dir(DISP_RST, GPIO_OUT);

    gpio_init(DISP_BL);
    gpio_set_dir(DISP_BL, GPIO_OUT);

    gpio_set_function(DISP_DIN, GPIO_FUNC_SPI);
    gpio_set_function(DISP_CLK, GPIO_FUNC_SPI);

    spi_init(DISP_SPI, 60 MHz);
}