#ifndef	PWM_C
#define	PWM_C

#include	"PWM.H"


uint16_t peroid;
uint16_t fre=50;

//uint16_t frequence	=	1/((float)peroid/100000);
 //uint16_t fre=1000000/peroid;
TIM_OCInitTypeDef	TIM_OCSet;
void PWM_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_config;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_config.GPIO_Mode	=	GPIO_Mode_AF_PP;//复用推挽
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	
	
	GPIO_config.GPIO_Pin	=	GPIO_MOTOR_Pin;
	GPIO_Init(MOTO_GPIO,&GPIO_config);
		
	
	GPIO_ResetBits(MOTO_GPIO,GPIO_MOTOR_Pin);

}

/*时钟频率不对啊*/
 void PWM_SET()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	PWM_GPIO_INIT();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	peroid=1000000U/fre;
	
	TIM_Base_SET.TIM_Prescaler	=	72;//时钟频率1M
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	peroid;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM4,&TIM_Base_SET);
	
	TIM_OCSet.TIM_OCMode	=	TIM_OCMode_PWM1;
	TIM_OCSet.TIM_OutputState	=	TIM_OutputState_Enable;
	TIM_OCSet.TIM_Pulse	=	1000;
	TIM_OCSet.TIM_OCPolarity	=	TIM_OCPolarity_High;

	TIM_OC2Init(TIM4,&TIM_OCSet);
	//TIM_OC4Init(TIM4,&TIM_OCSet);
	//PWM配置完毕后需要强制pwm输出为低
	
	TIM_Cmd(TIM4, ENABLE);
}

void PWM1_Enable()
{
	TIM_OCSet.TIM_Pulse	=	100;
	TIM_OC2Init(TIM4,&TIM_OCSet);

}

void PWM1_Disable()
{
	TIM_ForcedOC2Config(TIM4,TIM_ForcedAction_InActive);
}

void PWM1OutPut(uint16_t vaule)
{	
	if(vaule>100)vaule =0;
	
		vaule=vaule*0.0005*peroid+peroid*0.05;
		TIM4->CCR2=vaule;
	
}
//高精度版
//范围0-4000
void PWM1_Out_H(uint16_t vaule)
{
	if(vaule>4000)vaule =0;
	vaule=peroid*vaule*0.0000125+peroid*0.05;
	TIM4->CCR2=vaule;
}
	

void PWM2_Enable()
{
	TIM_OCSet.TIM_Pulse	=	100;
	TIM_OC4Init(TIM4,&TIM_OCSet);
	//TIM_ForcedOC2Config(TIM4,TIM_ForcedAction_InActive);
}

void PWM2_Disable()
{
	TIM_ForcedOC4Config(TIM4,TIM_ForcedAction_InActive);
}
//基本是准的
//vaule 占空比，中点值1500，最低1000，最2000
void PWM2OutPut(uint16_t vaule)
{
	if(vaule>100)
		vaule =0;
	vaule=vaule*0.0005*peroid+peroid*0.05;
		TIM4->CCR4=vaule;
}

int set_frequence(uint16_t frequence)
{
	if(frequence>400)
		return ERROR;//
	peroid=1000000/frequence;
	PWM_SET();
	return SUCCESS;
}
#endif	/*PWM_C*/

