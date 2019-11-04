#ifndef	UI_H
#define	UI_H
#include	"stm32f10x.h"



#define MAX_throttle 4000

/*
屏幕刷新使用计划任务 15HZ或者30HZ
*/
#define	OLED_FPS	1	//OLED刷新率



#define	USE_OLED	//这样写有什么意义
#ifdef	USE_OLED

/*
参数表
*/
enum Parameters_table
{
	e_step,		//uint16_t
	e_step_t,	//uint16_t
	e_fre,		//uint16_t 固定值
	e_buad,		//uint16_t
	e_cur_ratio,//float
	e_cur_offset,//uint16_t
	e_vol_ratio,//float
	e_vol_offset,//uint16_t
	
};

enum mode
{
	hand_mode	=	0,
	auto_mode	=	1,
	
};

void UIinit(void);
void power_on(void);
void show_function(void);
void auto_Tension(void);
void hand_Tension(void);
void PWM_OutPut(void);
void Sys_set(void);
void page0(void);
void page1(void);
void page2(void);
void page3(void);
void page4(void);
void page5(void);
void page6(void);
void page7(void);
void page8(void);
void page9(void);
void pageA(void);
void set_PwmStep(void);
void set_step_t(void);
void set_fre(void);
void set_baud(void);
void set_current_ratio(void);
void set_current_offset(void);
void set_voltage_offset(void);
void set_voltage_ratio(void);
void ESC_call(void);
void ESC_calibration(void);
void show_data(uint8_t mode);
void count_down(void);
void set_Parameters(uint8_t Parameters);
void fresh_cursor(uint8_t cursor);
void fresh_data(void);

void OLED_Refresh(void);
#endif

#ifdef USE_EDP
void START_CLEAR(const unsigned char* EPD_BUFF);
void power_on(const unsigned char* EPD_BUFF);
void show_function_select(const unsigned char* EPD_BUFF);
void show_status_bar(void);
int auto_Tension(const unsigned char* EPD_BUFF);
void hand_Tension(void);
void PWM_OutPut(void);
void system_set(void);
void wave_selct(void);
void fliter_set(void);

#endif	/*USE_EDP*/

#endif	/*UI_H
*/
