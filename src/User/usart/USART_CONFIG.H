/**
  ******************************************************************************
  * @file    USART_CONFIG.H
  * @author  XJJ
  * @version ######
  * @date    2018年2月24日
  * @brief   这个文件用来配置串口通信
  ******************************************************************************
  */
#include "stm32f10x.h"
#ifndef	USART_CONFIG_H
#define	USART_CONFIG_H

/***********定义缓存长度****************/
#define		USART1_TX_CATCH_LENGTH		256	
#define		USART1_RX_CATCH_LENGTH		256
#define		USART2_TX_CATCH_LENGTH		128
#define		USART2_RX_CATCH_LENGTH		128
//串口波特率定义
#define USART1_Rate 115200	
#define USART2_Rate 115200	


/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


typedef struct 
{
	float	S_cur;
	float	S_vol;
	float	S_pow;
	float	S_eff;//桨效
	float	S_pull;
	u16		S_thr;
	u16		S_rmp;
	float	t0;		//温度传感器0
	float	t1;		//温度传感器1
	u16		clock;	//时钟
	



}ANO_DT;


extern  ANO_DT	ANO;
extern  bool Bluetooth_connect;

void USART_GPIO_INTIT(void);
void USART_config(void);
void PrintString(USART_TypeDef* USARTx,uint8_t *puts);

void ANO_TransmitCallBack(void);
void dat_sent(USART_TypeDef* USARTx,uint8_t *dataToSend , uint8_t length);
void ANO_DT_Send_RCData(float cur,	float vol,	float pow,	
						float eff,	float pull,	u16 thr,
						u16 rmp,	float t0,	float t1,u16 clock);


void ANO_DT_SendStruct(ANO_DT *ANO);
void clear_USART1_RX_buff(void);
uint8_t Bluetooth(void);
void set_USART1_Baud(uint32_t baud);
void NVIC_config(void);
void Bluetooth_state_LED_init(void);
#endif	/*USART_CONFIG_H*/

