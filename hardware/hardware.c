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

    gpio_init(DISP_DIN);
    gpio_set_function(DISP_DIN, GPIO_FUNC_SPI);
    gpio_init(DISP_CLK);
    gpio_set_function(DISP_CLK, GPIO_FUNC_SPI);

    spi_init(DISP_SPI, 60 MHz);


    // Init RTC pins and I2C
    gpio_init(RTC_SCL);
    gpio_set_function(RTC_SCL, GPIO_FUNC_I2C);
    gpio_init(RTC_SDA);
    gpio_set_function(RTC_SDA, GPIO_FUNC_I2C);

    gpio_init(RTC_INT);
    gpio_set_dir(RTC_INT, false);
    gpio_pull_up(RTC_INT);

    i2c_init(RTC_I2C, 100 kHz);
}