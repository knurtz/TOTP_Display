#include "pico/stdlib.h"
#include "string.h"

#include "bmp.h"
#include "font.h"

#include "hardware/lcd.h"

uint16_t sono_30p_buffer[SONO_30P_WIDTH * SONO_30P_HEIGHT];
uint16_t sono_50p_buffer[SONO_50P_WIDTH * SONO_50P_HEIGHT];


static void Font_Load(char* filename, uint16_t font_buffer[], uint16_t buffer_width, uint16_t buffer_height)
{
    BMP_RenderToBuffer(filename, 0, 0, buffer_width, buffer_height, font_buffer);
}

void Font_Init(void)
{
    Font_Load("Fonts/sono_30p.bmp", sono_30p_buffer, SONO_30P_WIDTH, SONO_30P_HEIGHT);
    Font_Load("Fonts/sono_50p.bmp", sono_50p_buffer, SONO_50P_WIDTH, SONO_50P_HEIGHT);
}

void Font_Put(enum fonts font, uint8_t char_offset, uint16_t x, uint16_t y)
{
    if (font == SONO_30P)
    {
        if (char_offset >= SONO_30P_NUM_CHARS) return;
        for (uint8_t height = 0; height < SONO_30P_CHAR_HEIGHT; height++)
        {
            memcpy(
                lcd_buffer + (y + height) * LCD_WIDTH + x, 
                sono_30p_buffer + height * SONO_30P_WIDTH + char_offset * SONO_30P_CHAR_WIDTH, 
                SONO_30P_CHAR_WIDTH * sizeof(uint16_t)
            );
        }
    }

    else if (font == SONO_50P)
    {
        if (char_offset >= SONO_50P_NUM_CHARS) return;
        for (uint8_t height = 0; height < SONO_50P_CHAR_HEIGHT; height++)
        {
            memcpy(
                lcd_buffer + (y + height) * LCD_WIDTH + x, 
                sono_50p_buffer + height * SONO_50P_WIDTH + char_offset * SONO_50P_CHAR_WIDTH, 
                SONO_50P_CHAR_WIDTH * sizeof(uint16_t)
            );
        }
    }
}

void Font_WriteCenter(enum fonts font, char* text, uint16_t center_x, uint16_t top_left_y)
{
    uint16_t half_width = strlen(text) * (font == SONO_30P ? SONO_30P_CHAR_WIDTH : SONO_50P_CHAR_WIDTH) / 2;
    if (half_width > center_x) return;

    uint16_t top_left_x = center_x - half_width;
    Font_WriteLeft(font, text, top_left_x, top_left_y);
}

void Font_WriteLeft(enum fonts font, char* text, uint16_t top_left_x, uint16_t top_left_y)
{
    char c;
    while(c = *text++)
    {   
        // Space
        if (c == ' ') {
            for (uint8_t height = 0; height < (font == SONO_30P ? SONO_30P_CHAR_HEIGHT : SONO_50P_CHAR_HEIGHT); height++)
                memset(lcd_buffer + (top_left_y + height) * LCD_WIDTH + top_left_x, 0, SONO_30P_CHAR_WIDTH * sizeof(uint16_t));
        }
        // Numbers map to the correct ASCII values
        else if (c >= '0' && c <= '9') {
            Font_Put(font, c - '0', top_left_x, top_left_y);
        }
        // Special characters after that map to A, B, C, etc.
        else if (c >= 'A') {
            Font_Put(font, c - 'A' + 10, top_left_x, top_left_y);
        }
        top_left_x += (font == SONO_30P ? SONO_30P_CHAR_WIDTH : SONO_50P_CHAR_WIDTH);
    }
}