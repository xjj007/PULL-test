#ifndef	PWM_H
#define	PWM_H

#include	"stm32f10x.h"
//TIM4CH2
#define MOTO_GPIO	GPIOB
#define	GPIO_MOTOR_Pin GPIO_Pin_7//电机PWM输出
//TIM4CH4
#define	POTEN_GPIO GPIOB
#define	GPIO_POTEN_Pin	GPIO_Pin_9	//电位器

#define PWM0 0	//电机控制输出
#define PWM1 1	//舵机控制输出

#define	beep_GPIO	GPIOA
#define	beep_GPIO_Pin	GPIO_Pin_8

#define	beep_TIM	TIM1


int set_frequence(uint16_t frequence);
void PWM_SET(void);
void PWM_GPIO_INIT(void);
void PWM1_Enable(void);
void PWM1_Disable(void);
void PWM1OutPut(uint16_t vaule);

void PWM2_Enable(void);
void PWM2_Disable(void);
void PWM2OutPut(uint16_t vaule);




#endif	/*PWM_H*/

