#ifndef	DISPATCHER_H
#define	DISPATCHER_H
#include	"stm32f10x.h"

#define USE_USART1
//#define KEY_TEST
#define USE_FLASH
#define USE_AVERAGE_FLITER
//#define PWM_TEST


extern volatile uint64_t sys_time;

#ifndef	USE_TIM6_DELAY
void delay_ms(uint32_t ms);
#endif	/*USE_TIM6_DELAY*/

void dispatcher(void);
void dispatcherMain(void);
void dispatcher_set(void);

void TaskInit(void);

#endif	/*DISPATCHER_H*/
