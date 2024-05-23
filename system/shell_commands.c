#include "shell_commands.h"

#include <stdlib.h>
#include <string.h>

#include "hardware/clocks.h"
#include "pico/bootrom.h"

#include "hardware.h"
#include "ff.h"

// Numbers extracted from argument string
uint arg[5];

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

static void listfolder(const char* dir_name, uint8_t current_depth) {
    DIR dir;
    FILINFO fileinfo;

    f_opendir(&dir, dir_name);
    f_readdir(&dir, &fileinfo);

    while (fileinfo.fname[0]) {
        printf("%*s%s\n", current_depth * 2, "", fileinfo.fname);
        if (fileinfo.fattrib & AM_DIR) {
            size_t next_dir_length = strlen(dir_name) + strlen(fileinfo.altname) + 3;
            char next_dir[next_dir_length];
            snprintf(next_dir, next_dir_length - 1, "%s/%s", dir_name, fileinfo.altname);
            listfolder(next_dir, current_depth + 1);
        }
        f_readdir(&dir, &fileinfo);
    }
}

void listfolder_cb(int arglen, char* argv) {
    listfolder("/", 0);
}


// Assemble complete command set into a single array
ShellCommand command_set[] = {
    {"echo", echo_cb},
    {"clocks", clocks_cb},
    {"test", test_cb},
    {"uf2", uf2_cb},
    {"ls", listfolder_cb},

};
size_t command_cnt = count_of(command_set);