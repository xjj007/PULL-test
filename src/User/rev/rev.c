//ת�ٴ���������
#ifndef	REV_C
#define	REV_C

/*
Ŀǰ��ģ������210000ת/��
3500ת/��
3500hz
*/
#include	"rev.h"

uint16_t rmp=0;

void REV_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_config;
	RCC_APB2PeriphClockCmd(RCC_REVPeriph_GPIO,ENABLE);
	
	GPIO_config.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;//����
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_config.GPIO_Pin	=	GPIO_REV_Pin;
	
	GPIO_Init(REV_GPIO,&GPIO_config);
}
/*
��δ�����Ҫ��������Ϊ��λģʽ
ccr1�ڵ�ֵ��Ϊ����
*/
void REV_Config()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	
	REV_GPIO_Config();
	RCC_APB1PeriphClockCmd(RCC_REVPeriph_TIM3,ENABLE);
	
	TIM_Base_SET.TIM_Prescaler	=	0;//1080;//���֧�ֵ�6.6��ת/��
																			//���֧��1ת/��
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;		//���ϼ���ģʽ
	TIM_Base_SET.TIM_Period	=	65535;							
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM3,&TIM_Base_SET);
	TIM3->SMCR |= TIM_SlaveMode_External1;	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI2FP2);

	
	TIM_Cmd(TIM3,ENABLE);
}
//��ȡת��
//�����ж���Ͳ�֪��ʲôʱ�������¼���
//������ģʽ�£��������¼�����ʱ��������λ������
//ֻ��Ҫ������ھ�����
uint16_t get_rmp()
{
	uint16_t temp=TIM3->CNT;
	TIM3->CNT=0;
	return temp;
}
#endif	/*REV_C*/
