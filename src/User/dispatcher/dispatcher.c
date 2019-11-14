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

/*FPS换算成ms*/
#define	FPS(x)	(1000/x)

uint8_t key_vaule_buff;
extern uint8_t key_flag;
extern uint8_t HX711_FLAG;  //正在读取时为0
extern output_data	out_data;

extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
volatile uint64_t sys_time=0;//系统总时间
extern uint32_t set_zero;
extern float GapValue;
extern uint16_t step_t;
extern all_data data;
extern uint16_t HX_weight;
extern uint16_t rmp;

/*
OLED.C
OLED刷新标志
*/
extern bool oledFresh;	

/*
任务分配：
蜂鸣器任务
按键扫描任务
串口发送任务
状态灯任务（目前不支持）
传感器数据采集
	压力传感器
	温度传感器
	转速传感器

*/
struct					//此结构体下时间单位为ms
{
	volatile int delay;			//ms级延时，us级延时需要使用专用定时器
	volatile u32 OledRefreshTaskRemainTime;		//任务剩余时间
	volatile u32 OLedRefreshTaskPerTime;			//任务预设时间
	volatile void (*OledRefreshTask)(void);		//函数指针
	volatile bool OLED_Trig;
    
	/*按键扫描*/
	volatile u32 KeyScanRemain;
	volatile u32 KeyScanPer;
	volatile void (*KeyScanTask)(void);
    volatile bool KeyScan_Trig;
	
	volatile u32 USARTransmitRemain;
	volatile u32 USARTranmitPer;
	volatile void (*USARTranmit)(void);
	volatile bool USARTransmin_Trig;
    
	volatile u32 TemperautreRemain;              //不需要函数指针，直接调用
	volatile u32 TemperaturePer;
	volatile bool Temperature_Trig;
	
	volatile u32 FliterFPS;				//滤波器帧率计算
	volatile u32 FliterFPSCounter;		//

}Tick;
static  uint32_t SysTickConfig(uint32_t ticks);

/*定时器任务设置*/
void TaskInit()
{	
	Tick.delay=0;
	Tick.OLedRefreshTaskPerTime	    =	FPS(OLED_FPS);
	Tick.OledRefreshTaskRemainTime	=	Tick.OLedRefreshTaskPerTime;
	Tick.OledRefreshTask	        =	&OLED_Refresh;
	Tick.OLED_Trig                  =   false;
    
	Tick.KeyScanPer	    =	FPS(100);//按键扫描速度
	Tick.KeyScanRemain	=	Tick.KeyScanPer;
	Tick.KeyScanTask	=	&ScanKey;
	Tick.KeyScan_Trig   =   false;
    
	Tick.USARTranmitPer	    =	FPS(100);//刷新速度
	Tick.USARTransmitRemain	=	Tick.USARTranmitPer;
	Tick.USARTranmit	    =	&ANO_TransmitCallBack;
	Tick.USARTransmin_Trig  =   false;
    
	Tick.TemperaturePer	    =	FPS(10);
	Tick.TemperautreRemain  =   Tick.TemperaturePer;
	Tick.Temperature_Trig   =   false;
    
	Tick.FliterFPS	        =	0;//滤波器滤波频率采样结果
	Tick.FliterFPSCounter   =   0;//滤波器滤波次数计数器
    
}


#ifdef USE_DSIPATCHER

//向下计数
void dispatcher_set()
{	
	SysTickConfig(72000);//72M主频下 1000hz
}
/*
调度程序
注意：不要将具体的执行函数放在中断中，否则会严重影响中断的执行速度
此函数内的所有内容需要在1ms内执行完毕
造成中断溢出
通用任务的执行放在主循环中
*/
void dispatcher(void)
{		
	ANO.clock++;//应该赋予这个变量更正式的名字
	sys_time++;//系统时间（ms）
	
    /*每100ms读下电机的转速*/
	if(ANO.clock%100==0)
	{
	rmp	=	get_rmp()*10/2;//这个算法需要优化
	}
	
    if(ANO.clock==1000)//帧数计算
	{		
		ANO.clock=0;
		Tick.FliterFPS= Tick.FliterFPSCounter;
		Tick.FliterFPSCounter	=	0;
	}
	if(Tick.delay)
     Tick.delay--;
	/*刷新温度*/
    
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
来不及触发的Task会被覆盖
滤波次数尽可能的多，数据才能稳
不加任务执行标志只有1ms 的任务时间
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
	HX_weight=get_wetght();
	
	PWM1_Out_H(data.throttle);
    
    
	if(Tick.KeyScan_Trig)
    {   
        Tick.KeyScan_Trig   =   false;
        (*Tick.KeyScanTask)();
    }
    
	if(Tick.Temperature_Trig)
	{   Tick.Temperature_Trig   =   false;
		data.tempture0	=	Read_MLX_IIC_Data(0x07)*0.02-273.15;    //远端温度
		data.tempture1	=	Read_MLX_IIC_Data(0x06)*0.02-273.15;	//探头本身温度
	}
	
	if(oledFresh && Tick.OLED_Trig)
    {   
        Tick.OLED_Trig  =   false;
        (*Tick.OledRefreshTask)();
	}
    
	if(Tick.USARTransmin_Trig )
    {
        Tick.USARTransmin_Trig  =   false;
		(*Tick.USARTranmit)();	//发送数据
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
SYSTick配置
*/
static  uint32_t SysTickConfig(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  =		SysTick_CTRL_TICKINT_Msk   |							//启动sysTick
						SysTick_CTRL_CLKSOURCE_Msk |							//	AHB时钟
						SysTick_CTRL_ENABLE_Msk;					
  return 0;       																						/* Function successful */
}

#endif

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

#endif	/*DISPATCHER_H*/


