#ifndef	DISPATCHER_H
#define	DISPATCHER_H

#include	"dispatcher.h"
#include	"fliter.h"
#include	"ADC.h"
#include	"HX711.H"
#include	"USART_CONFIG.H"
#include	"UI.H"


#define	USE_DSIPATCHER

/*FPS换算成ms*/
#define	FPS(x)	(1000/x)	//不加浮点问题也不大
/*
调度函数
*/

uint8_t key_vaule_buff;
extern uint8_t key_flag;
extern uint8_t HX711_FLAG;//正在读取时为0
extern output_data	out_data;

extern uint16_t ADC_sourse[end];
extern uint16_t ADC_Fliter[end];
volatile uint64_t sys_time=0;//系统总时间
extern uint32_t set_zero;
extern float GapValue;
extern uint16_t step_t;
extern  all_data data;


uint32_t sys_cycle=0;
uint32_t task_time[10];//任务等待时间
uint32_t	task_tim[10];//任务定时时间
uint8_t tast_counter=0;//任务计数器，


/*
任务分配：
蜂鸣器任务（目前不支持）
按键扫描任务
串口发送任务
状态灯任务（目前不支持）
传感器数据采集
	压力传感器
	温度传感器
	转速传感器
有一个新问题，我们并不希望滤波器以多块的速度运行，我们希望
能尽可能快的运行，
*/
struct					//此结构体下时间单位为ms
{
	u32 delay;			//ms级延时，us级延时需要使用专用定时器
	
	u32 OledRefreshTaskRemainTime;		//任务剩余时间
	u32 OLedRefreshTaskPerTime;			//任务预设时间
	void (*OledRefreshTask)(void);		//函数指针
	
	/*按键扫描*/
	u32 KeyScanRemain;
	u32 KeyScanPer;
	void (*KeyScanTask)(void);
	
	u32 USARTransmitRemain;
	u32 USARTranmitPer;
	void (*USARTranmit)(void);
	
	u32 FliterFPS;		//滤波器帧率计算
	u32 FliterFPSCounter;	//

}Tick;

/*定时器任务设置*/
void TaskInit()
{	
	Tick.delay=0;
	Tick.OLedRefreshTaskPerTime	=	FPS(OLED_FPS);
	Tick.OledRefreshTaskRemainTime	=	Tick.OLedRefreshTaskPerTime;
	Tick.OledRefreshTask	=	&OLED_Refresh;
	
	Tick.KeyScanPer	=	FPS(100);//100hz按键扫描速度
	Tick.KeyScanRemain	=	Tick.KeyScanPer;
	Tick.KeyScanTask	=	&ScanKey;
	
	Tick.USARTranmitPer	=	FPS(100);//100hz刷新速度
	Tick.USARTransmitRemain	=	Tick.USARTranmitPer;
	Tick.USARTranmit	=	&ANO_TransmitCallBack;
	
	Tick.FliterFPS	=	0;
	Tick.FliterFPSCounter=0;

}


#ifdef USE_DSIPATCHER
/*
SYSTick配置
*/
static  uint32_t SysTickConfig(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  =   SysTick_CTRL_TICKINT_Msk   |											//启动sysTick
					 SysTick_CTRL_CLKSOURCE_Msk |				//	AHB时钟
					 SysTick_CTRL_ENABLE_Msk;					
  return 0;                                                  /* Function successful */
}

//向下计数
void dispatcher_set()
{	
	SysTickConfig(72000);//72M主频下 1000hz
}
/*
调度程序
注意：不要将具体的执行函数放在中断中，否则会严重影响中断的执行速度
造成中断溢出
通用任务的执行放在主循环中
*/
void dispatcher(void)
{		
	ANO.clock++;//应该赋予这个变量更正式的名字
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
任务触发过快就会被无视
滤波次数尽可能的多，数据才能稳
*/
void dispatcherMain()
{	
	/*
	滤波器无条件运行
	*/
	
	Tick.FliterFPSCounter++;
	LOWPASS();
	parameter_cuc();
	if((Read_DOUT==RESET)&&HX711_FLAG)//HX711准备好，且未开始读取
	ADC_Fliter[weight]=get_wetght();
	
	PWM1_Out_H(data.throttle);
	if(Tick.OledRefreshTaskRemainTime==Tick.OLedRefreshTaskPerTime)
	(*Tick.OledRefreshTask)();
	if(Tick.USARTransmitRemain==Tick.USARTranmitPer )
		(*Tick.USARTranmit)();	//发送数据
	
	
	
	
	
}

void delay_ms(uint32_t ms)
{
	Tick.delay	=	ms;
	while(Tick.delay);
}


#endif
//显示屏刷新任务，300ms执行一次
void EDP_TASK()
{
	;
}	
//最长定时时间1.3s
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


/*配置定时器*/
void TIM2_inti(uint16_t peroi)//串口 发送专用定时器
{	
	/*发送一次数据需要1.28ms，加上滤波数据采集，最小时间5ms
	最大时间间隔*/
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	
	if(peroi<50||peroi>1000)
		return;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_Base_SET.TIM_Prescaler	=	7200;//时钟频率1M
	//TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	peroi*10;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM2,&TIM_Base_SET);
	
	TIM_Cmd(TIM2, DISABLE);
}
void TIM1_inti()//PWM步进定时
{	/*发送一次数据需要1.28ms，加上滤波数据采集，最小时间5ms
	最大时间间隔*/
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	
	TIM_Base_SET.TIM_Prescaler	=	7200;//时钟频率1M
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	step_t*10;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM1,&TIM_Base_SET);
	
	TIM_Cmd(TIM1, DISABLE);
}


#endif	/*DISPATCHER_H*/


