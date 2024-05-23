/*****************************************************************************
* | File      	:   lcd.h, highly based on LCD_1inch28.h by Waveshare team
* | Author      :   Johannes Herold
* | Function    :   Hardware underlying interface for 1.28" LCD
* | Date        :   2022-06-29
*******************************************************************************/

#ifndef LCD_H
#define LCD_H	

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#include "hardware.h"

#define LCD_HEIGHT 		240
#define LCD_WIDTH 		240

#define HORIZONTAL 		0
#define VERTICAL   		1

#define LCD_SetBacklight(_value) 			gpio_put(DISP_BL, _value)
#define LCD_SetCS(_value)	    			gpio_put(DISP_CS, _value)
#define LCD_SetRST(_value)	  				gpio_put(DISP_RST, _value)
#define LCD_SetDC(_value)	    			gpio_put(DISP_DC, _value)

typedef struct {
	uint16_t 	width;
	uint16_t 	height;
	uint8_t 	scan_dir;
} LCD_Attributes;

#define RGB(r, g, b)				(uint16_t)(r << 11 | g << 5 | b)

extern LCD_Attributes lcd_attrib;
extern uint16_t lcd_buffer[LCD_WIDTH * LCD_HEIGHT];

void LCD_Init(uint8_t scan_dir);
void LCD_SetOrientation(bool flipped);

void LCD_Clear(uint16_t color);
void LCD_ClearBuffer(uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawSquare(uint16_t center_x, uint16_t center_y, uint8_t side_length, uint16_t color);

bool LCD_DisplayImageLine(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t *image_line);
void LCD_DisplayImageFrame(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *image);
void LCD_DisplayImage(uint16_t *image);

#endif	// LCD_H
