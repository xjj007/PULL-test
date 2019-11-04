#ifndef	ADC_H
#define	ADC_H

#include "stm32f10x.h"

/*
所有有关ADC的外设在此配置
*/


#define	KEY_PORT	GPIOA
#define GPIO_KEY_Pin	GPIO_Pin_0
#define	RCC_KEY_Periph	RCC_APB2Periph_ADC1


enum ADC_Sourse		//ADC 源
{
		KEY=0,
		poten,
		BAT,
		CUR,
};



enum _key_vaule		//键值
{	
		no_key,
		down,
		up,
		right,
		mid,
		left,
};


// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

#define		MAX_Current		70	//最大电流70A

// 转换通道个数
#define    NOFCHANEL										 4

#define    ADC_PIN1                      GPIO_Pin_0
#define    ADC_CHANNEL1                  ADC_Channel_0

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_1

#define    ADC_PIN3                      GPIO_Pin_2
#define    ADC_CHANNEL3                  ADC_Channel_2

#define    ADC_PIN4                      GPIO_Pin_3
#define    ADC_CHANNEL4                  ADC_Channel_3

#define    ADC_PIN5                      GPIO_Pin_4
#define    ADC_CHANNEL5                  ADC_Channel_4

#define    ADC_PIN6                      GPIO_Pin_5
#define    ADC_CHANNEL6                  ADC_Channel_5


// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************函数声明********************************/

void ADCx_Init(void);
void KEY_INIT(void);
static void ADC_NVIC_Config(void);
uint8_t key_vaule(void);
void voltage_calibration(int offset);
void current_calibrater(void);
uint8_t key_vaule(void);
uint8_t getkey(void);
uint8_t getK(void);
void ScanKey(void);

#endif	/*ADC_H*/
