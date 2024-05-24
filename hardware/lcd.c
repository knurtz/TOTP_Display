/*****************************************************************************
* | File      	:   LCD_Driver.c, highly based on LCD_1inch28.c by Waveshare team
* | Author      :   Johannes Herold
* | Function    :   Hardware underlying interface for 1.28" LCD
* | Date        :   2022-06-22
*******************************************************************************/

#include "lcd.h"
#include "string.h"

LCD_Attributes lcd_attrib;
uint16_t lcd_buffer[LCD_WIDTH * LCD_HEIGHT];

/******************************************************************************
function : Hardware reset
parameter:
******************************************************************************/
static void LCD_Reset(void)
{
    LCD_SetRST(1);
    sleep_ms(100);
    LCD_SetRST(0);
    sleep_ms(100);
    LCD_SetRST(1);
    sleep_ms(100);
}

/******************************************************************************
function : Send command
parameter:
  reg : command register
******************************************************************************/
static void LCD_SendCommand(uint8_t reg)
{
    LCD_SetDC(0);
    LCD_SetCS(0);
	spi_write_blocking(DISP_SPI, &reg, 1);
    // LCD_SetCS(1);
}

/******************************************************************************
function : Send data
parameter:
  data : 8bit data
******************************************************************************/
static void LCD_SendData_8Bit(uint8_t data)
{
    LCD_SetDC(1);
    LCD_SetCS(0);
	spi_write_blocking(DISP_SPI, &data, 1);
    LCD_SetCS(1);
}

/******************************************************************************
function : Send data
parameter:
  data : 16bit data
******************************************************************************/
static void LCD_SendData_16Bit(uint16_t data)
{
	uint8_t tx_buf[2] =
	{
		(data >> 8) & 0xff,
		(data >> 0) & 0xff
	};

    LCD_SetDC(1);
    LCD_SetCS(0);
	spi_write_blocking(DISP_SPI, tx_buf, 2);
    LCD_SetCS(1);
}

/******************************************************************************
function :	initialize the LCD register
parameter:
******************************************************************************/
static void LCD_InitReg(void)
{
	LCD_SendCommand(0xEF);
	LCD_SendCommand(0xEB);
	LCD_SendData_8Bit(0x14); 

	LCD_SendCommand(0xFE);			 
	LCD_SendCommand(0xEF); 

	LCD_SendCommand(0xEB);	
	LCD_SendData_8Bit(0x14); 

	LCD_SendCommand(0x84);			
	LCD_SendData_8Bit(0x40); 

	LCD_SendCommand(0x85);			
	LCD_SendData_8Bit(0xFF); 

	LCD_SendCommand(0x86);			
	LCD_SendData_8Bit(0xFF); 

	LCD_SendCommand(0x87);			
	LCD_SendData_8Bit(0xFF);

	LCD_SendCommand(0x88);			
	LCD_SendData_8Bit(0x0A);

	LCD_SendCommand(0x89);			
	LCD_SendData_8Bit(0x21); 

	LCD_SendCommand(0x8A);			
	LCD_SendData_8Bit(0x00); 

	LCD_SendCommand(0x8B);			
	LCD_SendData_8Bit(0x80); 

	LCD_SendCommand(0x8C);			
	LCD_SendData_8Bit(0x01); 

	LCD_SendCommand(0x8D);			
	LCD_SendData_8Bit(0x01); 

	LCD_SendCommand(0x8E);			
	LCD_SendData_8Bit(0xFF); 

	LCD_SendCommand(0x8F);			
	LCD_SendData_8Bit(0xFF); 

	LCD_SendCommand(0xB6);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x20);

	LCD_SetOrientation(true);

	LCD_SendCommand(0x3A);			
	LCD_SendData_8Bit(0x05); 

	LCD_SendCommand(0x90);			
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x08); 

	LCD_SendCommand(0xBD);			
	LCD_SendData_8Bit(0x06);

	LCD_SendCommand(0xBC);			
	LCD_SendData_8Bit(0x00);	

	LCD_SendCommand(0xFF);			
	LCD_SendData_8Bit(0x60);
	LCD_SendData_8Bit(0x01);
	LCD_SendData_8Bit(0x04);

	LCD_SendCommand(0xC3);			
	LCD_SendData_8Bit(0x13);
	LCD_SendCommand(0xC4);			
	LCD_SendData_8Bit(0x13);

	LCD_SendCommand(0xC9);			
	LCD_SendData_8Bit(0x22);

	LCD_SendCommand(0xBE);			
	LCD_SendData_8Bit(0x11); 

	LCD_SendCommand(0xE1);			
	LCD_SendData_8Bit(0x10);
	LCD_SendData_8Bit(0x0E);

	LCD_SendCommand(0xDF);			
	LCD_SendData_8Bit(0x21);
	LCD_SendData_8Bit(0x0c);
	LCD_SendData_8Bit(0x02);

	LCD_SendCommand(0xF0);   
	LCD_SendData_8Bit(0x45);
	LCD_SendData_8Bit(0x09);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x26);
	LCD_SendData_8Bit(0x2A);

	LCD_SendCommand(0xF1);    
	LCD_SendData_8Bit(0x43);
	LCD_SendData_8Bit(0x70);
	LCD_SendData_8Bit(0x72);
	LCD_SendData_8Bit(0x36);
	LCD_SendData_8Bit(0x37);  
	LCD_SendData_8Bit(0x6F);

	LCD_SendCommand(0xF2);   
	LCD_SendData_8Bit(0x45);
	LCD_SendData_8Bit(0x09);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x26);
	LCD_SendData_8Bit(0x2A);

	LCD_SendCommand(0xF3);   
	LCD_SendData_8Bit(0x43);
	LCD_SendData_8Bit(0x70);
	LCD_SendData_8Bit(0x72);
	LCD_SendData_8Bit(0x36);
	LCD_SendData_8Bit(0x37); 
	LCD_SendData_8Bit(0x6F);

	LCD_SendCommand(0xED);	
	LCD_SendData_8Bit(0x1B); 
	LCD_SendData_8Bit(0x0B); 

	LCD_SendCommand(0xAE);			
	LCD_SendData_8Bit(0x77);

	LCD_SendCommand(0xCD);			
	LCD_SendData_8Bit(0x63);

	LCD_SendCommand(0x70);			
	LCD_SendData_8Bit(0x07);
	LCD_SendData_8Bit(0x07);
	LCD_SendData_8Bit(0x04);
	LCD_SendData_8Bit(0x0E); 
	LCD_SendData_8Bit(0x0F); 
	LCD_SendData_8Bit(0x09);
	LCD_SendData_8Bit(0x07);
	LCD_SendData_8Bit(0x08);
	LCD_SendData_8Bit(0x03);

	LCD_SendCommand(0xE8);			
	LCD_SendData_8Bit(0x34);

	LCD_SendCommand(0x62);			
	LCD_SendData_8Bit(0x18);
	LCD_SendData_8Bit(0x0D);
	LCD_SendData_8Bit(0x71);
	LCD_SendData_8Bit(0xED);
	LCD_SendData_8Bit(0x70); 
	LCD_SendData_8Bit(0x70);
	LCD_SendData_8Bit(0x18);
	LCD_SendData_8Bit(0x0F);
	LCD_SendData_8Bit(0x71);
	LCD_SendData_8Bit(0xEF);
	LCD_SendData_8Bit(0x70); 
	LCD_SendData_8Bit(0x70);

	LCD_SendCommand(0x63);			
	LCD_SendData_8Bit(0x18);
	LCD_SendData_8Bit(0x11);
	LCD_SendData_8Bit(0x71);
	LCD_SendData_8Bit(0xF1);
	LCD_SendData_8Bit(0x70); 
	LCD_SendData_8Bit(0x70);
	LCD_SendData_8Bit(0x18);
	LCD_SendData_8Bit(0x13);
	LCD_SendData_8Bit(0x71);
	LCD_SendData_8Bit(0xF3);
	LCD_SendData_8Bit(0x70); 
	LCD_SendData_8Bit(0x70);

	LCD_SendCommand(0x64);			
	LCD_SendData_8Bit(0x28);
	LCD_SendData_8Bit(0x29);
	LCD_SendData_8Bit(0xF1);
	LCD_SendData_8Bit(0x01);
	LCD_SendData_8Bit(0xF1);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x07);

	LCD_SendCommand(0x66);			
	LCD_SendData_8Bit(0x3C);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0xCD);
	LCD_SendData_8Bit(0x67);
	LCD_SendData_8Bit(0x45);
	LCD_SendData_8Bit(0x45);
	LCD_SendData_8Bit(0x10);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x00);

	LCD_SendCommand(0x67);			
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x3C);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(0x01);
	LCD_SendData_8Bit(0x54);
	LCD_SendData_8Bit(0x10);
	LCD_SendData_8Bit(0x32);
	LCD_SendData_8Bit(0x98);

	LCD_SendCommand(0x74);			
	LCD_SendData_8Bit(0x10);	
	LCD_SendData_8Bit(0x85);	
	LCD_SendData_8Bit(0x80);
	LCD_SendData_8Bit(0x00); 
	LCD_SendData_8Bit(0x00); 
	LCD_SendData_8Bit(0x4E);
	LCD_SendData_8Bit(0x00);					

	LCD_SendCommand(0x98);			
	LCD_SendData_8Bit(0x3e);
	LCD_SendData_8Bit(0x07);

	LCD_SendCommand(0x35);	
	LCD_SendCommand(0x21);

	LCD_SendCommand(0x11);
	sleep_ms(120);
	LCD_SendCommand(0x29);
	sleep_ms(20);
}

void LCD_SetOrientation(bool flipped)
{
	LCD_SendCommand(0x36);
	if (flipped) LCD_SendData_8Bit((1 << 3) | (1 << 6) | (1 << 7));		// set BGR, MX and MY bits
	else LCD_SendData_8Bit((1 << 3));									// only set BGR bit
}

/********************************************************************************
function : Set the resolution and scanning method of the screen
parameter:
  scan_dir: scan direction
********************************************************************************/
static void LCD_SetAttributes(uint8_t scan_dir)
{
    // Get the screen scan direction
    lcd_attrib.scan_dir = scan_dir;
    uint8_t MemoryAccessReg = 0x08;

    // Get GRAM and LCD width and height
    if (scan_dir == HORIZONTAL)
	{
        lcd_attrib.height = LCD_HEIGHT;
        lcd_attrib.width = LCD_WIDTH;
        MemoryAccessReg = 0xC8;
    } 
	else 
	{
        lcd_attrib.height = LCD_WIDTH;
        lcd_attrib.width = LCD_HEIGHT;
        MemoryAccessReg = 0x68;
    }

    // Set the read / write scan direction of the frame memory
    LCD_SendCommand(0x36); 					// MX, MY, RGB mode
    LCD_SendData_8Bit(MemoryAccessReg);		// 0x08 set RGB
}

/********************************************************************************
function : Initialize the LCD
parameter:
  scan_dir: scan direction
********************************************************************************/
void LCD_Init(uint8_t scan_dir)
{
    // Hardware reset
	LCD_SetCS(1);
	LCD_SetDC(1);
    LCD_Reset();

    // Set the resolution and scanning method of the screen
    LCD_SetAttributes(scan_dir);
    
    // Set the initialization register
    LCD_InitReg();

	// Reset to blank screen
    LCD_Clear(RGB(0, 0, 0));

	// Wait for refresh
	sleep_ms(500);

	// Backlight on
	LCD_SetBacklight(1);
}

/********************************************************************************
function : Sets the start position and size of the display area
parameter:
  Xstart: X direction Start coordinates
  Ystart: Y direction Start coordinates
  Xend: X direction end coordinates
  Yend: Y direction end coordinates
********************************************************************************/
void LCD_SetFrame(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{	
	// Set the X coordinates
	LCD_SendCommand(0x2A);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(x_start);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(x_end);

	// Set the Y coordinates
	LCD_SendCommand(0x2B);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(y_start);
	LCD_SendData_8Bit(0x00);
	LCD_SendData_8Bit(y_end);

	LCD_SendCommand(0x2C);
}


/******************************************************************************
function : Transmit image data in 16-bit mode
parameter:
******************************************************************************/
static void LCD_TransmitBuffer(uint16_t* buf, size_t length, uint8_t inc_buffer)
{
	// Switch SPI to 16-bit transmission mode 
	// -> set bit 3 of SSPCR0->DSS[3:0] for 16-bit data size (8-bit: 0b0111, 16-bit: 0b1111)
	spi_get_hw(DISP_SPI)->cr0 |= 1 << SPI_SSPCR0_DSS_MSB;

	LCD_SetDC(1);

	if (inc_buffer)
	{
		for (size_t i = 0; i < length; i++)
		{
			spi_get_hw(DISP_SPI)->dr = *(buf++);
			while(spi_get_hw(DISP_SPI)->sr & SPI_SSPSR_BSY_BITS);
		}	
	}

	else
	{
		for (size_t i = 0; i < length; i++)
		{
			spi_get_hw(DISP_SPI)->dr = *buf;
			while(spi_get_hw(DISP_SPI)->sr & SPI_SSPSR_BSY_BITS);
		}
	}
	
	// Switch SPI back 8-bit transmission mode 
	// -> clear bit 3 of SSPCR0->DSS[3:0] for 8-bit data size (8-bit: 0b0111, 16-bit: 0b1111)
	spi_get_hw(DISP_SPI)->cr0 &= ~(1 << SPI_SSPCR0_DSS_MSB);
}


/******************************************************************************
function : Clear screen with color
parameter:
******************************************************************************/
void LCD_Clear(uint16_t color)
{	
	LCD_ClearBuffer(color);
	LCD_DisplayImage(lcd_buffer);
}

void LCD_ClearBuffer(uint16_t color)
{	
	//memset(lcd_buffer, color, sizeof(lcd_buffer));
	for (size_t i = 0; i < count_of(lcd_buffer); i++) lcd_buffer[i] = color;
}

/******************************************************************************
function : Transmit single line of an image
parameter:
******************************************************************************/
bool LCD_DisplayImageLine(uint16_t x, uint16_t y, uint16_t width, uint16_t *image_line)
{
	if ((x + width - 1 >= LCD_WIDTH) || (y >= LCD_HEIGHT)) return false;

	LCD_SetFrame(x, y, x + width - 1, y);
	LCD_TransmitBuffer(image_line, width, 1);
	return true;
}

/******************************************************************************
function : Transmit a complete image in one go
parameter:
******************************************************************************/
void LCD_DisplayImageFrame(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *image)
{
	LCD_SetFrame(x_start, y_start, x_end, y_end);
	LCD_TransmitBuffer(image, (x_end - x_start + 1) * (y_end - y_start + 1), 1);
}

/******************************************************************************
function : Transmit a complete image filling the whole screen
parameter:
******************************************************************************/
void LCD_DisplayImage(uint16_t *image)
{
	LCD_DisplayImageFrame(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, image);
}

/******************************************************************************
function: Draw a point
parameter	:
	    X	: 	Set the X coordinate
	    Y	:	Set the Y coordinate
	  Color :	Set the color
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_SetFrame(x, y, x, y);
	LCD_SendData_16Bit(color); 	    
}

void LCD_DrawSquare(uint16_t center_x, uint16_t center_y, uint8_t side_length, uint16_t color)
{
	uint8_t half_side_length = (side_length - 1) >> 1;
	
	// handle quares outside the border later, for now just return
	if (half_side_length > center_x || (center_x + half_side_length - 1 >= LCD_WIDTH)) return;

	printf("Drawing square.\nCenter: %d, %d, Width: %d\n", center_x, center_y, side_length);
}

/*
void Paint_DrawPoint( UWORD Xpoint,       UWORD Ypoint, UWORD Color,
                      DOT_PIXEL Dot_Pixel,DOT_STYLE Dot_FillWay)
{
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_FillWay == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2*Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

void Paint_DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, 
                        uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill )
{
    if (X_Center > Paint.Width || Y_Center >= Paint.Height) {
        Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                Paint_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                Paint_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                Paint_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                Paint_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                Paint_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                Paint_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                Paint_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                Paint_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    }
}
*/