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



//���еĹ��ܿ�����dispatcher.h��

//OLED Ҫ��ӡ�����������ݣ��������������ʾ��������
//ʾ����ģʽ��ѡ���ź�Դ��ѡ��DIV��
uint8_t paint[1024]={0};	//7��128��
extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
extern uint8_t key_vaule_buff;
extern uint16_t current_offset;//������ƫ����
extern all_data data;
void  sys_init(void);

int main()
{	
	
#ifdef USE_FLASH
	uint16_t temp;
	FLASH_Unlock();	//����flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR 
									| FLASH_FLAG_WRPRTERR);	
	
	temp=(*(__IO  uint32_t*)START_ADDR);///ǿ��ת��Ϊ��ַ
	if(temp==first_key)//�ж�Ϊ��һ�����н����һ�γ�ʼ��
		read_flash();//��FLASH �ж�ȡ����
	else
		flash_init();//��Ĭ������д��FLASH��
	
#endif	/*USE_FLASH*/
	
	dispatcher_set();//72M��Ƶ�� 1000hz
	
	ADCx_Init();
	HX711_SET();
	PWM_SET();
	beep_set();		
	Low_Pass_init();
	HX711set_to_0();//HX711����
	OLED_Init();
	UIinit();
	REV_Config();
	USART_config();
	TaskInit();
	MLX90614_Init();
    
    for(u16 i=5000;i;i--)
    {
     LOWPASS();
     parameter_cuc();
    }
	sys_init();
   
	while(1)
	{
		dispatcherMain();
        
	}
		return 0;	
}


void  sys_init()
{	
	
	u8 bat_cells	=	 bettery_cells_check();
    OLED_Refresh();
    for(u8 i=bat_cells;i;i--)
    {
    set_beep_fre_vol(1000,10);
    delay_ms(200);
    set_beep_fre_vol(0,10);
    delay_ms(100);
    }
    
    set_beep_fre_vol(4000,10);
    delay_ms(600);
    set_beep_fre_vol(0,10);
    SYSUI.Page=1;  //1.5s�󿪻���ת
	
}























