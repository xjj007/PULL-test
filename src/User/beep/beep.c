#ifndef BEEP_C
#define	BEEP_C

#include	"beep.h"

//������Ҫ����ȫ�ֱ���
//������С�������Ÿ��û�����
/*�˱�����ʧЧ*/
uint16_t beep_fre	=	5000;//��λHZ��ȡֵ20-20000
uint16_t beep_Sound	=	0; //������С��0-10��		

typedef struct
{
	u16 B_fre;	//Ƶ��/HZ
	u8 	B_vol;	//��С
	u16  B_onT;	//ʱ��/ms
}beep_seq;

const beep_seq beep[1]={1000,10,500};
const beep_seq B_power_on[5]={1000,10,500,2000,10,500,500,10,500};//��������
/*
���ڿ��Ʒ���������ָ��������Ƶ��
*/

/*
���������巽��
*/

/*
����������
�ɱ�����
*/
void beep_set()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	GPIO_InitTypeDef GPIO_config;
	TIM_OCInitTypeDef	TIM_OCSet;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_config.GPIO_Mode	=	GPIO_Mode_AF_PP;//��������
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	/*beep GPIO set*/
	GPIO_config.GPIO_Pin	=	beep_GPIO_Pin;
	GPIO_Init(beep_GPIO,&GPIO_config);
	
	GPIO_SetBits(beep_GPIO,beep_GPIO_Pin);
	
	/*20-20000hz*/
	TIM_Base_SET.TIM_Prescaler		=	Beep_TIM_Prescaler;
	
	TIM_Base_SET.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period			=	beep_TIM_Period;		//��������
	TIM_Base_SET.TIM_ClockDivision	=	TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM1,&TIM_Base_SET);
	
	TIM_OCSet.TIM_OCMode				=	TIM_OCMode_PWM1;
	TIM_OCSet.TIM_OutputState		=	TIM_OutputState_Enable;
	TIM_OCSet.TIM_Pulse				=	beep_TIM_Pulse;//�������
	TIM_OCSet.TIM_OCPolarity			=	TIM_OCPolarity_High;

	TIM_OC1Init(TIM1,&TIM_OCSet);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	
}

/*
���÷���������Ƶ�ʺ����
*/
void set_beep_fre_vol(u16 fre, u16 vol)
{
	TIM1->ARR	=	beep_FRE(fre);
	TIM1->CCR1	= beep_Volume(vol,fre);
}

/*
beep����
��ô֪�����г�����
��������ʽ�����֡β
���룺beep �������е�ַ

*/
void beep_drive(beep_seq *seq)
{

}


#endif	/*BEEP_C*/
