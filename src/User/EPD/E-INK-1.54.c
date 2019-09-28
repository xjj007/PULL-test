#include <stdlib.h>
#include "E-INK-1.54.h"
#include "EPDIF.h"

int EPD_Init(EPD* epd, const unsigned char* lut)
	{
  epd->dc_pin = DC_PIN;
  epd->cs_pin = CS_PIN;
  epd->busy_pin = BUSY_PIN;
  epd->width = EPD_WIDTH;
  epd->height = EPD_HEIGHT;	//为什么要在这里定义
  
  epd->lut = lut;
  /* EPD hardware init start */
  EPD_SendCommand(epd, DRIVER_OUTPUT_CONTROL);
  EPD_SendData(epd, (EPD_HEIGHT - 1) & 0xFF);
  EPD_SendData(epd, ((EPD_HEIGHT - 1) >> 8) & 0xFF);
  EPD_SendData(epd, 0x00);                     // GD = 0; SM = 0; TB = 0;
  EPD_SendCommand(epd, BOOSTER_SOFT_START_CONTROL);
  EPD_SendData(epd, 0xD7);
  EPD_SendData(epd, 0xD6);
  EPD_SendData(epd, 0x9D);
  EPD_SendCommand(epd, WRITE_VCOM_REGISTER);
  EPD_SendData(epd, 0xA8);                     // VCOM 7C
  EPD_SendCommand(epd, SET_DUMMY_LINE_PERIOD);
  EPD_SendData(epd, 0x1A);                     // 4 dummy lines per gate
  EPD_SendCommand(epd, SET_GATE_TIME);
  EPD_SendData(epd, 0x08);                     // 2us per line
  EPD_SendCommand(epd, DATA_ENTRY_MODE_SETTING);
  EPD_SendData(epd, 0x03);                     // X increment; Y increment
  EPD_SetLut(epd, epd->lut);
  return 0;
}



void EPD_DigitalWrite(EPD* epd, int pin, int value) 
	{
  EpdDigitalWriteCallback(pin, value);
	}

int EPD_DigitalRead(EPD* epd, int pin)
	{
  return EpdDigitalReadCallback(pin);
	}

void EPD_DelayMs(EPD* epd, unsigned int delaytime)
	{  
  EpdDelayMsCallback(delaytime);
	}
	
/*******basic function for sending commands********/
void EPD_SendCommand(EPD* epd, unsigned char command) 
	{
  EPD_DigitalWrite(epd, epd->dc_pin, LOW);//DC脚拉低
  EpdSpiTransferCallback(command);
	}

	
/*******basic function for sending data*******/
void EPD_SendData(EPD* epd, unsigned char data) 
	{
  EPD_DigitalWrite(epd, epd->dc_pin, HIGH);
  EpdSpiTransferCallback(data);
	}

	
/****** put an image buffer to the frame memory******/
//显示图片
void EPD_SetFrameMemory(
  EPD* epd,	//IO信息
  const unsigned char* image_buffer,	//图像地址
  int x,//X   Y的坐标
  int y,
  int image_width,	//图像宽度
  int image_height
)
	{
  int x_end;
  int y_end;

  if (image_buffer == NULL ||			//非法参数判断
    x < 0 || image_width < 0 ||
    y < 0 || image_height < 0)
	{
    return;		
  }
  /* x point must be the multiple of 8 or the last 3 bits will be ignored */
	//X必须为8的倍数，否则最后三bit将会丢弃
  x &= 0xF8;
  image_width &= 0xF8;	//1111 1000
  if (x + image_width >= epd->width) //判断图像大小是否超过最大尺寸
		{
    x_end = epd->width - 1;
		}
		else 
			{
    x_end = x + image_width - 1;
			}
  if (y + image_height >= epd->height) 
		{
			y_end = epd->height - 1;
		} 
		else 
			{
				y_end = y + image_height - 1;
			}
  EPD_SetMemoryArea(epd, x, y, x_end, y_end);
  EPD_SetMemoryPointer(epd, x, y);	//设置内存起点
  EPD_SendCommand(epd, WRITE_RAM);//写入内存
  /* send the image data */
  for (int j = 0; j < y_end - y + 1; j++) 
			{
    for (int i = 0; i < (x_end - x + 1) / 8; i++) 
				{
					EPD_SendData(epd, image_buffer[i + j * (image_width / 8)]);
				}
			}
}



/*******clear the frame memory with the specified color********/
void EPD_ClearFrameMemory(EPD* epd, unsigned char color)
	{
  EPD_SetMemoryArea(epd, 0, 0, epd->width - 1, epd->height - 1);
  EPD_SetMemoryPointer(epd, 0, 0);
  EPD_SendCommand(epd, WRITE_RAM);
  /* send the color data */
  for (int i = 0; i < epd->width / 8 * epd->height; i++)
		{
    EPD_SendData(epd, color);
		}
	}

	
	
	
/*	update the display
*		there are 2 memory areas embedded in the e-paper display
* 	but once this function is called
* 	the the next action of SetFrameMemory or ClearFrame will 
* 	set the other memory area
	启动这个函数后，在设置SetFrameMemory或ClearFrame
	都将操作另一块内存
	*/
void EPD_DisplayFrame(EPD* epd)
	{
  EPD_SendCommand(epd, DISPLAY_UPDATE_CONTROL_2);
  EPD_SendData(epd, 0xC4);//这句话不要也是可以的
  EPD_SendCommand(epd, MASTER_ACTIVATION);
  EPD_SendCommand(epd, TERMINATE_FRAME_READ_WRITE);
	}


	
/********set the look-up tables********/
static void EPD_SetLut(EPD* epd, const unsigned char* lut)
	{
  epd->lut = lut;
  EPD_SendCommand(epd, WRITE_LUT_REGISTER);
  /* the length of look-up table is 30 bytes */
  for (int i = 0; i < 30; i++) 
		{
			EPD_SendData(epd, epd->lut[i]);
		} 
	}

	
	
/******private function to specify the memory area for data R/W******/
static void EPD_SetMemoryArea(EPD* epd, int x_start, int y_start, int x_end, int y_end) 
	{
		EPD_SendCommand(epd, SET_RAM_X_ADDRESS_START_END_POSITION);
		/* x point must be the multiple of 8 or the last 3 bits will be ignored */
		EPD_SendData(epd, (x_start >> 3) & 0xFF);
		EPD_SendData(epd, (x_end >> 3) & 0xFF);
		EPD_SendCommand(epd, SET_RAM_Y_ADDRESS_START_END_POSITION);
		EPD_SendData(epd, y_start & 0xFF);
		EPD_SendData(epd, (y_start >> 8) & 0xFF);
		EPD_SendData(epd, y_end & 0xFF);
		EPD_SendData(epd, (y_end >> 8) & 0xFF);
	}

/******private function to specify the start point for data R/W******/
static void EPD_SetMemoryPointer(EPD* epd, int x, int y)
	{
  EPD_SendCommand(epd, SET_RAM_X_ADDRESS_COUNTER);
  /* x point must be the multiple of 8 or the last 3 bits will be ignored */
  EPD_SendData(epd, (x >> 3) & 0xFF);
  EPD_SendCommand(epd, SET_RAM_Y_ADDRESS_COUNTER);
  EPD_SendData(epd, y & 0xFF);
  EPD_SendData(epd, (y >> 8) & 0xFF);
	}

const unsigned char lut_full_update[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

const unsigned char lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void Dispaly(EPD* epd,	//IO信息
  const unsigned char* image_buffer,	//图像地址
  int x,//X   Y的坐标
  int y,
  int image_width,	//图像宽度
  int image_height)
{
	EPD_SetFrameMemory(epd, image_buffer, x, y, image_width, image_height);    //显示
  EPD_DisplayFrame(epd);

}

/* END OF FILE */


