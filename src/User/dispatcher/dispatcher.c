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

#define	USE_DSIPATCHER

/*FPS�����ms*/
#define	FPS(x)	(1000/x)	//���Ӹ�������Ҳ����



uint8_t key_vaule_buff;
extern uint8_t key_flag;
extern uint8_t HX711_FLAG;//���ڶ�ȡʱΪ0
extern output_data	out_data;

extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
volatile uint64_t sys_time=0;//ϵͳ��ʱ��
extern uint32_t set_zero;
extern float GapValue;
extern uint16_t step_t;
extern  all_data data;

extern uint16_t HX_weight;

extern uint16_t rmp;

uint32_t sys_cycle=0;
uint32_t task_time[10];//����ȴ�ʱ��
uint32_t	task_tim[10];//����ʱʱ��
uint8_t tast_counter=0;//�����������


/*
������䣺
����������Ŀǰ��֧�֣�
����ɨ������
���ڷ�������
״̬������Ŀǰ��֧�֣�
���������ݲɼ�
	ѹ��������
	�¶ȴ�����
	ת�ٴ�����
��һ�������⣬���ǲ���ϣ���˲����Զ����ٶ����У�����ϣ��
�ܾ����ܿ�����У�
*/
struct					//�˽ṹ����ʱ�䵥λΪms
{
	u32 delay;			//ms����ʱ��us����ʱ��Ҫʹ��ר�ö�ʱ��
	
	u32 OledRefreshTaskRemainTime;		//����ʣ��ʱ��
	u32 OLedRefreshTaskPerTime;			//����Ԥ��ʱ��
	void (*OledRefreshTask)(void);		//����ָ��
	
	/*����ɨ��*/
	u32 KeyScanRemain;
	u32 KeyScanPer;
	void (*KeyScanTask)(void);
	
	u32 USARTransmitRemain;
	u32 USARTranmitPer;
	void (*USARTranmit)(void);
	
	u32 TemperautreRemain;		//����Ҫ����ָ�룬ֱ�ӵ���
	u32 TemperaturePer;
	
	
	u32 FliterFPS;				//�˲���֡�ʼ���
	u32 FliterFPSCounter;		//

}Tick;

/*��ʱ����������*/
void TaskInit()
{	
	Tick.delay=0;
	Tick.OLedRefreshTaskPerTime	=	FPS(OLED_FPS);
	Tick.OledRefreshTaskRemainTime	=	Tick.OLedRefreshTaskPerTime;
	Tick.OledRefreshTask	=	&OLED_Refresh;
	
	Tick.KeyScanPer	=	FPS(1);//100hz����ɨ���ٶ�
	Tick.KeyScanRemain	=	Tick.KeyScanPer;
	Tick.KeyScanTask	=	&ScanKey;
	
	Tick.USARTranmitPer	=	FPS(500);//100hzˢ���ٶ�
	Tick.USARTransmitRemain	=	Tick.USARTranmitPer;
	Tick.USARTranmit	=	&ANO_TransmitCallBack;
	
	Tick.TemperaturePer	=	FPS(10);
	Tick.TemperautreRemain=Tick.TemperaturePer;
	
	Tick.FliterFPS	=	0;
	Tick.FliterFPSCounter=0;

}


#ifdef USE_DSIPATCHER
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
										SysTick_CTRL_CLKSOURCE_Msk|							//	AHBʱ��
										SysTick_CTRL_ENABLE_Msk;					
  return 0;       																						/* Function successful */
}

//���¼���
void dispatcher_set()
{	
	SysTickConfig(72000);//72M��Ƶ�� 1000hz
}
/*
���ȳ���
ע�⣺��Ҫ�������ִ�к��������ж��У����������Ӱ���жϵ�ִ���ٶ�
����ж����
ͨ�������ִ�з�����ѭ����
*/
void dispatcher(void)
{		
	ANO.clock++;//Ӧ�ø��������������ʽ������
	
	/*ÿ100ms���µ����ת��*/
	if(ANO.clock%100==0)
	{
	rmp	=	get_rmp()*10/2;//����㷨��Ҫ�Ż�
	}
	if(ANO.clock==1000)
	{	
		
		ANO.clock=0;
		Tick.FliterFPS= Tick.FliterFPSCounter;
		Tick.FliterFPSCounter	=	0;
	}
	if(Tick.delay>0 )
		Tick.delay--;
	
	/*ˢ���¶�*/
	if(Tick.TemperautreRemain >0)Tick.TemperautreRemain--;
	else Tick.TemperautreRemain=Tick.TemperaturePer;
	
	if(Tick.OledRefreshTaskRemainTime>0) Tick.OledRefreshTaskRemainTime--;
	else
	Tick.OledRefreshTaskRemainTime=Tick.OLedRefreshTaskPerTime;
	
	
	if(Tick.USARTransmitRemain>0)Tick.USARTransmitRemain--;
	else
		Tick.USARTransmitRemain=Tick.USARTranmitPer;
	
}

/*
��������������ŭ�ᱻ����
�˲����������ܵĶ࣬���ݲ�����
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
	
	if(Tick.TemperautreRemain==Tick.TemperaturePer)
	{
		data.tempture0	=	Read_MLX_IIC_Data(0x07)*0.02-273.15; //Զ���¶�
		data.tempture1	=	Read_MLX_IIC_Data(0x06)*0.02-273.15;	//̽ͷ�����¶�
	}
	
	if(Tick.OledRefreshTaskRemainTime==Tick.OLedRefreshTaskPerTime)
	(*Tick.OledRefreshTask)();
	
	if(Tick.USARTransmitRemain==Tick.USARTranmitPer )
		(*Tick.USARTranmit)();	//��������
	
	/*
	��������3�����ݰ�������200ms�����ж�
	*/
	
	
	
	
}

void delay_ms(uint32_t ms)
{
	Tick.delay	=	ms;

	while(Tick.delay);
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


