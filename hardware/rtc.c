#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "rtc.h"

void RTC_Interrupt(uint32_t events) {
    printf("ALARM!!\n");
    RTC_ResetInterrupts();
}

void RTC_Init(void) {

    // todo: Sync I2C interface by toggling SCL until SDA reads high

    // Write control register 0xEh
    uint8_t control_reg = 
        (0b0 << 7) |        // nEOSC - oscillator enabled
        (0b0 << 6) |        // BBSQW - no battery backed square wave
        (0b0 << 5) |        // CONV - force temperature conversion
        (0b00 << 3) |       // RSx - 1 Hz square wave output
        (0b1 << 2) |        // INTCN - enable interrupt on nINT/SQW pin
        (0b0 << 1) |        // A2IE - disable alarm 2 interrupt
        (0b1 << 0);         // A1IE - enable alarm 1 interrupt
    RTC_WriteRegister(0x0e, control_reg);

    // Write status register
    uint8_t status_reg = 0;
    RTC_WriteRegister(0x0f, status_reg);

    // Set alarm to 00/00/0000 - 00:00:00
    // Also set flags to only copare seconds in order to fire once every minute
    uint8_t alarm_regs[5];
    alarm_regs[0] = 0x7;
    alarm_regs[1] = (0 << 7);
    alarm_regs[2] = (1 << 7);
    alarm_regs[3] = (1 << 7);
    alarm_regs[4] = (1 << 7);    
    i2c_write_blocking(RTC_I2C, 0b1101000, alarm_regs, 5, false);
}

uint8_t RTC_ReadRegister(uint8_t addr) {
    uint8_t reg;
    i2c_write_blocking(RTC_I2C, 0b1101000, &addr, 1, false);
    i2c_read_blocking(RTC_I2C, 0b1101000, &reg, 1, false);
    return reg;
}

void RTC_WriteRegister(uint8_t addr, uint8_t value) {
    uint8_t packet[] = {addr, value};
    i2c_write_blocking(RTC_I2C, 0b1101000, packet, 2, false);
}

void RTC_EnableOscillator(bool enabled) {
    uint8_t control_reg = 0;        // todo: read register from RTC
    if (enabled) control_reg &= ~(1 << 7);
    else control_reg |= (1 << 7);
    // todo: write register back to RTC
}

void RTC_ResetInterrupts(void) {
    uint8_t status_reg = 0;
    RTC_WriteRegister(0x0f, status_reg);
}

void RTC_GetTimeStruct(struct tm *time) {
    uint8_t time_regs[7];
    uint8_t start_addr = 0;
    i2c_write_blocking(RTC_I2C, 0b1101000, &start_addr, 1, false);
    i2c_read_blocking(RTC_I2C, 0b1101000, time_regs, 7, false);

    time->tm_sec = (time_regs[0] & 0b1111) + 10 * (time_regs[0] >> 4);
    time->tm_min = (time_regs[1] & 0b1111) + 10 * (time_regs[1] >> 4);
    time->tm_hour = (time_regs[2] & 0b1111) + 10 * ((time_regs[2] >> 4) & 0b11);

    time->tm_mday = (time_regs[4] & 0b1111) + 10 * (time_regs[4] >> 4);
    time->tm_mon = (time_regs[5] & 0b1111) + 10 * (time_regs[5] >> 4) - 1;          // struct tm definition: month since January (0 - 11)
    time->tm_year = (time_regs[6] & 0b1111) + 10 * (time_regs[6] >> 4) + 100;       // struct tm definition: years since 1900
}

void RTC_SetTimeStruct(struct tm *time) {
    uint8_t time_regs[8];

    time_regs[0] = 0;
    time_regs[1] = (time->tm_sec % 10) | ((time->tm_sec / 10) << 4);
    time_regs[2] = (time->tm_min % 10) | ((time->tm_min / 10) << 4);
    time_regs[3] = (time->tm_hour % 10) | ((time->tm_hour / 10) << 4);
    time_regs[4] = 0;
    time_regs[5] = (time->tm_mday % 10) | ((time->tm_mday / 10) << 4);
    time_regs[6] = ((time->tm_mon + 1) % 10) | (((time->tm_mon + 1) / 10) << 4);
    time_regs[7] = ((time->tm_year - 100) % 10) | (((time->tm_year - 100) / 10) << 4);
    
    i2c_write_blocking(RTC_I2C, 0b1101000, time_regs, 8, false);
}