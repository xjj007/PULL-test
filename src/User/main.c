#include  "stm32f10x.h"   
#include  "dispatcher.h"
#include  "UI.H"
#include  "port.h"
#include  "E-INK-1.54.h"
#include  "epdif.h"
#include  "E-INK-Display.h"
#include  "IMAGE.h"
#include  <stdlib.h>
#include  "ADC.H"
#include  "HX711.H"
#include  "delay.h"
#include  "oled.h"
#include  "USART_CONFIG.h"
#include  "fliter.h"
#include  "PWM.H"
#include  "flash.h"
#include  "rev.h"
#include  "temperature.h"
#include  "beep.h"

/*

*/
/*
电池保护策略：
只低压报警，不干扰输出
*/

/*
过热保护策略：
两个阈值：
第一个阈值降低到当前输出的50%

第二个阈值立即关闭动力

*/

/*
过流保护策略：


电流因为其均值滤波比较大，所以响应会慢，只能作为一个击穿后的
防止继续短路导致燃烧
事实上如果真的已经发生了电调烧毁或者电机烧毁
过流保护是没有用的，单片机无法关断主供电
因此不设计电流保护
*/

//所有的功能开关在dispatcher.h里
//内存不太够啊
//要不然加个flash
//OLED 要打印出完整的数据，并且最好能做个示波器出来
//示波器模式：选择信号源，选择DIV，
uint8_t paint[1024]={0};	//7行128列
extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
extern uint8_t key_vaule_buff;
extern uint16_t current_offset;//电流计偏移量
extern all_data data;
int main()
{	
	
#ifdef USE_FLASH
	uint16_t temp;
	FLASH_Unlock();	//解锁flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR 
									| FLASH_FLAG_WRPRTERR);	
	
	temp=(*(__IO  uint32_t*)START_ADDR);///强制转化为地址
	if(temp==first_key)//判断为第一次运行进入第一次初始化
		read_flash();//从FLASH 中读取数据
	else
		flash_init();//将默认数据写入FLASH中
	
#endif	/*USE_FLASH*/
	
	dispatcher_set();//72M主频下 1000hz
	
	ADCx_Init();
	HX711_SET();
	PWM_SET();
	beep_set();		
	Low_Pass_init();
	HX711set_to_0();//HX711清零
	OLED_Init();
	UIinit();
	REV_Config();
	USART_config();
	TaskInit();
	MLX90614_Init();
	
	/*现在OLED不显示*/
	
	while(1)
	{
		dispatcherMain();
        
	}
		return 0;	
}


void  sys_init()
{	
	
	u8 bat_cells	=	 bettery_cells_check();
	
}

