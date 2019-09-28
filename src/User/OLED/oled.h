#ifndef __OLED_H
#define __OLED_H			  	 
#include	"stm32f10x.h"
#include "stdlib.h"	  
#include "fonts.h"

#define white 0xff
#define black 0

#define	COLOR	0 //黑底白字
#define	UNCOLOR	1 //白底黑字


#define OLED_MODE 0
#define SIZE 8
#define XLevelL			0x00
#define XLevelH			0x10
#define Max_Column		128
#define Max_Row			64
#define	Brightness		0xFF 
#define X_WIDTH 		128
#define Y_WIDTH 		64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//数据拆分宏定义
#define	BYTE(dwTemp,bit)		( *( (char *)(&dwTemp)+bit) )


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
	 
void OLED_Set_Pos(unsigned char x, unsigned char y);

void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack(void);

int  ShowChar(u8 x,u8 y,u8 chr,sFONT* font,u8 color);
void show_int_num(u8 x,u8 y,int num,sFONT* font,u8 color);
void show_uint16(u8 x,u8 y,uint16_t num,sFONT* font,u8 color);
void show_string(u8 x,u8 y,u8* text,sFONT* font,u8 color);
void show_CN(u8 x,u8 y,uint8_t* text,sFONT* font,u8 color);
void show_uint32(u8 x,u8 y,uint32_t num,u8 valid_bit,sFONT* font,u8 color);
void show_float(u8 x,u8 y,float num,u8 valid_x,u8 valid_y,sFONT* font,u8 color);
void draw_SolidRectangle(u8 x,u8 y,u8 Xend,u8 Yend,u8 Color);
void turnover(u8 x,u8 y, u8 xend,u8 yend);

#endif  /*__OLED_H*/
	  



