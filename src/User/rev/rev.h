#ifndef	REV_H
#define	REV_H

#include	"stm32f10x.h"

//TIM3_CH3
#define	REV_GPIO			GPIOB
#define	GPIO_REV_Pin		GPIO_Pin_0
#define	RCC_REVPeriph_GPIO  RCC_APB2Periph_GPIOB
#define	RCC_REVPeriph_TIM3	RCC_APB1Periph_TIM3

uint16_t get_rmp(void);
void REV_Config(void);
void REV_GPIO_Config(void);
uint16_t get_rmp(void);

#endif	/*REV_H*/
