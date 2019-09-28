#ifndef EPDIF_H
#define EPDIF_H

#include "stm32f10x.h"
// Pin definition
#define CS_PIN           0
//#define RST_PIN        1
#define DC_PIN           2
#define BUSY_PIN         3

// Pin level definition
#define LOW             0
#define HIGH            1

typedef struct 
	{
  GPIO_TypeDef* port;
  int pin;
	} EPD_Pin;

int  EpdInitCallback(void);
void EpdDigitalWriteCallback(int pin, int value);
int  EpdDigitalReadCallback(int pin);
void EpdDelayMsCallback(unsigned int delaytime);
void EpdSpiTransferCallback(unsigned char data);

#endif /* EPDIF_H */
