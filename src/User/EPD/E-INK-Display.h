#ifndef EPDPAINT_H
#define EPDPAINT_H

// Display orientation
#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     1

#include "../fonts/fonts.h"

typedef struct Paint_t {
    unsigned char* image;
    int width;
    int height;
    int rotate;	//Ðý×ª
} Paint;

void Paint_Init(Paint* paint, unsigned char* image, int width, int height);
void Paint_Clear(Paint* paint, int colored);
int  Paint_GetWidth(Paint* paint);
void Paint_SetWidth(Paint* paint, int width);
int  Paint_GetHeight(Paint* paint);
void Paint_SetHeight(Paint* paint, int height);
int  Paint_GetRotate(Paint* paint);
void Paint_SetRotate(Paint* paint, int rotate);
unsigned char* Paint_GetImage(Paint* paint);
void Paint_DrawAbsolutePixel(Paint* paint, int x, int y, int colored);
void Paint_DrawPixel(Paint* paint, int x, int y, int colored);
void Paint_DrawCharAt(Paint* paint, int x, int y, char ascii_char, sFONT* font, int colored);
void Paint_DrawStringAt(Paint* paint, int x, int y, const char* text, sFONT* font, int colored);
void Paint_DrawLine(Paint* paint, int x0, int y0, int x1, int y1, int colored);
void Paint_DrawHorizontalLine(Paint* paint, int x, int y, int width, int colored);
void Paint_DrawVerticalLine(Paint* paint, int x, int y, int height, int colored);
void Paint_DrawRectangle(Paint* paint, int x0, int y0, int x1, int y1, int colored);
void Paint_DrawFilledRectangle(Paint* paint, int x0, int y0, int x1, int y1, int colored);
void Paint_DrawCircle(Paint* paint, int x, int y, int radius, int colored);
void Paint_DrawFilledCircle(Paint* paint, int x, int y, int radius, int colored);
void show_num_5(Paint* paint,int x,int y,uint16_t num,sFONT* font,int colored);
void show_num_4(Paint* paint,int x,int y,uint16_t num,sFONT* font,int colored);
void show_num_2(Paint* paint,int x,int y,uint16_t num,sFONT* font,int colored);

#endif

/* END OF FILE */

