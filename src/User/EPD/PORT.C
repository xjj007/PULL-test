#ifndef	PORT_C
#define	PORT_C

#include	"port.h"
#include "epdif.h"


void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
												 RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|
												 RCC_APB2Periph_AFIO,ENABLE);

  /*Configure GPIO pin Output Level */
  GPIO_ResetBits(DC_GPIO_Port, DC_Pin);

  /*Configure GPIO pin Output Level */
  GPIO_ResetBits(SPI_CS_GPIO_Port, SPI_CS_Pin);
 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  /*Configure GPIO pin : DC_Pin */
  GPIO_InitStruct.GPIO_Pin = DC_Pin;
 // GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_CS_Pin */
  GPIO_InitStruct.GPIO_Pin = SPI_CS_Pin;
  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	EpdInitCallback();

}

 void MX_SPI1_Init(void)
{

  SPI_InitTypeDef hspi1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
  hspi1.SPI_Mode = SPI_Mode_Master;
  hspi1.SPI_Direction = SPI_Direction_1Line_Tx;
  hspi1.SPI_DataSize = SPI_DataSize_8b;
  hspi1.SPI_CPOL = SPI_CPOL_Low;
  hspi1.SPI_CPHA = SPI_CPHA_1Edge;
  hspi1.SPI_NSS = SPI_NSS_Soft;
  hspi1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  hspi1.SPI_FirstBit = SPI_FirstBit_MSB;
  //hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
 // hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.SPI_CRCPolynomial = 10;
  
	SPI_Init(SPI1,&hspi1);
	//SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,DISABLE);
	SPI_Cmd(SPI1, ENABLE);
	
	//SPI1_NVIC_Config();
}
#if 0
static void SPI1_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  // 配置中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#endif
#endif	/*PORT_C*/

