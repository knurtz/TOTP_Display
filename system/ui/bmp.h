#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include "pico/stdlib.h"

#define LITTLE_ENDIAN_16BIT(addr)   *(addr) | *(addr + 1) << 8
#define LITTLE_ENDIAN_32BIT(addr)   *(addr) | *(addr + 1) << 8 | *(addr + 2) << 16 | *(addr + 3) << 24

typedef struct bmp_info {
    uint16_t height;
    uint16_t width;
    uint32_t data_offset;
    uint8_t bitcount;
    bool compressed;
    bool indexed;
    uint16_t colortable_size;
} BMP_Info;

// Retrieve info section from BMP file
uint8_t BMP_GetInfo(const char* filename, BMP_Info* info, uint8_t* file_buffer);

// Print BMP info
void BMP_PrintInfo(BMP_Info* info);

// Render a BMP image to provided buffer. Aspect ratio (width, height) of buffer needed for correct wrapping.
bool BMP_RenderToBuffer(const char* filename, uint16_t image_x, uint16_t image_y, uint16_t buffer_width, uint16_t buffer_height, uint16_t buffer[]);

// Render BMP to special buffer reserved for LCD. Optionally transfer to display immediately after rendering.
bool BMP_RenderToDisplay(const char* filename, uint16_t image_x, uint16_t image_y, bool send_immediately);

#endif /* BMP_H */
