#ifndef	UI_C
#define	UI_C
#define USE_OLED
#include	"UI.H"
#include	"ADC.H"
#include	"fonts.h"
#include	"USART_CONFIG.H"
#include	"fliter.h"
#include	"PWM.H"
#include	"flash.h"
#include	"dispatcher.h"

extern uint8_t  key_vaule_buff;
extern uint16_t ADC_Fliter[4];
extern uint16_t ADC_sourse[4];
extern uint8_t  key_flag;
extern uint16_t current_offset;//电流计偏移量
extern float    current_ratio;
extern float    voltage_ratio;
extern uint16_t voltage_offset;
extern all_data data;
extern uint16_t fre;//PWM频率
extern uint32_t baud_rate[12];
extern uint8_t  baud;
extern float    a_parameter[a_end];//低通滤波器参数
/*
OLED.C
OLED刷新标志
*/
extern bool oledFresh;	

uint16_t step=10,step_t=500;//每步长度

/*
最简单的办法，给每个页面加上标号，按键操作通过枚举实现
*/
struct	_sysUI
{
	int couser;		//光标位置
	int	Page;		//页面编号 
	int OlPage;		//当前页面，这种方式并不好
	bool modify;	//修改模式
}SYSUI;


#ifdef USE_EPD
#include "../EPD/E-INK-1.54.h"
#include "Definition.h"

char CN0[]={32,33,34,35,'\0'};//自动模式
char CN1[]={36,37,38,39,'\0'};//手动模式
char	CN2[]={40,41,42,43,44,'\0'};//PWM输出
char	CN3[]={45,46,47,48,'\0'};//系统设置
extern EPD epd;
extern Paint paint;
extern uint16_t ADC_Fliter[end];
extern uint8_t key_vaule_buff;
extern uint8_t key_flag;
//extern uint16_t	weight;
//开机清屏
void START_CLEAR(const unsigned char* EPD_BUFF)
{
	Paint_Clear(&paint, UNCOLORED);
	Dispaly(&epd, EPD_BUFF, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	delay_ms(1500);
	
	Dispaly(&epd, EPD_BUFF, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	delay_ms(1500);
	if(EPD_Init(&epd, lut_partial_update) != 0) 
			{
				;
			}
}
void power_on(const unsigned char* EPD_BUFF)
{	
	Paint_DrawFilledRectangle(&paint, 15, 20, 185, 55, COLORED);					//画矩形
  Paint_DrawStringAt(&paint,18,30,"Tension test", &Font20, UNCOLORED);	//写字
	Paint_DrawStringAt(&paint,18,70,"please wait ", &Font20, COLORED);
  Paint_DrawStringAt(&paint,57,170,"powerd by XJJ",&Font16, COLORED);
	
	Dispaly(&epd, EPD_BUFF, 0, 0, epd.width, epd.height);
	delay_ms(1000);

}
//这个函数应该直到功能选择后才退出
void show_function_select(const unsigned char* EPD_BUFF)
{	
		int cursor=18;
	uint8_t line_dis=40;//行间距
	Paint_Clear(&paint, UNCOLORED);
	show_status_bar();
	Paint_DrawStringAt(&paint,0,cursor,">", &Font24,COLORED);
	Paint_DrawStringAt(&paint,18,18,CN0, &FontCN, COLORED);
	Paint_DrawStringAt(&paint,18,58,CN1, &FontCN, COLORED);
	Paint_DrawStringAt(&paint,18,98,CN2, &FontCN, COLORED);	
	Paint_DrawStringAt(&paint,18,138,CN3, &FontCN, COLORED);
	Dispaly(&epd, EPD_BUFF, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	delay_ms(300);
	
	while(1){	
		show_status_bar();	//刷新状态栏
	
	//if(key_flag==2){
	switch(key_vaule_buff)
	{

		case down:
		Paint_DrawCharAt(&paint,0,cursor,'>', &Font24, UNCOLORED);
		cursor+=line_dis;
		if(cursor>18+line_dis*3)cursor=18;
		Paint_DrawStringAt(&paint,0,cursor,">", &Font24, COLORED);
		break;
		
		case up:
			Paint_DrawCharAt(&paint,0,cursor,'>', &Font24, UNCOLORED);
			cursor-=line_dis;
		if(cursor<0)cursor=18+line_dis*3;
		Paint_DrawStringAt(&paint,0,cursor,">", &Font24, COLORED);
		break;
		
		case mid :
		
				switch(cursor)
						{
							case 18:
								auto_Tension(EPD_BUFF);
							break;
			
							case 58:
								hand_Tension();
							break;
			
							case 98:
							PWM_OutPut();
							break;
							case 138:
									system_set();
							break;
						}
			break;
	};
//}
	
	Dispaly(&epd, EPD_BUFF, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	key_vaule_buff=no_key;
	key_flag=0;
	delay_ms(300);
}

}

void show_status_bar()
{
	Paint_DrawFilledRectangle(&paint, 0, 0, 200,10, UNCOLORED);
	Paint_DrawFilledRectangle(&paint, 0,  180, 200,200, UNCOLORED);
	Paint_DrawStringAt(&paint,0,0,"bat:    cur:    K:    P:  ",
											&Font12,COLORED);
	
	Paint_DrawStringAt(&paint,0,180,"weight:    ",
											&Font12,COLORED);
	show_num_4(&paint,28,0,ADC_Fliter[BAT],&Font12,COLORED);
	show_num_4(&paint,84,0,ADC_Fliter[CUR],&Font12,COLORED);
	show_num_4(&paint,126,0,ADC_Fliter[KEY],&Font12 ,COLORED);
	show_num_4(&paint,168,0,ADC_Fliter[poten],&Font12,COLORED);
	show_num_5(&paint,49,180,ADC_Fliter[weight],&Font12,COLORED);

}
//自动拉力测试
//光标应该有闪烁效果
int auto_Tension(const unsigned char* EPD_BUFF)
{	
	uint8_t i=1;
	uint16_t cursor=18;
	char step=ADC_Fliter[poten]/100;
	wave_selct();
	Dispaly(&epd, EPD_BUFF, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	delay_ms(300);
	while(1){
		i++;
		step=ADC_Fliter[poten]/100;
		show_status_bar();
	switch(key_vaule_buff)
	{	
		case no_key:
			if(i%2==1)
			Paint_DrawFilledRectangle(&paint,18,cursor,46,42,UNCOLORED);
			else 
			show_num_2(&paint,18,cursor,step,&Font24,COLORED);
			break;
		case left:
			return 0;
		case up:
				Paint_DrawCharAt(&paint,0,cursor,'>', &Font24, UNCOLORED);
			cursor-=40;
		if(cursor<0)cursor=98;
		Paint_DrawStringAt(&paint,0,cursor,">", &Font24, COLORED);
		break;
		
		case down:
			Paint_DrawCharAt(&paint,0,cursor,'>', &Font24, UNCOLORED);
		cursor+=40;
		if(cursor>100)cursor=18;
		Paint_DrawStringAt(&paint,0,cursor,">", &Font24, COLORED);
		break;
		
		case mid :
			if(cursor==98);
		//次出调用下级页面
				break;
	
	}
		Dispaly(&epd, EPD_BUFF, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
		key_vaule_buff=no_key;
		delay_ms(300);
	}
}
//手动拉力测试
void hand_Tension()
{
;
}

void PWM_OutPut()
{
;
}

void system_set()
{
;
}
	

void wave_selct()
{
	//paint.height=164;
	//paint.width=164;
	Paint_DrawFilledRectangle(&paint, 18, 18, 182,182, UNCOLORED);
	Paint_DrawStringAt(&paint,18,18,"10% step per sec", &Font20, COLORED);
	Paint_DrawStringAt(&paint,18,58,"50hzPWM", &Font20, COLORED);
	Paint_DrawStringAt(&paint,18,98,"ENTER", &Font20, COLORED);
	//Paint_DrawStringAt(&paint,18,138,"25% step per sec", &Font20, COLORED);
}
#endif

#ifdef USE_OLED

#include	"oled.h"

void UIinit()
{
	SYSUI.Page=0;
	SYSUI.couser=0;
	SYSUI.modify=false;
	OLED_Clear(); 

}

/*
OLED刷新任务
所有的页面最好能有一样的结构
默认15ms刷新一次
只有当页面数据发生变化的时候才需要刷新，直接写入整页数据
*/
void OLED_Refresh()
{	
  
	switch(SYSUI.Page )
	{
		case 0:
		power_on();//上电开机页
        if(sys_time>1500)
        {
            SYSUI.Page=1;  //1.5s后开机跳转
            OLED_Clear();   //跳转前清屏
        }
		break;
		
		case 1:		//菜单选择
		page2();
        SYSUI.Page=2;
		break;
        case 2:
            fresh_data();
         break;
			
	}


}

/*
+------------+
| 拉力测试台 |
|POWER BY XJJ|
+------------+
*/
void power_on()
{	
	uint8_t temp[6]={32,33,34,35,36,'\0'};//拉力测试台
	show_string(4,10,temp,&FontCN24,COLOR);//24号中文字
	show_string(20,45,"POWER BY XJJ",&Font8X16,COLOR);
}

void show_function()
{	
	uint8_t cursor=0;
	
	page0();//只运行一次
	turnover(8,cursor,72,cursor+16);
	while(1)
	{
		switch(getkey())
		{
			case up:
				turnover(8,cursor,72,cursor+16);
				cursor-=16;
				if(cursor>48)cursor=48;
				turnover(8,cursor,72,cursor+16);
				break;
			case down:
				turnover(8,cursor,72,cursor+16);
				cursor+=16;
				if(cursor>48)cursor=0;
				turnover(8,cursor,72,cursor+16);
				break;
			
			case mid:
					switch(cursor)
					{
					case 0:
					auto_Tension();
					
					break;
					case 16:
					hand_Tension();
					break;
					case 32:
					PWM_OutPut();
					break;
				
					case 48:
					Sys_set();
					break;
			
					}
					page0();
					cursor=0;
					turnover(8,cursor,72,cursor+16);
			break;
			
		
		}
	}
	
}
void auto_Tension()
{	
	/*步进1%-25%*/
	
	uint8_t cursor=0;
	
	page1();
	while(1)
	{
		
		switch(getkey())
		{
			case down:
				cursor++;
				if(cursor==3)cursor=0;
				show_uint32(90,0,step,2,&Font8X16,(cursor%3)==0);
				show_uint32(90,16,step_t,4,&Font8X16,(cursor%3)==1);
				show_string(80,32,"NEXT->",&Font8X16,(cursor%3)==2);

			break;
			case up:
				cursor--;
				if(cursor==255)cursor=2;
				show_uint32(90,0,step,2,&Font8X16,(cursor%3)==0);
				show_uint32(90,16,step_t,4,&Font8X16,(cursor%3)==1);
				show_string(80,32,"NEXT->",&Font8X16,(cursor%3)==2);
			break;
			case left:
				return;
			case mid :
				
					switch(cursor)
					{
						case 0:
						set_PwmStep();
						break;
						case 1:
						set_step_t();
						break;
						
						case 2:
						ESC_call();//询问是否校准电调
						page1();
						break;
				
					}
					
			break;
			
				
		}
		
	}
	
}

/*手动拉力测试*/
void hand_Tension()
{	
	uint8_t temp=0;
	page5();
	while(1){
		temp++;
		if(temp==80)temp=0;
		poten_Pass();
		if(ADC_Fliter[poten]<4000&&ADC_Fliter[poten]>100)
					if(temp/40)
					show_string(0,30,"thottle not 0!!",&Font8X16,COLOR);
					
						else
					show_string(0,30,"               ",&Font8X16,COLOR);
						
		else{ 
				show_string(0,30,"                 ",&Font8X16,COLOR);
				switch(getkey())
				{	

					case mid:
					case right:
						show_data(hand_mode);
						page5();
					break;
					case left:
					return;
				}
			}

}
	}
void Sys_set()
{	
	uint8_t cursor=0;//光标
	page7();
	fresh_cursor(cursor);
	while(1)
	{
		switch(getkey())
		{	
			case down:
				cursor++;
				if(cursor==8)cursor=0;//上限下限依据页数来确定
			fresh_cursor(cursor);
				break;
			case up:
				cursor--;
			if(cursor>8)cursor=7;
			fresh_cursor(cursor);
				break;
			case mid:
				set_Parameters(cursor);
				//while(key_vaule()!=no_key)delay_ms(10);//等待按键释放
			break;
			case left:
				write_flash();
				return;
		}
		
	}
}

void fresh_cursor(uint8_t cursor)
{
	switch(cursor/4)
		{
			case 0:
			page7();
			break;
			case 1:
			page8();
			break;
			case 2:
				break;
			
			}
	
	switch(cursor)
	{
		case e_step:
			show_uint32(80,0,step,2,&Font8X16,UNCOLOR);
		break;
		case e_step_t:
			show_uint32(80,16,step_t,4,&Font8X16,UNCOLOR);
		break;
		
		case e_fre:
			show_uint32(80,32,fre,3,&Font8X16,UNCOLOR);
		break;
		case e_buad:
			show_uint32(70,48,baud_rate[baud],7,&Font8X16,UNCOLOR);
		break;
		case e_cur_ratio:
			show_float(75,0,current_ratio,1,3,&Font8X16,UNCOLOR);
		break;
		case e_cur_offset:
			show_uint32(75,16,current_offset,4,&Font8X16,UNCOLOR);
		break;
		case e_vol_ratio:
			show_float(75,32,voltage_ratio,1,3,&Font8X16,UNCOLOR);
		break;
		case e_vol_offset:
			show_uint32(75,48,voltage_offset,4,&Font8X16,UNCOLOR);
		break;
	}

}

void set_Parameters(uint8_t Parameters)
{	
	#if 0
	switch(Parameters/4)
	{
		case 0:
			page7();
		break;
		case 1:
			page8();
		break;
		case 2:
			break;
	}
	#endif
	switch(Parameters)
	{
		case e_step:
			set_PwmStep();
		break;
		
		case e_step_t:
			set_step_t();
		break;
		
		case	e_fre:
			set_fre();
		break;
		
		case	e_buad:
			set_baud();
		break;
		
		case	e_cur_ratio:
			set_current_ratio();
		break;
		
		case	e_cur_offset:
			set_current_offset();
		break;
		
		case	e_vol_ratio:
			set_voltage_ratio();
		break;
		
		case	e_vol_offset:
			set_voltage_offset();
		break;
		
		default :
			return;
	}
	
}

//设置每步跨度
void set_PwmStep()
{	
	uint8_t temp_step=step,COL=0;
	uint16_t temp=0;
	while(1){
	switch(getkey())
	{	
		case	up:
			temp_step++;
		show_uint32(80,0,temp_step,2,&Font8X16,COL);
		break;
		case down:		
			temp_step--;
		show_uint32(80,0,temp_step,2,&Font8X16,COL);
		break;	
		case mid:
			step=temp_step;
		case left:
			show_uint32(80,0,step,2,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_step<1)temp_step=1;
			if(temp_step>25)temp_step=25;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_uint32(80,0,temp_step,2,&Font8X16,COL);
		}
	}
}
//设置每步时间
void set_step_t()/*每步时间100ms-3000ms*/
{	
	uint16_t temp_step=step_t,COL=0;
	uint16_t temp=0;
	while(1){
	switch(getkey())
	{	
		case	up:
			temp_step+=100;
		show_uint32(80,16,temp_step,4,&Font8X16,COL);
		break;
		case down:		
			temp_step-=100;
		show_uint32(80,16,temp_step,4,&Font8X16,COL);
		break;	
		case mid:
			step_t=temp_step;
		case left:
			show_uint32(80,16,step_t,4,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_step<100)temp_step=100;
			if(temp_step>3000)temp_step=3000;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_uint32(80,16,temp_step,4,&Font8X16,COL);
		}
	}
}
//设置PWM频率
void set_fre()
{	
	uint16_t temp_fre=fre,COL=0;
	uint16_t temp=0;
	while(1){
	switch(getK())	//支持长按操作
	{	
		case	up:
			temp_fre++;
		show_uint32(80,32,temp_fre,3,&Font8X16,COL);
		break;
		case down:		
			temp_fre--;
		show_uint32(80,32,temp_fre,3,&Font8X16,COL);
		break;	
		case mid:	//此处需要等待按键释放才能进入
			
			fre=temp_fre;
		set_frequence(fre);
		case left:	//退出，不修改
			show_uint32(80,32,fre,3,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_fre<50)temp_fre=50;
			if(temp_fre>400)temp_fre=400;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_uint32(80,32,temp_fre,3,&Font8X16,COL);
		}
	}
}
//设置波特率
void set_baud()
{	
	uint8_t temp_para=baud,COL=0;
	uint16_t *para_ptr=(uint16_t*)&baud;
	uint16_t temp=0;
	while(1){
	switch(getkey())
	{	
		case	up:
			temp_para++;
		show_uint32(70,48,baud_rate[temp_para],7,&Font8X16,COL);
		break;
		case down:		
			temp_para--;
		show_uint32(70,48,baud_rate[temp_para],7,&Font8X16,COL);
		break;	
		case mid:
			*para_ptr=(uint16_t)temp_para;
		set_USART1_Baud(baud_rate[*para_ptr]);
		case left:
			show_uint32(70,48,baud_rate[*para_ptr],7,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_para<0)temp_para=0;
			if(temp_para>12)temp_para=12;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_uint32(70,48,baud_rate[temp_para],7,&Font8X16,COL);
		}
	}
}
//设置电流比率
void set_current_ratio()
{	
	float temp_para=current_ratio;
	uint8_t	COL=0;
	float *para_ptr=&current_ratio;
	uint16_t temp=0;
	while(1){
	switch(getkey())
	{	
		case	up:
			temp_para+=0.01;
		show_float(75,0,temp_para,1,3,&Font8X16,COL);
		break;
		case down:		
			temp_para-=0.01;
		show_float(75,0,temp_para,1,3,&Font8X16,COL);
		break;	
		case mid:
			*para_ptr=temp_para;
		case left:
			show_float(75,0,*para_ptr,1,3,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_para<0)temp_para=0;
			if(temp_para>1)temp_para=1;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_float(75,0,temp_para,1,3,&Font8X16,COL);
		}
	}
}

//设置电压比率
void set_voltage_ratio()
{	
	float temp_para=voltage_ratio;
	uint8_t	COL=0;
	float *para_ptr=&voltage_ratio;
	uint16_t temp=0;
	while(1){
	switch(getkey())
	{	
		case	up:
			temp_para+=0.01;
		show_float(75,32,temp_para,1,3,&Font8X16,COL);
		break;
		case down:		
			temp_para-=0.01;
		show_float(75,32,temp_para,1,3,&Font8X16,COL);
		break;	
		case mid:
			*para_ptr=temp_para;
		case left:
			show_float(75,32,*para_ptr,1,3,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_para<0)temp_para=0;
			if(temp_para>1)temp_para=1;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_float(75,32,temp_para,1,3,&Font8X16,COL);
		}
	}
}
//void set
void set_current_offset()
{	
	uint16_t temp_para=current_offset,COL=0;
	uint16_t *para_ptr=&current_offset;
	uint16_t temp=0;
	while(1){
	switch(getK())
	{	
		case	up:
			temp_para++;
		show_uint32(75,16,temp_para,4,&Font8X16,COL);
		break;
		case down:		
			temp_para--;
		show_uint32(75,16,temp_para,4,&Font8X16,COL);
		break;	
		case mid:
			*para_ptr=temp_para;
				write_flash();
		case left:
			show_uint32(75,16,*para_ptr,4,&Font8X16,UNCOLOR);
		
			return;
	}
			if(temp_para==65535)temp_para=200;
			if(temp_para>200)temp_para=0;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_uint32(75,16,temp_para,4,&Font8X16,COL);
		}
	}
}

void set_voltage_offset()
{	
	uint16_t temp_para=voltage_offset,COL=0;
	uint16_t *para_ptr=&voltage_offset;
	uint16_t temp=0;
	while(1){
	switch(getK())
	{	
		case	up:
			temp_para++;
		show_uint32(75,48,temp_para,4,&Font8X16,COL);
		break;
		case down:		
			temp_para--;
		show_uint32(75,48,temp_para,4,&Font8X16,COL);
		break;	
		case mid:
			*para_ptr=temp_para;
		case left:
			show_uint32(75,48,*para_ptr,4,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_para==65535)temp_para=200;
			if(temp_para>200)temp_para=0;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_uint32(75,48,temp_para,4,&Font8X16,COL);
		}
	}
}

void set_lowpass_para(uint8_t para)
{	
	float temp_para=a_parameter[para];
	uint8_t	COL=0;
	float *para_ptr=&a_parameter[para];
	uint16_t temp=0;
	while(1){
	switch(getK())
	{	
		case	up:
			temp_para+=0.001;
		show_float(75,32,temp_para,1,4,&Font8X16,COL);
		break;
		case down:		
			temp_para-=0.001;
		show_float(75,32,temp_para,1,4,&Font8X16,COL);
		break;	
		case mid:
			*para_ptr=temp_para;
		case left:
			show_float(75,32,*para_ptr,1,4,&Font8X16,UNCOLOR);
			return;
	}
			if(temp_para<0)temp_para=0;
			if(temp_para>1)temp_para=1;
	temp++;
	if(temp==60000)
	{	
		temp=0;
		COL=~COL;
		show_float(75,32,temp_para,1,4,&Font8X16,COL);
		}
	}
}

void PWM_OutPut()
{
		page6();
	while(1)
	{
		switch(getkey())
		{
			case left:
				return;
		}
	}
}


void page0()
{
	OLED_Clear();
	uint8_t CN0[]={自 动 测 试 '\0'};
	show_CN(8,0,CN0,&FontCN16,COLOR);
	uint8_t CN1[]={手 动 测 试 '\0'};
	show_CN(8,16,CN1,&FontCN16,COLOR);
	uint8_t CN2[]={信 号 输 出 '\0'};
	show_CN(8,32,CN2,&FontCN16,COLOR);
	uint8_t CN3[]={系 统 设 置 '\0'};
	show_CN(8,48,CN3,&FontCN16,COLOR);
}
void page1()
{
	OLED_Clear();
	uint8_t CN0[]={步 进'\0'};
	show_CN(0,0,CN0,&FontCN16,COLOR);
	show_uint32(90,0,step,2,&Font8X16,UNCOLOR);
	
	uint8_t CN1[]={步 时'\0'};
	show_CN(0,16,CN1,&FontCN16,COLOR);
	show_uint32(90,16,step_t,4,&Font8X16,COLOR);
	show_string(80,32,"NEXT->",&Font8X16,COLOR);
}
/*
+----------------+
|U:12.34vT:1234  |
|I:12.34AP:12.34w|
|F:1234 gE:12.34/|
|R:1234 sQ:1234  |
+----------------+
*/
void page2()
{
	//OLED_Clear();
	show_string(0,0, "U:     v",&Font8X16,COLOR);
	show_string(0,16,"I:     A",&Font8X16,COLOR);
	show_string(0,32,"F:     g",&Font8X16,COLOR);
	show_string(0,48,"R:     s",&Font8X16,COLOR);
	
	show_string(64,0,"T:",&Font8X16,COLOR);
	show_string(64,16,"P:     w",&Font8X16,COLOR);
	show_string(64,32,"E:    g/w",&Font8X16,COLOR);
	show_string(64,48,"Q:    ",&Font8X16,COLOR);

}
void page3()
{	
	OLED_Clear();//校准模式，请谨慎操作
	 uint8_t CN0[]={请 手 动 校 准'\0'};
	show_CN(0,0,CN0,&FontCN16,COLOR);
	show_string(80,48,"next>>",&Font8X16,COLOR); 
	show_string(0,48,"<<",&Font8X16,COLOR);	

}
void page4()
{
	OLED_Clear();
	uint8_t CN0[]={校 准 电 调'\0'};
	show_CN(20,8,CN0,&FontCN16,COLOR);
	show_string(84,8,"?",&Font8X16,COLOR);
	show_string(80,48,"skip>>",&Font8X16,COLOR); 
	show_string(50,48,"*",&Font8X16,COLOR); 
	show_string(0,48,"<<",&Font8X16,COLOR);	
}
void page5()
{
	OLED_Clear();
	show_string(20,8,"be careful!!",&Font8X16,COLOR);
}
void page6()
{
	OLED_Clear();
	show_string(0,0,"PWM_out_mode",&Font8X16,COLOR);
	show_string(0,16,"Fre:",&Font8X16,COLOR);
	show_string(0,32,"thr:",&Font8X16,COLOR);
}
void page7()
{
	OLED_Clear();
	uint8_t CN0[]={步 进'\0'};
	show_CN(0,0,CN0,&FontCN16,COLOR);
	show_uint32(80,0,step,2,&Font8X16,COLOR);
	
	uint8_t CN1[]={步 时'\0'};
	show_CN(0,16,CN1,&FontCN16,COLOR);
	show_uint32(80,16,step_t,4,&Font8X16,COLOR);
	
	uint8_t CN2[]={频 率'\0'};
	show_CN(0,32,CN2,&FontCN16,COLOR);
	show_uint32(80,32,fre,3,&Font8X16,COLOR);
	
	uint8_t CN3[]={蓝 牙 速 率'\0'};
	show_CN(0,48,CN3,&FontCN16,COLOR);
	show_uint32(70,48,USART1_Rate,6,&Font8X16,COLOR);
	
	draw_SolidRectangle(122,0,128,64,white);
	draw_SolidRectangle(123,2,127,15,black);
}

void page8()
{
	OLED_Clear();
	uint8_t CN0[]={电 流 比 率'\0'};
	show_CN(0,0,CN0,&FontCN16,COLOR);
	show_float(75,0,current_ratio,1,3,&Font8X16,COLOR);
	
	uint8_t CN1[]={电 流 偏 移'\0'};
	show_CN(0,16,CN1,&FontCN16,COLOR);
	show_uint32(75,16,current_offset,4,&Font8X16,COLOR);
	
	uint8_t CN2[]={电 压 比 率'\0'};
	show_CN(0,32,CN2,&FontCN16,COLOR);
	show_float(75,32,voltage_ratio,1,3,&Font8X16,COLOR);
	
	uint8_t CN3[]={电 压 偏 移'\0'};
	show_CN(0,48,CN3,&FontCN16,COLOR);
	show_uint32(75,48,voltage_offset,4,&Font8X16,COLOR);
	draw_SolidRectangle(122,0,128,64,white);
	draw_SolidRectangle(123,17,127,32,black);
}
void page9()
{
	OLED_Clear();
	uint8_t CN0[]={电 流 比 率'\0'};
	show_CN(0,0,CN0,&FontCN16,COLOR);
	show_float(90,0,current_ratio,1,6,&Font8X16,COLOR);
	
	uint8_t CN1[]={电 流 偏 移'\0'};
	show_CN(0,0,CN1,&FontCN16,COLOR);
	show_uint32(90,16,current_offset,4,&Font8X16,COLOR);
	
		uint8_t CN2[]={电 压 偏 移'\0'};
	show_CN(0,48,CN2,&FontCN16,COLOR);
	show_uint32(90,48,voltage_offset,4,&Font8X16,COLOR);
	
	uint8_t CN3[]={电 压 比 率'\0'};
	show_CN(0,32,CN3,&FontCN16,COLOR);
	show_float(90,0,voltage_ratio,1,6,&Font8X16,COLOR);
	draw_SolidRectangle(122,0,128,64,white);
	draw_SolidRectangle(123,32,127,47,black);
}
void pageA()
{
	OLED_Clear();
	uint8_t CN0[]={电 流 比 率'\0'};
	show_CN(0,0,CN0,&FontCN16,COLOR);
	show_float(90,0,current_ratio,1,6,&Font8X16,COLOR);
	
	uint8_t CN1[]={电 流 偏 移'\0'};
	show_CN(0,0,CN1,&FontCN16,COLOR);
	show_uint32(90,16,current_offset,4,&Font8X16,COLOR);
	
		uint8_t CN2[]={电 压 偏 移'\0'};
	show_CN(0,48,CN2,&FontCN16,COLOR);
	show_uint32(90,48,voltage_offset,4,&Font8X16,COLOR);
	
	uint8_t CN3[]={电 压 比 率'\0'};
	show_CN(0,32,CN3,&FontCN16,COLOR);
	show_float(90,0,voltage_ratio,1,6,&Font8X16,COLOR);
	draw_SolidRectangle(122,0,128,64,white);
	draw_SolidRectangle(123,47,127,62,black);
}
	
//是否电调校准
void pageB()
{
	OLED_Clear();
	show_string(0,0,"CUR:",&Font8X16,COLOR);
	show_string(0,16,"BAT:",&Font8X16,COLOR);

}
void ESC_call()
{
	page4();
	
	while(1){
	switch(getkey())
	{	
		case	mid:
			ESC_calibration();
		page4();
		break;
		case right:
			show_data(auto_mode);
		page4();
		break;	
		case left:
			return;
		
	}
	
	}
}
/*
电调校准
*/
void ESC_calibration()
{
	uint16_t temp;
	page3();
	show_string(0,16,"T:",&Font8X16,COLOR);
	draw_SolidRectangle(0,32,127,48,white);
	while(1){
		temp=poten_Pass();
		show_uint32(16,16,temp,4,&Font8X16,COLOR);
		temp=((float)poten_Pass()/4096)*127;
		draw_SolidRectangle(2,34,temp,46,black);
		draw_SolidRectangle(temp,32,127,48,white);
		switch(getkey())
		{
			case 	right:
			case 		mid:
				show_data(auto_mode);
				page3();
			break;
			case left:
				return;
		}
	}
}

void show_data(uint8_t mode)
{	//手动模式下，需要定时发送数据
	uint16_t pwm=0;
	count_down();
	page2();
	//TIM2_inti(100);
	//TIM1_inti();
	PWM1_Enable();
	PWM1OutPut(0);
	//TIM_Cmd(TIM2, ENABLE);
	//TIM_Cmd(TIM1, ENABLE);
	while(1)
		{
			LOWPASS();
			parameter_cuc();
			if(mode)data.throttle=pwm;
			fresh_data();
		while(TIM2->SR)//产生更新时发送数据
		{
			TIM2->SR=0;
			ANO_DT_Send_RCData(data.current,data.voltage,
												 data.power,data.efficiency,
												 data.pull,data.throttle,
												 data.rmp,data.tempture0,
													data.tempture1,data.clock);
		//	ANO_DT_Send_RCData(data.pull,data.throttle,
		//	0,0,0,0,0,0,ADC_sourse[CUR],data.current);
		
		}
		
		if(mode)
		{
			while(TIM1->SR)
			{
			TIM1->SR=0;
			pwm+=(step*40);
				
			PWM1_Out_H(pwm);
			if(pwm==MAX_throttle)
				return;
			}
	}
		else
		{
		PWM1_Out_H(data.throttle);
		}
		
		switch(getK())
		{
				case left:	//此处应该执行立刻停机
				PWM1OutPut(0);
				///TIM_Cmd(TIM2, DISABLE);
				//TIM_Cmd(TIM1, DISABLE);
				return;
		}
		delay_ms(1);
	}

}
/*
数据坐标，字号，颜色，
*/
void fresh_data()
{
		show_float  (16, 0,data.voltage,    2,2,&Font8X16,COLOR);
		show_float  (16,16,data.current,    2,2,&Font8X16,COLOR);
		show_uint32 (16,32,data.pull,       4,  &Font8X16,COLOR);
		show_uint32 (16,48,data.rmp,        4,  &Font8X16,COLOR);
		show_uint32 (80, 0,data.throttle,   4,  &Font8X16,COLOR);
		show_float  (80,16,data.power,      2,2,&Font8X16,COLOR);
		show_float  (80,32,data.efficiency, 2,2,&Font8X16,COLOR);
}

//3s倒计时
void count_down()
{
	OLED_Clear();
	ShowChar(40,14,2,&Font64,COLOR);
	delay_ms(1000);
	ShowChar(40,14,1,&Font64,COLOR);
	delay_ms(1000);
	ShowChar(40,14,0,&Font64,COLOR);
	delay_ms(1000);
}
/*
滤波器设置
*/
void fliter_set()
{
	uint8_t course=0;
	pageB();
	show_float(32,0,a_parameter[a_cur],1,4,&Font8X16,UNCOLOR);
	show_float(32,16,a_parameter[a_bat],1,4,&Font8X16,COLOR);
	//TIM2_inti(100);
	TIM_Cmd(TIM2, ENABLE);
	while(1)
	{	
		
			LOWPASS();
			parameter_cuc();
		while(TIM2->SR)//产生更新时发送数据
		{
			TIM2->SR=0;
			ANO_DT_Send_RCData(ADC_Fliter[CUR],ADC_sourse[CUR],
												 ADC_Fliter[BAT],ADC_sourse[BAT],
												 data.throttle,0,0,0,
													0,data.current);
	
		}
		switch(getkey())
		{
			case down:	//使用中断处理页面，采用SYSTick
				course++;//光标++
			if(course==2)course=0;//光标循环
			show_float(32,0,a_parameter[a_cur],1,4,&Font8X16,!course);
			show_float(32,16,a_parameter[a_bat],1,4,&Font8X16,course);
			break;
			case up:
			course--;
			if(course>1)course=1;
			show_float(32,0,a_parameter[a_cur],1,4,&Font8X16,!course);
			show_float(32,16,a_parameter[a_bat],1,4,&Font8X16,course);
		break;
			case mid:
			set_lowpass_para((!course)+a_bat);
			//while(key_vaule()!=no_key)delay_ms(10);//等待按键释放
			break; 
			
			case left:
				return;
			
	}
		delay_ms(1);
	
}

}

#endif	/*use_oled*/
#endif	/*UI_C*/
