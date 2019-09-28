/**
  ******************************************************************************
  * @file    delay.h
  * @author  XJJ
  * @version ######
  * @date    2018��2��25��
  * @brief   ����ļ�����������ʱ
  ******************************************************************************
  */
	
	
#ifndef	DELAY_H
#define	DELAY_H









//ʹ�ö�ʱ��6���ж�ʱ
void delay_init(uint32_t Period)
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;

	TIM_Base_SET.TIM_Prescaler	=	1;
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Down;
	TIM_Base_SET.TIM_Period	=	Period;
	TIM_Base_SET.TIM_ClockDivision	=TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM6,&TIM_Base_SET);
}

#ifdef USE_TIM6_DELAY
void delay_ms(uint32_t time)
{
	delay_init(72000);
	TIM_Cmd(TIM6,ENABLE);
	for(;time>0;time--)
	{
	while( TIM3->CNT>0 );
	
	}
	TIM_Cmd(TIM6,DISABLE);
}

void delay_us(uint32_t time)
{
	delay_init(72);
	TIM_Cmd(TIM6,ENABLE);
	for(;time>0;time--)
	{
	while( TIMx->CNT>0 );
	
	}
	TIM_Cmd(TIM6,DISABLE);
}
#endif 	/*USE_TIM6_DELAY*/
#endif	/*DELAY_H*/
