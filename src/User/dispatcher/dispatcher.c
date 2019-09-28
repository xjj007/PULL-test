#ifndef	DISPATCHER_H
#define	DISPATCHER_H

#include	"dispatcher.h"
#include	"fliter.h"
#include	"ADC.h"
#include	"HX711.H"
#include	"USART_CONFIG.H"
#include	"UI.H"


#define	USE_DSIPATCHER

/*FPS�����ms*/
#define	FPS(x)	(1000/x)	//���Ӹ�������Ҳ����
/*
���Ⱥ���
*/

uint8_t key_vaule_buff;
extern uint8_t key_flag;
extern uint8_t HX711_FLAG;//���ڶ�ȡʱΪ0
extern output_data	out_data;

extern uint16_t ADC_sourse[end];
extern uint16_t ADC_Fliter[end];
volatile uint64_t sys_time=0;//ϵͳ��ʱ��
extern uint32_t set_zero;
extern float GapValue;
extern uint16_t step_t;
extern  all_data data;


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
	
	u32 FliterFPS;		//�˲���֡�ʼ���
	u32 FliterFPSCounter;	//

}Tick;

/*��ʱ����������*/
void TaskInit()
{	
	Tick.delay=0;
	Tick.OLedRefreshTaskPerTime	=	FPS(OLED_FPS);
	Tick.OledRefreshTaskRemainTime	=	Tick.OLedRefreshTaskPerTime;
	Tick.OledRefreshTask	=	&OLED_Refresh;
	
	Tick.KeyScanPer	=	FPS(100);//100hz����ɨ���ٶ�
	Tick.KeyScanRemain	=	Tick.KeyScanPer;
	Tick.KeyScanTask	=	&ScanKey;
	
	Tick.USARTranmitPer	=	FPS(100);//100hzˢ���ٶ�
	Tick.USARTransmitRemain	=	Tick.USARTranmitPer;
	Tick.USARTranmit	=	&ANO_TransmitCallBack;
	
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
  SysTick->CTRL  =   SysTick_CTRL_TICKINT_Msk   |											//����sysTick
					 SysTick_CTRL_CLKSOURCE_Msk |				//	AHBʱ��
					 SysTick_CTRL_ENABLE_Msk;					
  return 0;                                                  /* Function successful */
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
	if(ANO.clock==1000)
	{
		ANO.clock=0;
		Tick.FliterFPS= Tick.FliterFPSCounter;
		Tick.FliterFPSCounter	=	0;
	}
	if(Tick.delay>0 )Tick.delay--;
	
	if(Tick.OledRefreshTaskRemainTime>0) Tick.OledRefreshTaskRemainTime--;
	else
	Tick.OledRefreshTaskRemainTime=Tick.OLedRefreshTaskPerTime;
	
	
	if(Tick.USARTransmitRemain>0)Tick.USARTransmitRemain--;
	else
		Tick.USARTransmitRemain=Tick.USARTranmitPer;
	
}

/*
���񴥷�����ͻᱻ����
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
	ADC_Fliter[weight]=get_wetght();
	
	PWM1_Out_H(data.throttle);
	if(Tick.OledRefreshTaskRemainTime==Tick.OLedRefreshTaskPerTime)
	(*Tick.OledRefreshTask)();
	if(Tick.USARTransmitRemain==Tick.USARTranmitPer )
		(*Tick.USARTranmit)();	//��������
	
	
	
	
	
}

void delay_ms(uint32_t ms)
{
	Tick.delay	=	ms;
	while(Tick.delay);
}


#endif
//��ʾ��ˢ������300msִ��һ��
void EDP_TASK()
{
	;
}	
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


/*���ö�ʱ��*/
void TIM2_inti(uint16_t peroi)//���� ����ר�ö�ʱ��
{	
	/*����һ��������Ҫ1.28ms�������˲����ݲɼ�����Сʱ��5ms
	���ʱ����*/
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	
	if(peroi<50||peroi>1000)
		return;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_Base_SET.TIM_Prescaler	=	7200;//ʱ��Ƶ��1M
	//TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	peroi*10;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM2,&TIM_Base_SET);
	
	TIM_Cmd(TIM2, DISABLE);
}
void TIM1_inti()//PWM������ʱ
{	/*����һ��������Ҫ1.28ms�������˲����ݲɼ�����Сʱ��5ms
	���ʱ����*/
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	
	TIM_Base_SET.TIM_Prescaler	=	7200;//ʱ��Ƶ��1M
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	step_t*10;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM1,&TIM_Base_SET);
	
	TIM_Cmd(TIM1, DISABLE);
}


#endif	/*DISPATCHER_H*/


