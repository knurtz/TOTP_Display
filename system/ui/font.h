#ifndef FONT_H
#define FONT_H

#define SONO_30P_WIDTH 496
#define SONO_30P_HEIGHT 27
#define SONO_30P_CHAR_WIDTH 19
#define SONO_30P_CHAR_HEIGHT 27
#define SONO_30P_NUM_CHARS 26

#define SONO_50P_WIDTH 436
#define SONO_50P_HEIGHT 38
#define SONO_50P_CHAR_WIDTH 31
#define SONO_50P_CHAR_HEIGHT 38
#define SONO_50P_NUM_CHARS 14

// Corresponding letters:   A B C D E F G H I J K L M N O
// 30P characters:          W h k m : , . ° C [C] [B] [D]   -> [C] = clock, [B] = battery, [D] = distance
// 50P characters:          ° C : %

enum fonts
{
    SONO_30P,
    SONO_50P
};

struct font_properties
{
    uint16_t width;
    uint16_t height;
    uint8_t char_width;
    uint8_t char_height;
    uint8_t num_chars;
};

void Font_Init(void);
void Font_Put(enum fonts font, uint8_t char_offset, uint16_t x, uint16_t y);
void Font_WriteCenter(enum fonts font, char* text, uint16_t center_x, uint16_t top_left_y);
void Font_WriteLeft(enum fonts font, char* text, uint16_t top_left_x, uint16_t top_left_y);

#endif /* FONT_H */
