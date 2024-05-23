#include "bmp.h"
#include "string.h"

#include "hardware/timer.h"
#include "lcd.h"
#include "ff.h"

uint8_t file_buffer[3 * 512];
uint16_t colortable[256];
BMP_Info global_bmp_info;

uint8_t BMP_GetInfo(const char* filename, BMP_Info* info, uint8_t* file_buffer)
{
    FIL file;
    UINT br;
    FRESULT res;

    //uint32_t fileinfo_time = time_us_32();

    // If no image data is provided, read necessary bytes into new buffer
    if (file_buffer == NULL)
    {
        uint8_t new_buffer[54];

        res = f_open(&file, filename, FA_READ);
        if (res != FR_OK) 
        {
            printf("Could not open file (%d)\n", res);
            return 0;
        }
        f_read(&file, new_buffer, sizeof(new_buffer), &br);
        f_close(&file);

        // Reassign buffer pointer
        file_buffer = new_buffer;
    }  

    if (strncmp((const char*)file_buffer, "BM", 2)) return 0;

    // Look for start of image data
    info->data_offset = LITTLE_ENDIAN_32BIT(file_buffer + 0x0a);

    // Extract image information
    info->width = LITTLE_ENDIAN_32BIT(file_buffer + 0x12);
    info->height = LITTLE_ENDIAN_32BIT(file_buffer + 0x16);

    info->bitcount = *(file_buffer + 0x1c);
    info->indexed = info->bitcount < 16 ? true : false;
    info->compressed = *(file_buffer + 0x1e) ? true : false;

    info->colortable_size = LITTLE_ENDIAN_16BIT(file_buffer + 0x2e);
    if (info->indexed && info->colortable_size == 0) info->colortable_size = (1 << info->bitcount);

    //printf("Got file info in %d us.\n", time_us_32() - fileinfo_time);

    return 1;
}

void BMP_PrintInfo(BMP_Info* info)
{
    printf("Picture size: %d x %d\n", info->width, info->height);
    printf("Picture is %sindexed with %d bits and %scompressed.\n", info->indexed ? "" : "not ", info->bitcount, info->compressed ? "" : "not ");
    printf("Color table has %d entries.\n", info->colortable_size);
    printf("Image data starts at offset 0x%x.\n", info->data_offset);
}

bool BMP_RenderToBuffer(const char* filename, uint16_t image_x, uint16_t image_y, uint16_t buffer_width, uint16_t buffer_height, uint16_t buffer[])
{
    FIL file;
    UINT br;

    //uint32_t transmit_line_time, read_file_time;
    //uint32_t render_start_time = time_us_32();

    if (f_open(&file, filename, FA_READ) != FR_OK) return 0;

    f_read(&file, file_buffer, 3 * 512, &br);
    //printf("Read 3 * 512 bytes in %d us.\n", time_us_32() - render_start_time);

    BMP_GetInfo(filename, &global_bmp_info, file_buffer);

    // Currently only uncompressed and indexed images are supported
    if (!global_bmp_info.indexed || global_bmp_info.compressed || global_bmp_info.colortable_size > 256) return 0;

    // Build color table
    //uint32_t colortable_time = time_us_32();

    uint8_t ct_cnt = 0;
    uint8_t r, g, b;
    uint8_t* offset = file_buffer + 54;

    for (ct_cnt = 0; ct_cnt < global_bmp_info.colortable_size; ct_cnt++)
    {   
        b = *(offset++);
        g = *(offset++);
        r = *(offset++);
        offset++;               // discard 4th byte
        colortable[ct_cnt] = RGB(r >> 3, g >> 2, b >> 3);
        //printf("Color %3d: %3d, %3d, %3d (%5d)\n", ct_cnt, r, g, b, colortable[ct_cnt]);
    }

    //printf("Built colortable in %d us.\n", time_us_32() - colortable_time);

    // Decode image data line by line
    offset = file_buffer + global_bmp_info.data_offset;
    size_t line_counter = 0;
    //uint16_t read_accesses = 0;

    for (int y = image_y + global_bmp_info.height - 1; y >= image_y; y--)
    {
        line_counter = y * buffer_width;
        for (int x = image_x; x < (image_x + global_bmp_info.width); x++)
        {
            if (offset - file_buffer >= 3 * 512) 
            {   
                //read_file_time = time_us_32();
                f_read(&file, file_buffer, 3 * 512, &br);
                //read_file_time = time_us_32() - read_file_time;
                offset = file_buffer;
                //read_accesses++;
            }
            //image_line[x] = colortable[*(offset++)];
            buffer[line_counter + x] = colortable[*(offset++)];
        }
        // Discard bytes if line is not a multiple of 4 pixels wide
        uint8_t leftover_bytes = global_bmp_info.width & 0b11;
        if (leftover_bytes) offset += leftover_bytes;
        //transmit_line_time = time_us_32();
        //LCD_DisplayImageLine(image_x, image_y + y, info->width, image_line);
    }
    //transmit_line_time = time_us_32() - transmit_line_time;

    f_close(&file);

    uint32_t now = time_us_32();
    /*printf("Transmitted one line to display in %d us.\nRendered image in %d us.\n%d read accesses were necessary.\nLast read access took %d us.",
        transmit_line_time,
        now - render_start_time,
        read_accesses,
        read_file_time);*/
    return true;
}

bool BMP_RenderToDisplay(const char* filename, uint16_t image_x, uint16_t image_y, bool send_immediately)
{
    BMP_RenderToBuffer(filename, image_x, image_y, LCD_WIDTH, LCD_HEIGHT, lcd_buffer);
    if (send_immediately) LCD_DisplayImage(lcd_buffer);

    return true;
}