#include "E-INK-Display.h"

//初始化Paint结构体
void Paint_Init(Paint* paint, unsigned char* image, int width, int height) 
	{
    paint->rotate = ROTATE_270;	//旋转0度
    paint->image = image;
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    paint->width = width % 8 ? width + 8 - (width % 8) : width;
    paint->height = height;
	}

	
	
/*******clear the image*******/
	//清除图片
void Paint_Clear(Paint* paint, int colored) 
	{
    for (int x = 0; x < paint->width; x++) 
		{
        for (int y = 0; y < paint->height; y++) 
				{
            Paint_DrawAbsolutePixel(paint, x, y, colored);//对指定位置的像素写参数
        }
    }
	}

	
	
/*******this draws a pixel by absolute coordinates*******/
	//此函数没有实质的写入，只是定义了paint->image结构体
void Paint_DrawAbsolutePixel(Paint* paint, int x, int y, int colored) 
	{
    if (x < 0 || x >= paint->width || y < 0 || y >= paint->height) 	//参数检查
			{
        return;
			}
    if (IF_INVERT_COLOR)//如果定义了反转色彩
			{
        if (colored)
					{
            paint->image[(x + y * paint->width) / 8] |= 0x80 >> (x % 8);
					} 
					else 
							{
            paint->image[(x + y * paint->width) / 8] &= ~(0x80 >> (x % 8));
						}
			} 
			else
				{
        if (colored) 
					{
            paint->image[(x + y * paint->width) / 8] &= ~(0x80 >> (x % 8));
					} 
					else 
						{
            paint->image[(x + y * paint->width) / 8] |= 0x80 >> (x % 8);
						}
				}
	}

	
/*******Getters and Setters*******/
unsigned char* Paint_GetImage(Paint* paint)
	{
    return paint->image;
	}

int Paint_GetWidth(Paint* paint)
	{
    return paint->width;
	}

void Paint_SetWidth(Paint* paint, int width)
	{
    paint->width = width % 8 ? width + 8 - (width % 8) : width;
	}

int Paint_GetHeight(Paint* paint) 
	{
    return paint->height;
	}

void Paint_SetHeight(Paint* paint, int height) 
	{
    paint->height = height;
	}

int Paint_GetRotate(Paint* paint)
	{
    return paint->rotate;
	}

void Paint_SetRotate(Paint* paint, int rotate)
	{
    paint->rotate = rotate;
	}

	
/*******this draws a pixel by the coordinates*******/
	//通过坐标系画点
void Paint_DrawPixel(Paint* paint, int x, int y, int colored) 
	{
    int point_temp;
    if (paint->rotate == ROTATE_0) 
			{
        if(x < 0 || x >= paint->width || y < 0 || y >= paint->height) //参数检查
					{
            return;
					}
        Paint_DrawAbsolutePixel(paint, x, y, colored);
			} 
			else if (paint->rotate == ROTATE_90)
			{
        if(x < 0 || x >= paint->height || y < 0 || y >= paint->width)
					{
          return;
					}
        point_temp = x;
        x = paint->width - y;
        y = point_temp;
        Paint_DrawAbsolutePixel(paint, x, y, colored);
			}
			else if (paint->rotate == ROTATE_180)
				{
        if(x < 0 || x >= paint->width || y < 0 || y >= paint->height)
					{
						return;
					}
        x = paint->width - x;
        y = paint->height - y;
        Paint_DrawAbsolutePixel(paint, x, y, colored);
				} 
				else if (paint->rotate == ROTATE_270)
					{
						if(x < 0 || x >= paint->height || y < 0 || y >= paint->width) 
							{
								return;
							}
        point_temp = x;
        x = y;
        y = paint->height - point_temp;
        Paint_DrawAbsolutePixel(paint, x, y, colored);
					}
	}

	
	
/*******this draws a charactor on the frame buffer but not refresh*******/
void Paint_DrawCharAt(Paint* paint, int x, int y, char ascii_char, sFONT* font, int colored) 
	{
    int i, j;
    unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->table[char_offset];
    for (j = 0; j < font->Height; j++) 
		{
        for (i = 0; i < font->Width; i++) 
				{
            if (*ptr & (0x80 >> (i % 8)))
							{
                Paint_DrawPixel(paint, x + i, y + j, colored);
							}
            if (i % 8 == 7)
							{
                ptr++;
							}
        }
        if (font->Width % 8 != 0)
					{
            ptr++;
					}
    }
	}

	
/*******this displays a string on the frame buffer but not refresh*******/
void Paint_DrawStringAt(Paint* paint, int x, int y, const char* text, sFONT* font, int colored)
	{
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    
    /* Send the string character by character on EPD */
    while (*p_text != 0)
			{
        /* Display one character on EPD */
        Paint_DrawCharAt(paint, refcolumn, y, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        p_text++;
        counter++;
			}
	}

	
	
/*******this draws a line on the frame buffer*******/
void Paint_DrawLine(Paint* paint, int x0, int y0, int x1, int y1, int colored)
	{
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while((x0 != x1) && (y0 != y1)) 
			{
        Paint_DrawPixel(paint, x0, y0 , colored);
        if (2 * err >= dy) 
					{     
            err += dy;
            x0 += sx;
					}
        if (2 * err <= dx) 
					{
            err += dx; 
            y0 += sy;
					}
			}	
	}

	
	
/*******this draws a horizontal line on the frame buffer******/
void Paint_DrawHorizontalLine(Paint* paint, int x, int y, int line_width, int colored)
	{
    int i;
    for (i = x; i < x + line_width; i++)
		{
        Paint_DrawPixel(paint, i, y, colored);
    }
	}

	
/*******this draws a vertical line on the frame buffer*******/
void Paint_DrawVerticalLine(Paint* paint, int x, int y, int line_height, int colored) 
	{
    int i;
    for (i = y; i < y + line_height; i++) 
		{		//画点
        Paint_DrawPixel(paint, x, i, colored);
    }
	}

	
/*******this draws a rectangle*******/
void Paint_DrawRectangle(Paint* paint, int x0, int y0, int x1, int y1, int colored)
	{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    
    Paint_DrawHorizontalLine(paint, min_x, min_y, max_x - min_x + 1, colored);
    Paint_DrawHorizontalLine(paint, min_x, max_y, max_x - min_x + 1, colored);
    Paint_DrawVerticalLine(paint, min_x, min_y, max_y - min_y + 1, colored);
    Paint_DrawVerticalLine(paint, max_x, min_y, max_y - min_y + 1, colored);
	}

	
/*******this draws a filled rectangle*******/
void Paint_DrawFilledRectangle(Paint* paint, int x0, int y0, int x1, int y1, int colored)
	{
    int min_x, min_y, max_x, max_y;
    int i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    
    for (i = min_x; i <= max_x; i++)
		{//画水平线
      Paint_DrawVerticalLine(paint, i, min_y, max_y - min_y + 1, colored);
    }
	}

/*******this draws a circle*******/
void Paint_DrawCircle(Paint* paint, int x, int y, int radius, int colored) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do 
			{
        Paint_DrawPixel(paint, x - x_pos, y + y_pos, colored);
        Paint_DrawPixel(paint, x + x_pos, y + y_pos, colored);
        Paint_DrawPixel(paint, x + x_pos, y - y_pos, colored);
        Paint_DrawPixel(paint, x - x_pos, y - y_pos, colored);
        e2 = err;
        if (e2 <= y_pos) 
					{
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) 
							{
              e2 = 0;
							}
					}
        if (e2 > x_pos) 
					{
            err += ++x_pos * 2 + 1;
					}
			}
			while (x_pos <= 0);
}

/*******this draws a filled circle*******/
void Paint_DrawFilledCircle(Paint* paint, int x, int y, int radius, int colored)
	{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do
			{
        Paint_DrawPixel(paint, x - x_pos, y + y_pos, colored);
        Paint_DrawPixel(paint, x + x_pos, y + y_pos, colored);
        Paint_DrawPixel(paint, x + x_pos, y - y_pos, colored);
        Paint_DrawPixel(paint, x - x_pos, y - y_pos, colored);
        Paint_DrawHorizontalLine(paint, x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        Paint_DrawHorizontalLine(paint, x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
        e2 = err;
        if (e2 <= y_pos) 
					{
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos)
							{
                e2 = 0;
							}
					}
        if(e2 > x_pos) 
					{
            err += ++x_pos * 2 + 1;
					}
			} 
			while(x_pos <= 0);
}
void show_num_5(Paint* paint,int x,int y,uint16_t num,sFONT* font,int colored)
{
	char temp[6];
		temp[0]=num/10000+'0';
		temp[1]=num/1000%10+'0';
		temp[2]=num/100%10+'0';
		temp[3]=num/10%10+'0';
		temp[4]=num%10+'0';
		temp[5]='\0';
	Paint_DrawStringAt(paint,x,y,temp,font, colored); 
}

void show_num_4(Paint* paint,int x,int y,uint16_t num,sFONT* font,int colored)
{
	char temp[5];
		temp[0]=num/1000+'0';
		temp[1]=num/100%10+'0';
		temp[2]=num/10%10+'0';
		temp[3]=num%10+'0';
		temp[4]='\0';
	Paint_DrawStringAt(paint,x,y,temp,font, colored); 
}
void show_num_2(Paint* paint,int x,int y,uint16_t num,sFONT* font,int colored)
{
	char temp[3];
		temp[0]=num/10%10+'0';
		temp[1]=num%10+'0';
		temp[2]='\0';
	Paint_DrawStringAt(paint,x,y,temp,font, colored); 
}


/* END OF FILE */























