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

//���еĹ��ܿ�����dispatcher.h��
/*
����ϣ��ÿһ��ҳ�涼�����ܵ�����ͬ�Ĳ����߼�
ͬʱ�����ǻ�ϣ��ÿ��ҳ�������Լ�������
�Զ�����
�ֶ�����
�������
��������

����ӷ�����֧��


����Ҫ�Ĵ���ֱ��ɾ���ͺ���
����ͨ������ҳ�����ã�������Ĭ����������ȷ�Ŀ���ʹ�õ�

*/

uint8_t paint[1024]={0};//7��128��
extern uint16_t ADC_sourse[end];
extern uint16_t ADC_Fliter[end];
extern uint8_t key_vaule_buff;
extern uint16_t current_offset;//������ƫ����

int main()
{	
	
#ifdef USE_FLASH
	uint16_t temp;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	FLASH_Unlock();	//����flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR 
									| FLASH_FLAG_WRPRTERR);	
	
	temp=(*(__IO  uint32_t*)START_ADDR);///ǿ��ת��Ϊ��ַ
	if(temp==first_key)//�ж�Ϊ��һ�����н����һ�γ�ʼ��
		read_flash();//��FLASH �ж�ȡ����
	else
		flash_init();//��Ĭ������д��FLASH��
	
#endif	/*USE_FLASH*/
	
	
	beep_set();
	ADCx_Init();
	HX711_SET();
	PWM_SET();
	Low_Pass_init();
	HX711set_to_0();
	OLED_Init();//Ӳ����ʼ��7
	UIinit();
	REV_Config();

//	power_on();//��ʾ����ҳ��
//	delay_ms(800);
	USART_config();//���ô���
	//fliter_set();
	dispatcher_set();//72M��Ƶ�� 1000hz
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


