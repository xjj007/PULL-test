//ת�ٴ���������
#ifndef	REV_C
#define	REV_C

/*
Ŀǰ��ģ������210000ת/��
3500ת/��
3500hz

*/
#include	"rev.h"

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
	TIM_ICInitTypeDef TIM_IC_Set;
	
	REV_GPIO_Config();
	RCC_APB1PeriphClockCmd(RCC_REVPeriph_TIM3,ENABLE);
	
	TIM_Base_SET.TIM_Prescaler	=	1080;//���֧�ֵ�6.6��ת/��
																			//���֧��1ת/��
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
//��ȡת��
//�����ж���Ͳ�֪��ʲôʱ�������¼���
//������ģʽ�£��������¼�����ʱ��������λ������
//ֻ��Ҫ������ھ�����
uint16_t get_rmp()
{
 return TIM_GetCapture1(TIM3);
	
}
#endif	/*REV_C*/
