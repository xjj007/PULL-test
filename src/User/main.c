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

//所有的功能开关在dispatcher.h里
//内存不太够啊
//要不然加个flash

uint8_t paint[1024]={0};	//7行128列
extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
extern uint8_t key_vaule_buff;
extern uint16_t current_offset;//电流计偏移量

int main()
{	
	
#ifdef USE_FLASH
	uint16_t temp;
	//FLASH_Status FLASHStatus = FLASH_COMPLETE;
	FLASH_Unlock();	//解锁flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR 
									| FLASH_FLAG_WRPRTERR);	
	
	temp=(*(__IO  uint32_t*)START_ADDR);///强制转化为地址
	if(temp==first_key)//判断为第一次运行进入第一次初始化
		read_flash();//从FLASH 中读取数据
	else
		flash_init();//将默认数据写入FLASH中
	
#endif	/*USE_FLASH*/
	
	
	
	ADCx_Init();
	HX711_SET();
	PWM_SET();
	beep_set();		
	Low_Pass_init();
	HX711set_to_0();
	OLED_Init();
	UIinit();
	REV_Config();
	USART_config();//配置串口
	dispatcher_set();//72M主频下 1000hz
	TaskInit();
	
	MLX90614_Init();
	
	

	
	while(1)
	{
		dispatcherMain();
	}
		return 0;
	
	
}


