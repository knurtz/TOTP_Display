#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"

#include "totp.h"
#include "rtc.h"
#include "fat.h"
#include "TOTP-MCU/TOTP.h"

uint8_t hmac_key[20] = {0x82, 0x45, 0x4b, 0x5b, 0x67, 0xf4, 0x56, 0x59, 0xad, 0xf2, 0x7b, 0xe8, 0x12, 0xee, 0x54, 0x5a, 0x3b, 0x02, 0xb0, 0x23};
uint8_t key_length = 20;

bool TOTP_ReadKeyFromFile(char* filename) {
    char file_buffer[100];
    uint8_t read_bytes = FAT_ReadFileToBuffer(filename, 0, sizeof(file_buffer), file_buffer);
    if (read_bytes == 0) return false;

    char* endstr = file_buffer;
    for (key_length = 0; key_length < sizeof(hmac_key) && *endstr; key_length++) {
        hmac_key[key_length] = strtol(endstr, &endstr, 16);
    }

    printf("Read %d bytes from file into key.\n", key_length);
    for (int i = 0; i < key_length; i++) printf("%02x ", hmac_key[i]);
    printf("\n");
}

void TOTP_Init(void) {
    TOTP_ReadKeyFromFile("/Keys/hmac.txt");

    TOTP(hmac_key, key_length, 30);
    setTimezone(2);

    
}

uint32_t TOTP_GetToken(uint timestamp) {
    return getCodeFromTimestamp(timestamp);
}

uint32_t TOTP_GetCurrentToken(void) {   
    struct tm current_time;
    // todo: get from RTC
    // RTC_GetTimeStruct(&current_time);
    return getCodeFromTimeStruct(current_time);
}