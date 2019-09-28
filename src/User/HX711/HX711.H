#ifndef HX711_H
#define	HX711_H

#include	"stm32f10x.h"

#define	PD_SCK_Pin			GPIO_Pin_12
#define	GPIO_PD_SCK			GPIOA
#define	DOUT_Pin			GPIO_Pin_11
#define	GPIO_DOUT			GPIOA
#define	Read_DOUT			GPIO_ReadInputDataBit(GPIO_DOUT,DOUT_Pin)


void HX711_SET(void);
uint32_t read_HX711(void);
void HX711set_to_0(void);
uint16_t get_wetght(void);

#endif

