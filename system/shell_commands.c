#include "shell_commands.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hardware/clocks.h"
#include "pico/bootrom.h"

#include "hardware.h"
#include "fat.h"
#include "totp.h"
#include "rtc.h"

// Numbers extracted from argument string
uint arg[6];

// Extracts numbers from argument string. Returns amount of extracted numbers.
static size_t ExtractParameters(char* argv) {
    size_t i;
    char* endstr = argv;
    for (i = 0; i < count_of(arg) && *endstr; i++) {
        arg[i] = strtol(endstr, &endstr, 0);
    }
    return i;
}

// Echo command
static void echo_cb(int arglen, char* argv) {
    printf("%s\n", argv);
}

// Clocks command
static void clocks_cb(int arglen, char* argv) {
    printf("System clock: %d kHz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
}

// Test command
static void test_cb(int arglen, char* argv) {
    printf("arglen: %d, argv: %s\n", arglen, argv);
}

// UF2 command
static void uf2_cb(int arglen, char* argv) {
    reset_usb_boot(0, 2);
}

void listfolder_cb(int arglen, char* argv) {
    FAT_ListFolder("/", 0);
}

void totp_cb(int arglen, char* argv) {
    ExtractParameters(argv);
    printf("Your TOTP for timestamp %d: %d\n", arg[0], TOTP_GetToken(arg[0]));   
}

void key_cb(int arglen, char* argv) {
    TOTP_ReadKeyFromFile(argv);
}

void rtc_read_cb(int arglen, char* argv) {
    ExtractParameters(argv);
    printf("RTC register 0x%02x: %02x", arg[0], RTC_ReadRegister(arg[0]));
}

void rtc_time_cb(int arglen, char* argv) {
    struct tm time;
    RTC_GetTimeStruct(&time);
    printf("Current date: %02d.%02d.%4d\nCurrent time: %02d:%02d:%02d\n", time.tm_mday, time.tm_mon + 1, time.tm_year + 1900, time.tm_hour, time.tm_min, time.tm_sec);
}

void rtc_set_cb(int arglen, char* argv) {
    ExtractParameters(argv);
    struct tm time = {
        .tm_mday = arg[0],
        .tm_mon = arg[1] - 1,
        .tm_year = arg[2] - 1900,
        .tm_hour = arg[3],
        .tm_min = arg[4],
        .tm_sec = arg[5]
    };
    RTC_SetTimeStruct(&time);
}

// Assemble complete command set into a single array
ShellCommand command_set[] = {
    {"echo", echo_cb},
    {"clocks", clocks_cb},
    {"test", test_cb},
    {"uf2", uf2_cb},
    {"ls", listfolder_cb},
    {"totp token", totp_cb},
    {"totp key", key_cb},
    {"rtc read", rtc_read_cb},
    {"rtc time", rtc_time_cb},
    {"rtc set", rtc_set_cb}
};
size_t command_cnt = count_of(command_set);