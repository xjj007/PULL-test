//we have two kinds of pwm output
// ����͵�λ������������PWM���뺯��
//��Ҫ�õ����ʱ������
/*����Ҫ����һ·������*/
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
	
	GPIO_config.GPIO_Mode	=	GPIO_Mode_AF_PP;//��������
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_config.GPIO_Pin	=	GPIO_MOTOR_Pin;
	
	GPIO_Init(MOTO_GPIO,&GPIO_config);
	GPIO_config.GPIO_Pin	=	GPIO_POTEN_Pin;
	GPIO_Init(POTEN_GPIO,&GPIO_config);
	
	GPIO_ResetBits(MOTO_GPIO,GPIO_MOTOR_Pin);
	GPIO_ResetBits(POTEN_GPIO,GPIO_POTEN_Pin);

}


void PWM_SET()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	PWM_GPIO_INIT();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	peroid=1000000/fre;
	
	TIM_Base_SET.TIM_Prescaler	=	72;//ʱ��Ƶ��1M
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	peroid;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM4,&TIM_Base_SET);
	
	TIM_OCSet.TIM_OCMode	=	TIM_OCMode_PWM1;
	TIM_OCSet.TIM_OutputState	=	TIM_OutputState_Enable;
	TIM_OCSet.TIM_Pulse	=	1000;
	TIM_OCSet.TIM_OCPolarity	=	TIM_OCPolarity_High;

	TIM_OC2Init(TIM4,&TIM_OCSet);
	TIM_OC4Init(TIM4,&TIM_OCSet);
	//PWM������Ϻ���Ҫǿ��pwm���Ϊ��
	
	//TIM_ForcedOC2Config(TIM4,TIM_ForcedAction_InActive);
	//TIM_ForcedOC4Config(TIM4,TIM_ForcedAction_InActive);
	
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
//�߾��Ȱ�
//��Χ0-4000
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
//������׼��
//vaule ռ�ձȣ��е�ֵ1500�����1000����2000
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

/*
����������
�ɱ�����
*/
void beep_set()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	
	/*20-20000hz*/
	TIM_Base_SET.TIM_Prescaler	=	7200;//ʱ��Ƶ��1M
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period	=	10;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
//	TIM_Base_SET.TIM_RepetitionCounter	=	
	TIM_TimeBaseInit(TIM1,&TIM_Base_SET);
	
	TIM_OCSet.TIM_OCMode	=	TIM_OCMode_PWM1;
	TIM_OCSet.TIM_OutputState	=	TIM_OutputState_Enable;
	TIM_OCSet.TIM_Pulse	=	1000;
	TIM_OCSet.TIM_OCPolarity	=	TIM_OCPolarity_High;

	TIM_OC1Init(TIM1,&TIM_OCSet);
	
	
	
	
}

#endif	/*PWM_C*/
