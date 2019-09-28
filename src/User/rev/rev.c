//转速传感器驱动
#ifndef	REV_C
#define	REV_C

/*
目前航模电机最高210000转/分
3500转/秒
3500hz

*/
#include	"rev.h"

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
	TIM_ICInitTypeDef TIM_IC_Set;
	
	REV_GPIO_Config();
	RCC_APB1PeriphClockCmd(RCC_REVPeriph_TIM3,ENABLE);
	
	TIM_Base_SET.TIM_Prescaler	=	1080;//最高支持到6.6万转/秒
																			//最低支持1转/秒
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	65535;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM3,&TIM_Base_SET);
	
	TIM_IC_Set.TIM_Channel	=	TIM_Channel_3;
	TIM_IC_Set.TIM_ICPolarity	=	TIM_ICPolarity_Rising;
	TIM_IC_Set.TIM_ICSelection	=	TIM_ICSelection_DirectTI;
	TIM_IC_Set.TIM_ICPrescaler	=	TIM_ICPSC_DIV1;
	TIM_IC_Set.TIM_ICFilter	=	0x0;
	
	TIM_ICInit(TIM3,&TIM_IC_Set);
	
	TIM_Cmd(TIM3,ENABLE);
}
//获取转速
//不用中断你就不知道什么时候发生了事件，
//单输入模式下，必须在事件发生时就立即复位计数器
//只需要获得周期就行了
uint16_t get_rmp()
{
 return TIM_GetCapture1(TIM3);
	
}
#endif	/*REV_C*/
