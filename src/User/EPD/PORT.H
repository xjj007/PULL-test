#ifndef PORT_H
#define	PORT_H

#include "stm32f10x.h"


#define DC_Pin GPIO_Pin_8
#define DC_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_Pin_6
#define SPI_CS_GPIO_Port GPIOB

void MX_GPIO_Init(void);
void MX_SPI1_Init(void);
static void SPI1_NVIC_Config(void);
#endif	/*PORT_H*/

