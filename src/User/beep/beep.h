#ifndef BEEP_H
#define	BEEP_H

#include	"stm32f10x.h"


#define	beep_GPIO	GPIOA
#define	beep_GPIO_Pin	GPIO_Pin_8

#define	beep_TIM	TIM1
#define	Beep_TIM_clock_FREQ	SystemCoreClock		//72MHZ有效
#define	Beep_TIM_Prescaler	720					//定时器720分频	（这个分频会在20000HZ时出现较大音量计算舍余误差）
#define	beep_clock			(Beep_TIM_clock_FREQ/Beep_TIM_Prescaler)
#define	beep_TIM_Period		(beep_clock/beep_fre)	//定时器周期
#define	beep_TIM_Pulse		((beep_TIM_Period-((beep_TIM_Period*beep_Sound)/20))+1)
#define	beep_FRE(fre)		(beep_clock/fre)
#define	beep_Volume(Vol,fre)		(u16)((beep_FRE(fre)-((beep_FRE(fre)*Vol)/20))+1)

void beep_set(void);
void set_beep_fre_vol(u16 fre, u16 vol);

#endif	/*BEEP_H*/
