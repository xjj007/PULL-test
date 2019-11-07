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

//���еĹ��ܿ�����dispatcher.h��
//�ڴ治̫����
//Ҫ��Ȼ�Ӹ�flash

uint8_t paint[1024]={0};	//7��128��
extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
extern uint8_t key_vaule_buff;
extern uint16_t current_offset;//������ƫ����

int main()
{	
	
#ifdef USE_FLASH
	uint16_t temp;
	//FLASH_Status FLASHStatus = FLASH_COMPLETE;
	FLASH_Unlock();	//����flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR 
									| FLASH_FLAG_WRPRTERR);	
	
	temp=(*(__IO  uint32_t*)START_ADDR);///ǿ��ת��Ϊ��ַ
	if(temp==first_key)//�ж�Ϊ��һ�����н����һ�γ�ʼ��
		read_flash();//��FLASH �ж�ȡ����
	else
		flash_init();//��Ĭ������д��FLASH��
	
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
	USART_config();//���ô���
	dispatcher_set();//72M��Ƶ�� 1000hz
	TaskInit();
	
	MLX90614_Init();
	
	

	
	while(1)
	{
		dispatcherMain();
	}
		return 0;
	
	
}


