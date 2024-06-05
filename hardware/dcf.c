#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pico/stdlib.h"

#include "dcf.h"

uint32_t last_falling_edge = 0;

uint32_t current_telegram[2];
uint8_t bit_counter = 0;

uint32_t last_telegram[2];
bool telegram_ready = false;

bool DCF_DecodeTelegram(void) {
    if (!telegram_ready) return false;

    printf("Last telegram: 0x%x%x\n", last_telegram[1], last_telegram[0]);
    uint8_t minute = ((last_telegram[0] >> 21) & 0b1111) + 10 * ((last_telegram[0] >> 25) & 0b111);
    uint8_t hour = ((last_telegram[0] >> 29) & 0b111) + 8 * (last_telegram[1] & 0b1) + 10 * ((last_telegram[1] >> 1) & 0b11);
    uint8_t mday = ((last_telegram[1] >> 4) & 0b1111) + 10 * ((last_telegram[1] >> 8) & 0b11);
    uint8_t mon = ((last_telegram[1] >> 13) & 0b1111) + 10 * ((last_telegram[1] >> 17) & 0b1);
    uint8_t year = ((last_telegram[1] >> 18) & 0b1111) + 10 * ((last_telegram[1] >> 22) & 0b1111);
    printf("%02d.%02d.%d - %02d:%02d:00\n", mday, mon, year, hour, minute);

    telegram_ready = false;
    return true;
}

static void DCF_ResetTelegram(uint time_delta) {
    if (time_delta > 1500000) {
        if (bit_counter == 59) {
            last_telegram[0] = current_telegram[0];
            last_telegram[1] = current_telegram[1];
            telegram_ready = true;
        }
        else {
            printf("Invalid minute, bitcounter: %d, time_delta: %d ms\n", bit_counter, time_delta / 1000);
        }
    }
    else {
        printf("Invalid reset, time_delta: %d ms\n", time_delta / 1000);
    }
    bit_counter = 0;
}

static inline void DCF_AddBit(bool bit) {
    current_telegram[bit_counter / 32] |= (bit << (bit_counter % 32));
    bit_counter++;
    if (bit_counter >= 59) bit_counter = 59;
}

void DCF_Interrupt(uint32_t events) {
    gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(DCF_TCO));
    uint32_t time_delta = time_us_32() - last_falling_edge;
    if (time_delta < 50000) return;     // glitch

    // Falling edge (signal is inverted)
    //if (gpio_get(DCF_TCO)) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        // Last falling edge 2 seconds ago -> minute reset
        if (time_delta > 1500000) {
            DCF_ResetTelegram(time_delta);
        }
        last_falling_edge = time_us_32();
    }

    // Rising edge
    else {
        // 250 ms - infinty: invalid
        if (time_delta > 250000) {
            DCF_ResetTelegram(time_delta);
        }
        // 150 - 250 ms: 200 ms = 1
        else if (time_delta > 150000) {
            DCF_AddBit(1);
        }
        // 50 - 150 ms: 100 ms = 0
        else {
            DCF_AddBit(0);
        }
    }
}

void DCF_Init(void) {

}