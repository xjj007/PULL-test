
#ifndef OLED_C
#define	OLED_C

#include "oled.h"
#include "fonts.h"
#include "oledfont.h"  	 
#include "dispatcher.h"
   
extern uint8_t paint[1024];

bool oledFresh	=	false;//OLEDˢ�±�־����OLED��Ҫˢ�µ�ʱ����λ

uint64_t word_catch=0;//��ʱ����ϳ��ַ�������

void IIC_Start()
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}
/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
	//OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)OLED_SDIN_Set();
		else 	   OLED_SDIN_Clr();
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)		Write_IIC_Data(dat);
   	
	else		Write_IIC_Command(dat);
}


void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uint16_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
	
	for(i=0;i<1024;i++)
	paint[i]=0;
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}

//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				//��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6);	


	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   

#if OLED_diration   == 0
	
	OLED_WR_Byte(0xA0,OLED_CMD);//set segment remap 	
	OLED_WR_Byte(0xC0,OLED_CMD);//Com scan direction	
#endif
#if OLED_diration   ==  1
    OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 	
	OLED_WR_Byte(0xC0,OLED_CMD);//Com scan direction	
#endif
#if OLED_diration   ==   2
    OLED_WR_Byte(0xA0,OLED_CMD);//set segment remap 	
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction	
#endif 
#if OLED_diration   ==   3
    OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 	
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction	
#endif
	
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	//OLED_WR_Byte(0x20,OLED_CMD);//set charge pump enable
	//OLED_WR_Byte(0x01,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	oledFresh=true;

}  



/*
����Ļ������λ����ʾ�ַ�
x:0-63
y:0-127
*/				
int ShowChar(u8 x,u8 y,u8 chr,sFONT* font,u8 color)
{
	
	if (x > 127)x = 0;
	if (y > 63)y = 0;
	uint8_t current_page=0,need_draw_page,offset_y;
	uint16_t base_adder=0,temp1=0;
	int chr_base=chr*font->step;
	uint8_t temp=0;
	uint8_t row;

	row = (font->Height - 1) / 8 + 1;
	
		offset_y=y%8;
		y=y/8;
	need_draw_page=((font->Height+offset_y-1)/8)+1;
	base_adder=x+y*128;
	for(u8 offset_x=0;offset_x<font->Width;offset_x++)
	{
		word_catch=0;		
			for(uint8_t i= row;i>0;i--)
			{			//���ֿ�ȡ����
				BYTE(word_catch,i-1)=font->table[chr_base+(i-1)*font->Width+offset_x];	
			}
			if(color)
				word_catch=~word_catch;
			word_catch<<=offset_y;
			temp1=base_adder+offset_x;
			temp=paint[temp1]&(~(0XFF<<offset_y));//ȡͷ
			word_catch|=temp;
			temp1=base_adder+(need_draw_page-1)*128+offset_x;
			if (temp1 < 1024&&(offset_y+font->Height)%8!=0)//�ж��Ƿ�Ϸ�
			{
			temp=paint[temp1];//ȡβ��
			temp&=(0XFF<<((offset_y+font->Height)%8));//���β��
			word_catch|=temp<<(8*(need_draw_page-1));
			}
			
			
			for(uint8_t i=0;i<need_draw_page;i++)
			{	
				temp1=base_adder+i*128+offset_x;
				//if (temp1 > 1023)continue;
			paint[temp1]=BYTE(word_catch,i);	 
			}

	}//����д����
	
	for(current_page=0;current_page<need_draw_page;current_page++)
		{	
			OLED_Set_Pos(x,y);
			for(uint8_t i=0;i<font->Width;i++)
			{
				OLED_WR_Byte(paint[base_adder+current_page*128+i],OLED_DATA);

			}
	y++;
if(y>7)return 0;
			}
	return 0;	
}

//��ȡָ��λ�õ�����
u8 get_buff(u8 x ,u8 y)
{
	return paint[x+y*128];
	
}
//��ʾ��������,��Ҫfont->wight*6�Ŀռ�
void show_int_num(u8 x,u8 y,int num,sFONT* font,u8 color)
{	
	u8 temp[6];
	if(num<0)
	{
		ShowChar(x,y,'-',font,color);//��ʾ����
		x+=font->Width;
		num=-num;
	}	
		temp[0]=num/10000+'0';
		temp[1]=num/1000%10+'0';
		temp[2]=num/100%10+'0';
		temp[3]=num/10%10+'0';
		temp[4]=num%10+'0';
		temp[5]='\0';
	show_string(x,y,temp,font,color);
}
//��ʾ�޷���������Ҫfont->wight*5�Ŀռ�
void show_uint16(u8 x,u8 y,uint16_t num,sFONT* font,u8 color)
{
	char temp[6];
		temp[0]=num/10000+'0';
		temp[1]=num/1000%10+'0';
		temp[2]=num/100%10+'0';
		temp[3]=num/10%10+'0';
		temp[4]=num%10+'0';
		temp[5]='\0';
	show_string(x,y,temp,font,color);
	
}
void show_uint32(u8 x,u8 y,uint32_t num,u8 valid_bit,sFONT* font,u8 color)
{	
	uint32_t m=10;
	m=oled_pow(m,valid_bit-1);
	char temp[valid_bit+1];
	for(uint8_t i=0;i<valid_bit;i++)
	{	
		temp[i]=num/m%10+'0';
		m=m/10;
	}
	temp[valid_bit]='\0';
	show_string(x,y,temp,font,color);
}
/*
valid_x	������������Чλ
valid_y :С��������Чλ
��ౣ����λС��
*/
void show_float(u8 x,u8 y,float num,u8 valid_x,u8 valid_y,sFONT* font,u8 color)
{
	uint16_t m=10;
	uint16_t temp_X,temp_y;
	temp_X=num/1;
	temp_y=(num-temp_X)*oled_pow(m,valid_y);
	show_uint32(x,y,temp_X,valid_x,font,color);
	show_string(x+valid_x*font->Width,y,".",font,color);
	show_uint32(x+(valid_x+1)*font->Width,y,temp_y,valid_y,font,color);
}

void show_string(u8 x,u8 y,u8* text,sFONT* font,u8 color)
{
	const char* p_text = text;
	ShowChar(x,y,*p_text-' ',font,color);
	x+=font->Width;
	p_text++;
	for(;*p_text!='\0';p_text++)
	{
		ShowChar(x,y,*p_text-' ',font,color);
		x+=font->Width;
	}

}
void draw_SolidRectangle(u8 x,u8 y,u8 Xend,u8 Yend,u8 Color)
{
	uint8_t width,height;
	uint8_t current_page=0,need_draw_page,offset_y;
	uint16_t base_adder=0,temp1=0;
	uint8_t temp=0;
	uint8_t row;
	if(Xend<=x)
		
		return;
	if(Yend<=y)
		return;
	width=Xend-x;
	height=Yend-y;
	

	row = (height - 1) / 8 + 1;
	
		offset_y=y%8;
		y=y/8;
	need_draw_page=((height+offset_y-1)/8)+1;
	base_adder=x+y*128;
	
	for(u8 offset_x=0;offset_x<width;offset_x++)
	{
		word_catch=0;		
			for(uint8_t i= row;i>0;i--)
			{	//���ֿ�ȡ����
				BYTE(word_catch,i-1)=Color;	//д����ɫ����
			}
			word_catch<<=offset_y;
			temp1=base_adder+offset_x;
			temp=paint[temp1]&(~(0XFF<<offset_y));//ȡͷ
			word_catch|=temp;
			temp1=base_adder+(need_draw_page-1)*128+offset_x;
			if (temp1 < 1024&&(offset_y+height)%8!=0)//�ж��Ƿ�Ϸ�
			{
			temp=paint[temp1];//ȡβ��
			temp&=(0XFF<<((offset_y+height)%8));//���β��
			word_catch|=temp<<(8*(need_draw_page-1));
			}
			
			
			for(uint8_t i=0;i<need_draw_page;i++)
			{	
				temp1=base_adder+i*128+offset_x;
				//if (temp1 > 1023)continue;
			paint[temp1]=BYTE(word_catch,i);	 
			}

	}//����д����
	
	for(current_page=0;current_page<need_draw_page;current_page++)
		{	
			OLED_Set_Pos(x,y);
			for(uint8_t i=0;i<width;i++)
			{
				OLED_WR_Byte(paint[base_adder+current_page*128+i],OLED_DATA);

			}
	y++;
if(y>7)return;
			}
	return;	
}
//��ˮƽ��
void Paint_DrawHorizontalLine(u8 x,u8 y,u8 length)
{

}

void show_CN(u8 x,u8 y,uint8_t* text,sFONT* font,u8 color)
{	
	 const uint8_t* p_text = text;
	ShowChar(x,y,*p_text,font,color);
	x+=font->Width;
	p_text++;
	for(;*p_text!='\0';p_text++)
	{
		ShowChar(x,y,*p_text,font,color);
		x+=font->Width;
	}

}
#if 1
//��ѡ��λ�÷�����ʾ
//Ŀǰ�������û����
void turnover(u8 x,u8 y, u8 xend,u8 yend)
{
	if (x > 127)x = 0;
	if (y > 64)y = 0;
	uint8_t width,height;
	uint8_t current_page=0,need_draw_page,offset_y;
	uint16_t base_adder=0,temp1=0;
	uint8_t temp=0;
	

	width=xend-x;
	height=yend-y;
	
		offset_y=y%8;
		y=y/8;
	need_draw_page=((height+offset_y-1)/8)+1;
	base_adder=x+y*128;//��ʾ�������ַ
for(current_page=0;current_page<need_draw_page;current_page++)
	{	
		if(current_page==0)
		{
		for(u8 offset_x=0;offset_x<width;offset_x++)
		{
			temp1=base_adder+current_page*128+offset_x;
			temp=paint[temp1];//&(~(0XFF<<offset_y));//ȡͷ
			temp=~temp;//ȡ��
			temp&=(0xff<<offset_y);		//��������λ
			paint[temp1]=paint[temp1]&(~(0XFF<<offset_y));//�����λ
			paint[temp1]|=temp;
			continue;
			
		}
}
		else if(current_page==(need_draw_page-1)&&offset_y!=0)
		{
			for(u8 offset_x=0;offset_x<width;offset_x++)
		{
			temp1=base_adder+current_page*128+offset_x;
			temp=paint[temp1];//&(~(0XFF<<offset_y));
			temp=~temp;//ȡ��
			temp&=(~(0xff<<offset_y));		//��������λ
			paint[temp1]=paint[temp1]&((0XFF<<offset_y));//�����λ
			paint[temp1]|=temp;
			continue;
		}
		}
		else
		{
				for(u8 offset_x=0;offset_x<width;offset_x++)
		{
			temp1=base_adder+current_page*128+offset_x;
			paint[temp1]=~paint[temp1];//�����λ
			continue;
		}
		}
	}
	
	for(current_page=0;current_page<need_draw_page;current_page++)
		{	
					OLED_Set_Pos(x,y);
			for(uint8_t i=0;i<width;i++)
			{
				OLED_WR_Byte(paint[base_adder+current_page*128+i],OLED_DATA);
			}
		y++;
		if(y>7)return;//д�������һҳ
			}
	return;	
}
#endif 

int ShowChar_s(u8 x,u8 y,u8 chr,sFONT* font,u8 color)
{
	
	if (x > 127)x = 0;
	if (y > 63)y = 0;
	uint8_t current_page=0,need_draw_page,offset_y;
	uint16_t base_adder=0,temp1=0;
	int chr_base=chr*font->step;
	uint8_t temp=0;
	uint8_t row;

	row = (font->Height - 1) / 8 + 1;
	
		offset_y=y%8;
		y=y/8;
	need_draw_page=((font->Height+offset_y-1)/8)+1;
	base_adder=x+y*128;
	for(u8 offset_x=0;offset_x<font->Width;offset_x++)
	{
		word_catch=0;		
			for(uint8_t i= row;i>0;i--)
			{			//���ֿ�ȡ����
				BYTE(word_catch,i-1)=font->table[chr_base+(i-1)*font->Width+offset_x];	
			}
			if(UNCOLOR)
				word_catch=~word_catch;
			
			word_catch<<=offset_y;
			temp1=base_adder+offset_x;
			temp=paint[temp1]&(~(0XFF<<offset_y));//ȡͷ
			word_catch|=temp;
			temp1=base_adder+(need_draw_page-1)*128+offset_x;
			if (temp1 < 1024&&(offset_y+font->Height)%8!=0)//�ж��Ƿ�Ϸ�
			{
			temp=paint[temp1];//ȡβ��
			temp&=(0XFF<<((offset_y+font->Height)%8));//���β��
			word_catch|=temp<<(8*(need_draw_page-1));
			}
			
			
			for(uint8_t i=0;i<need_draw_page;i++)
			{	
				temp1=base_adder+i*128+offset_x;
				//if (temp1 > 1023)continue;
			paint[temp1]=BYTE(word_catch,i);	 
			}

	}//����д����
	
	for(current_page=0;current_page<need_draw_page;current_page++)
		{	
			OLED_Set_Pos(x,y);
			for(uint8_t i=0;i<font->Width;i++)
			{
				OLED_WR_Byte(paint[base_adder+current_page*128+i],OLED_DATA);

			}
	y++;
if(y>7)return 0;
			}
	return 0;	
}


#endif /*OLED_C*/

