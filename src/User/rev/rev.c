//转速传感器驱动
#ifndef	REV_C
#define	REV_C

/*
目前航模电机最高210000转/分
3500转/秒
3500hz
*/
#include	"rev.h"

uint16_t rmp=0;

void REV_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_config;
	RCC_APB2PeriphClockCmd(RCC_REVPeriph_GPIO,ENABLE);
	
	GPIO_config.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;//浮空
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_config.GPIO_Pin	=	GPIO_REV_Pin;
	
	GPIO_Init(REV_GPIO,&GPIO_config);
}
/*
这段代码需要重新配置为复位模式
ccr1内的值即为周期
*/
void REV_Config()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	
	REV_GPIO_Config();
	RCC_APB1PeriphClockCmd(RCC_REVPeriph_TIM3,ENABLE);
	
	TIM_Base_SET.TIM_Prescaler	=	0;//1080;//最高支持到6.6万转/秒
																			//最低支持1转/秒
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;		//向上计数模式
	TIM_Base_SET.TIM_Period	=	65535;							
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM3,&TIM_Base_SET);
	TIM3->SMCR |= TIM_SlaveMode_External1;	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI2FP2);

	
	TIM_Cmd(TIM3,ENABLE);
}
//获取转速
//不用中断你就不知道什么时候发生了事件，
//单输入模式下，必须在事件发生时就立即复位计数器
//只需要获得周期就行了
uint16_t get_rmp()
{
	uint16_t temp=TIM3->CNT;
	TIM3->CNT=0;
	return temp;
}
#endif	/*REV_C*/
