#ifndef	DISPATCHER_H
#define	DISPATCHER_H

#include	"dispatcher.h"
#include	"fliter.h"
#include	"ADC.h"
#include	"HX711.H"
#include	"USART_CONFIG.H"
#include	"UI.H"
#include	"rev.h"
#include	"temperature.h"
#include    "beep.h"

#define	USE_DSIPATCHER

/*FPS�����ms*/
#define	FPS(x)	(1000/x)

uint8_t key_vaule_buff;
extern uint8_t key_flag;
extern uint8_t HX711_FLAG;  //���ڶ�ȡʱΪ0
extern output_data	out_data;

extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
volatile uint64_t sys_time=0;//ϵͳ��ʱ��
extern uint32_t set_zero;
extern float GapValue;
extern uint16_t step_t;
extern all_data data;
extern uint16_t HX_weight;
extern uint16_t rmp;

/*
OLED.C
OLEDˢ�±�־
*/
extern bool oledFresh;	

/*
������䣺
����������
����ɨ������
���ڷ�������
״̬������Ŀǰ��֧�֣�
���������ݲɼ�
	ѹ��������
	�¶ȴ�����
	ת�ٴ�����

*/
struct					//�˽ṹ����ʱ�䵥λΪms
{
	volatile int delay;			//ms����ʱ��us����ʱ��Ҫʹ��ר�ö�ʱ��
	volatile u32 OledRefreshTaskRemainTime;		//����ʣ��ʱ��
	volatile u32 OLedRefreshTaskPerTime;			//����Ԥ��ʱ��
	volatile void (*OledRefreshTask)(void);		//����ָ��
	volatile bool OLED_Trig;
    
	/*����ɨ��*/
	volatile u32 KeyScanRemain;
	volatile u32 KeyScanPer;
	volatile void (*KeyScanTask)(void);
    volatile bool KeyScan_Trig;
	
	volatile u32 USARTransmitRemain;
	volatile u32 USARTranmitPer;
	volatile void (*USARTranmit)(void);
	volatile bool USARTransmin_Trig;
    
	volatile u32 TemperautreRemain;              //����Ҫ����ָ�룬ֱ�ӵ���
	volatile u32 TemperaturePer;
	volatile bool Temperature_Trig;
	
	volatile u32 FliterFPS;				//�˲���֡�ʼ���
	volatile u32 FliterFPSCounter;		//

}Tick;
static  uint32_t SysTickConfig(uint32_t ticks);

/*��ʱ����������*/
void TaskInit()
{	
	Tick.delay=0;
	Tick.OLedRefreshTaskPerTime	    =	FPS(OLED_FPS);
	Tick.OledRefreshTaskRemainTime	=	Tick.OLedRefreshTaskPerTime;
	Tick.OledRefreshTask	        =	&OLED_Refresh;
	Tick.OLED_Trig                  =   false;
    
	Tick.KeyScanPer	    =	FPS(100);//����ɨ���ٶ�
	Tick.KeyScanRemain	=	Tick.KeyScanPer;
	Tick.KeyScanTask	=	&ScanKey;
	Tick.KeyScan_Trig   =   false;
    
	Tick.USARTranmitPer	    =	FPS(100);//ˢ���ٶ�
	Tick.USARTransmitRemain	=	Tick.USARTranmitPer;
	Tick.USARTranmit	    =	&ANO_TransmitCallBack;
	Tick.USARTransmin_Trig  =   false;
    
	Tick.TemperaturePer	    =	FPS(10);
	Tick.TemperautreRemain  =   Tick.TemperaturePer;
	Tick.Temperature_Trig   =   false;
    
	Tick.FliterFPS	        =	0;//�˲����˲�Ƶ�ʲ������
	Tick.FliterFPSCounter   =   0;//�˲����˲�����������
    
}


#ifdef USE_DSIPATCHER

//���¼���
void dispatcher_set()
{	
	SysTickConfig(72000);//72M��Ƶ�� 1000hz
}
/*
���ȳ���
ע�⣺��Ҫ�������ִ�к��������ж��У����������Ӱ���жϵ�ִ���ٶ�
�˺����ڵ�����������Ҫ��1ms��ִ�����
����ж����
ͨ�������ִ�з�����ѭ����
*/
void dispatcher(void)
{		
	ANO.clock++;//Ӧ�ø��������������ʽ������
	sys_time++;//ϵͳʱ�䣨ms��
	
    /*ÿ100ms���µ����ת��*/
	if(ANO.clock%100==0)
	{
	rmp	=	get_rmp()*10/2;//����㷨��Ҫ�Ż�
	}
	
    if(ANO.clock==1000)//֡������
	{		
		ANO.clock=0;
		Tick.FliterFPS= Tick.FliterFPSCounter;
		Tick.FliterFPSCounter	=	0;
	}
	if(Tick.delay)
     Tick.delay--;
	/*ˢ���¶�*/
    
	if(Tick.TemperautreRemain >0)Tick.TemperautreRemain--;
    
	else   
        {
            Tick.TemperautreRemain=Tick.TemperaturePer;
            Tick.Temperature_Trig   =   true;
        }	
	if(Tick.OledRefreshTaskRemainTime>0) Tick.OledRefreshTaskRemainTime--;
	else
    {
        Tick.OledRefreshTaskRemainTime=Tick.OLedRefreshTaskPerTime;
        Tick.OLED_Trig  =   true;
	}
	if(Tick.USARTransmitRemain>0)Tick.USARTransmitRemain--;
	else
    {
		Tick.USARTransmitRemain=Tick.USARTranmitPer;
        Tick.USARTransmin_Trig  =   true;
    }
    if(Tick.KeyScanRemain>0)Tick.KeyScanRemain--;
    else 
    {    
        Tick.KeyScanRemain=Tick.KeyScanPer;
        Tick.KeyScan_Trig   =   true;
    }
}

/*
������������Task�ᱻ����
�˲����������ܵĶ࣬���ݲ�����
��������ִ�б�־ֻ��1ms ������ʱ��
*/
void dispatcherMain()
{	
	/*
	�˲�������������
	*/
	
	Tick.FliterFPSCounter++;
	LOWPASS();
	parameter_cuc();
	
	if((Read_DOUT==RESET)&&HX711_FLAG)//HX711׼���ã���δ��ʼ��ȡ
	HX_weight=get_wetght();
	
	PWM1_Out_H(data.throttle);
    
    
	if(Tick.KeyScan_Trig)
    {   
        Tick.KeyScan_Trig   =   false;
        (*Tick.KeyScanTask)();
    }
    
	if(Tick.Temperature_Trig)
	{   Tick.Temperature_Trig   =   false;
		data.tempture0	=	Read_MLX_IIC_Data(0x07)*0.02-273.15;    //Զ���¶�
		data.tempture1	=	Read_MLX_IIC_Data(0x06)*0.02-273.15;	//̽ͷ�����¶�
	}
	
	if(oledFresh && Tick.OLED_Trig)
    {   
        Tick.OLED_Trig  =   false;
        (*Tick.OledRefreshTask)();
	}
    
	if(Tick.USARTransmin_Trig )
    {
        Tick.USARTransmin_Trig  =   false;
		(*Tick.USARTranmit)();	//��������
    }

/*
    
    */
      set_beep_fre_vol((key_vaule(Average_Result[A_KEY]))*2000,10);	
	
}




void delay_ms(uint32_t ms)
{
	Tick.delay	=	ms;
	while(Tick.delay);
}

/*
SYSTick����
*/
static  uint32_t SysTickConfig(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  =		SysTick_CTRL_TICKINT_Msk   |							//����sysTick
						SysTick_CTRL_CLKSOURCE_Msk |							//	AHBʱ��
						SysTick_CTRL_ENABLE_Msk;					
  return 0;       																						/* Function successful */
}

#endif

//���ʱʱ��1.3s
#if	0//USE_TIM6_DELAY

void delay_us(uint32_t us)
{
	SysTickConfig(9*us);
	while(!((SysTick->CTRL) & (1<<16))==1 )
	{	
	if(key_vaule_buff==no_key)
	key_vaule_buff=key_vaule();
	
		if((!Read_DOUT)&&HX711_FLAG)
		ADC_sourse[weight]=read_HX711();
			
		LOWPASS();
	}
}

void delay_ms(uint32_t ms)
{
	delay_us(ms*1000);
}
#endif	/*USE_TIM6_DELAY*/

#endif	/*DISPATCHER_H*/


