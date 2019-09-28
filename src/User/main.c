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

//所有的功能开关在dispatcher.h里
/*
我们希望每一个页面都尽可能的有相同的操作逻辑
同时，我们还希望每个页面能有自己的坐标
自动测试
手动测试
舵机测试
参数矫正

先添加蜂鸣器支持


不需要的代码直接删除就好了
蓝牙通过设置页面配置，开机后默认蓝牙是正确的可以使用的

*/

uint8_t paint[1024]={0};//7行128列
extern uint16_t ADC_sourse[end];
extern uint16_t ADC_Fliter[end];
extern uint8_t key_vaule_buff;
extern uint16_t current_offset;//电流计偏移量

int main()
{	
	
#ifdef USE_FLASH
	uint16_t temp;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	FLASH_Unlock();	//解锁flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR 
									| FLASH_FLAG_WRPRTERR);	
	
	temp=(*(__IO  uint32_t*)START_ADDR);///强制转化为地址
	if(temp==first_key)//判断为第一次运行进入第一次初始化
		read_flash();//从FLASH 中读取数据
	else
		flash_init();//将默认数据写入FLASH中
	
#endif	/*USE_FLASH*/
	
	
	beep_set();
	ADCx_Init();
	HX711_SET();
	PWM_SET();
	Low_Pass_init();
	HX711set_to_0();
	OLED_Init();//硬件初始化7
	UIinit();
	REV_Config();

//	power_on();//显示开机页面
//	delay_ms(800);
	USART_config();//配置串口
	//fliter_set();
	dispatcher_set();//72M主频下 1000hz
	TaskInit();
	
	
#ifdef PWM_TEST
	//PWM1_Enable();
	PWM1OutPut(50);
	while(1);
#endif	/*PWM_TEST*/

#ifdef KEY_TEST
	if(KEY_Pass()>600)
			{
			delay_ms(10);
				key_vaule_buff=key_vaule();
			}  
			else
				key_vaule_buff=0;
#endif	/*KEY_TEST*/
	
	while(1)
	{
		dispatcherMain();
	}
		return 0;
	
	
}


