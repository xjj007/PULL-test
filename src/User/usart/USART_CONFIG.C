/**
  ******************************************************************************
  * @file    USART_CONFIG.C
  * @author  XJJ
  * @version ######
  * @date    2018年2月24日
  * @brief   这个文件用来配置串口通信
  ******************************************************************************
  */
#include "stm32f10x.h"
#include	"USART_CONFIG.H"
#include	"dispatcher.h"
#include "fonts.h"
#include	"OLED.H"
#include	"fliter.h"
extern uint16_t USART_RX_FLAG;
extern all_data data;
#ifdef	USE_USART1
uint8_t USART1_RX_CATCH[USART1_RX_CATCH_LENGTH]	=	{0};//串口1接收缓存
#endif

#ifdef	USE_USART3
uint8_t USART3_RX_CATCH[USART3_RX_CATCH_LENGTH]	=	{0};//串口2接收缓存
#endif

uint8_t baud=1;
//自动配置HC05的波特率表
uint32_t baud_rate[12]={460800,921600,230400,38400,115200,
						57600,19200,9600,4800,2400,1200,1382400};

												
uint8_t data_to_send[50];	//匿名科创发送数据缓存


ANO_DT	ANO={0};						
						
void	USART_GPIO_INTIT()
{
		GPIO_InitTypeDef	GPIO_INIT;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_INIT.GPIO_Speed=GPIO_Speed_50MHz;
	
		GPIO_INIT.GPIO_Mode	=	GPIO_Mode_AF_PP;			  //复用推挽输出
		GPIO_INIT.GPIO_Pin	=	GPIO_Pin_9;							//USART1_TX
		GPIO_Init(GPIOA,&GPIO_INIT);
	
		GPIO_INIT.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;	//浮空输入
		GPIO_INIT.GPIO_Pin	=	GPIO_Pin_10;						//USART1_RX
		GPIO_Init(GPIOA,&GPIO_INIT);
	
	#ifdef	USE_USART3
		GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
		GPIO_INIT.GPIO_Mode	=	GPIO_Mode_AF_PP;			  //复用推挽输出
		GPIO_INIT.GPIO_Pin	=	GPIO_Pin_10;							//USART3_TX
		GPIO_Init(GPIOC,&GPIO_INIT);
		
		GPIO_INIT.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;	//浮空输入
		GPIO_INIT.GPIO_Pin	=	GPIO_Pin_11;						//USART3_RX
		GPIO_Init(GPIOC,&GPIO_INIT);
	#endif
	
	
}

//==============================================//
//串口初始化函数
//初始化串口1和串口3
//==============================================//
void USART_config(void)
{
	
		USART_InitTypeDef	USART_Ini;
		uint32_t over_time;
		NVIC_config();
		USART_GPIO_INTIT();
	
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//打开时钟
	//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	
		USART_Ini.USART_BaudRate=USART1_Rate;
		USART_Ini.USART_WordLength=USART_WordLength_8b;
		USART_Ini.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//关闭硬件流控制
		USART_Ini.USART_StopBits=USART_StopBits_1;//一位停止位
		USART_Ini.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
		USART_Ini.USART_Parity=USART_Parity_No;
	
		USART_Init(USART1,&USART_Ini);
	
#ifdef	USE_USART3

	//USART_Ini.USART_BaudRate=USART2_Rate;
	//USART_Init(USART3,&USART_Ini);
	//USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//接收中断打开	
	//USART_Cmd(USART3,ENABLE);
	
#endif
	
	
	
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接收中断打开
		USART_Cmd(USART1,ENABLE);
#if 0	
	show_string(0,18,"config Bluetooth",&Font8X16,COLOR);
	show_string(0,36,"Please wait...",&Font8X16,COLOR);
	clear_USART1_RX_buff();
	PrintString(USART1,"AT");
	for(over_time=0x4fffff;USART_RX_FLAG<2&&over_time>0;over_time--);
	if(USART1_RX_CATCH[0]=='O'&&USART1_RX_CATCH[1]=='K')//收到正确回复
	{
		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//接收中断关闭
		show_string(0,18,"config SUCCESS!!",&Font8X16,COLOR);
		return;
	}
	else if(Bluetooth()==SUCCESS) //未收到正确答复，进行蓝牙设置
	
					show_string(0,18,"config SUCCESS!!",&Font8X16,COLOR);
			else 
					show_string(0,18,"config ERROR!!   ",&Font8X16,COLOR);
		delay_ms(1000);
		OLED_Clear();

		set_USART1_Baud(USART1_Rate);
#endif	
		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//接收中断关闭
		//USART_Cmd(USART1,DISABLE);
	//串口配置好了就不再需要接收数据了

}
void NVIC_config()
{
	NVIC_InitTypeDef	NVIC_Set;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_Set.NVIC_IRQChannel	=	USART1_IRQn;
	NVIC_Set.NVIC_IRQChannelPreemptionPriority	=	0;
	NVIC_Set.NVIC_IRQChannelSubPriority	=	1;
	NVIC_Set.NVIC_IRQChannelCmd	=	ENABLE;
	NVIC_Init(&NVIC_Set);
	
}
//===========================================//
//打印字符函数
//输入：字符
//============================================//
void PrintString(USART_TypeDef* USARTx,uint8_t *puts)
{
	
	//==============================USART1============================//
    if (USARTx == USART1)
			
		{ USART1->SR;
				for (; *puts != 0;	puts++) 
				{						
					USART_SendData(USART1,*(puts));
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
				}		
			//	USART_SendData(USART1,*puts);
					//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}
		
	//=========================USART2=============================//
		if(USARTx==USART3)
			
		{
		for (; *puts != 0;	puts++)  
			{
					USART_SendData(USART3,*puts);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
			}
			}
}

/*

匿名科创回调函数

*/
void ANO_TransmitCallBack()
{
		ANO_DT_SendStruct((ANO_DT*) &data);
}

void dat_sent(USART_TypeDef* USARTx,uint8_t *dataToSend , uint8_t length)
{
	
	uint16_t i=0;
	for(;i<length;i++)
	{
		USART_SendData(USARTx,dataToSend[i]);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
	
	}

}


//这个函数用来和匿名科创地面站进行通信
void ANO_DT_Send_RCData(float cur,float vol,float pow,float eff,
	float pull,u16 thr,u16 rmp,float t0,float t1,u16 clock)
{
	uint8_t _cnt=0;
	uint8_t sum = 0;
	uint8_t i=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xF1;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE3(cur); 
	data_to_send[_cnt++]=BYTE2(cur);
	data_to_send[_cnt++]=BYTE1(cur);
	data_to_send[_cnt++]=BYTE0(cur);
	data_to_send[_cnt++]=BYTE3(vol); 
	data_to_send[_cnt++]=BYTE2(vol);
	data_to_send[_cnt++]=BYTE1(vol);
	data_to_send[_cnt++]=BYTE0(vol);
	data_to_send[_cnt++]=BYTE3(pow); 
	data_to_send[_cnt++]=BYTE2(pow);
	data_to_send[_cnt++]=BYTE1(pow);
	data_to_send[_cnt++]=BYTE0(pow);
	data_to_send[_cnt++]=BYTE3(eff); 
	data_to_send[_cnt++]=BYTE2(eff);
	data_to_send[_cnt++]=BYTE1(eff);
	data_to_send[_cnt++]=BYTE0(eff);
	data_to_send[_cnt++]=BYTE3(pull); 
	data_to_send[_cnt++]=BYTE2(pull);
	data_to_send[_cnt++]=BYTE1(pull);
	data_to_send[_cnt++]=BYTE0(pull);
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(rmp);
	data_to_send[_cnt++]=BYTE0(rmp);
	data_to_send[_cnt++]=BYTE3(t0);
	data_to_send[_cnt++]=BYTE2(t0);
	data_to_send[_cnt++]=BYTE1(t0);
	data_to_send[_cnt++]=BYTE0(t0);
	data_to_send[_cnt++]=BYTE3(t1);
	data_to_send[_cnt++]=BYTE2(t1);
	data_to_send[_cnt++]=BYTE1(t1);
	data_to_send[_cnt++]=BYTE0(t1);
	data_to_send[_cnt++]=BYTE1(clock);
	data_to_send[_cnt++]=BYTE0(clock);

	data_to_send[3] = _cnt-4;
	
	
	for( ;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	dat_sent(USART1,data_to_send, _cnt);
}

/*
结构体版本
*/
void ANO_DT_SendStruct(ANO_DT *ANO)
{
	uint8_t _cnt=0;
	uint8_t sum = 0;
	uint8_t i=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xF1;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE3(ANO->S_cur ); 
	data_to_send[_cnt++]=BYTE2(ANO->S_cur );
	data_to_send[_cnt++]=BYTE1(ANO->S_cur );
	data_to_send[_cnt++]=BYTE0(ANO->S_cur );
	data_to_send[_cnt++]=BYTE3(ANO->S_vol); 
	data_to_send[_cnt++]=BYTE2(ANO->S_vol);
	data_to_send[_cnt++]=BYTE1(ANO->S_vol);
	data_to_send[_cnt++]=BYTE0(ANO->S_vol);
	data_to_send[_cnt++]=BYTE3(ANO->S_pow); 
	data_to_send[_cnt++]=BYTE2(ANO->S_pow);
	data_to_send[_cnt++]=BYTE1(ANO->S_pow);
	data_to_send[_cnt++]=BYTE0(ANO->S_pow);
	data_to_send[_cnt++]=BYTE3(ANO->S_eff); 
	data_to_send[_cnt++]=BYTE2(ANO->S_eff);
	data_to_send[_cnt++]=BYTE1(ANO->S_eff);
	data_to_send[_cnt++]=BYTE0(ANO->S_eff);
	data_to_send[_cnt++]=BYTE3(ANO->S_pull); 
	data_to_send[_cnt++]=BYTE2(ANO->S_pull);
	data_to_send[_cnt++]=BYTE1(ANO->S_pull);
	data_to_send[_cnt++]=BYTE0(ANO->S_pull);
	data_to_send[_cnt++]=BYTE1(ANO->S_thr);
	data_to_send[_cnt++]=BYTE0(ANO->S_thr);
	data_to_send[_cnt++]=BYTE1(ANO->S_rmp);
	data_to_send[_cnt++]=BYTE0(ANO->S_rmp);
	data_to_send[_cnt++]=BYTE3(ANO->t0);
	data_to_send[_cnt++]=BYTE2(ANO->t0);
	data_to_send[_cnt++]=BYTE1(ANO->t0);
	data_to_send[_cnt++]=BYTE0(ANO->t0);
	data_to_send[_cnt++]=BYTE3(ANO->t1);
	data_to_send[_cnt++]=BYTE2(ANO->t1);
	data_to_send[_cnt++]=BYTE1(ANO->t1);
	data_to_send[_cnt++]=BYTE0(ANO->t1);
	data_to_send[_cnt++]=BYTE1(ANO->clock);
	data_to_send[_cnt++]=BYTE0(ANO->clock);

	data_to_send[3] = _cnt-4;
	
	
	for( ;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	dat_sent(USART1,data_to_send, _cnt);
}
/*
初始化蓝牙，将蓝牙配置位230400波特率
*/
uint8_t Bluetooth(void)
{	
	
	uint32_t over_time;//超时
	
	for(uint8_t i=0;i<12;i++)
		{
			
			clear_USART1_RX_buff();
			set_USART1_Baud(baud_rate[i]);
			
			PrintString(USART1,"AT");
			//收到至少两个字符的反馈
			for(over_time=0x3fffff;USART_RX_FLAG<2&&over_time>0;over_time--);
							
			if(over_time==0)continue;	//超时
							  	
			else if(USART1_RX_CATCH[0]=='O'&&USART1_RX_CATCH[1]=='K')//收到正确回复	
					{
						clear_USART1_RX_buff();
						PrintString(USART1,"AT+BAUDB");//HC-06指令
						for(over_time=0x3fffff;USART_RX_FLAG<2&&over_time>0;over_time--);
					if(USART1_RX_CATCH[0]=='O'&&USART1_RX_CATCH[1]=='K')
							return SUCCESS;//结束程序
					else	//
						{
							PrintString(USART1,"AT+UART=921600,0,0,");//HC-05指令
							for(over_time=0x3fffff;USART_RX_FLAG<2&&over_time>0;over_time--);
							if(USART1_RX_CATCH[0]=='O'&&USART1_RX_CATCH[1]=='K')
							return SUCCESS;//结束程序
							else continue ;
						}
						
					}
		}												
			return ERROR;									
}

void set_USART1_Baud(uint32_t baud)
{
		USART_InitTypeDef	USART_Ini;
	
		USART_Ini.USART_WordLength=USART_WordLength_8b;
		USART_Ini.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//关闭硬件流控制
		USART_Ini.USART_StopBits=USART_StopBits_1;//一位停止位
		USART_Ini.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
		USART_Ini.USART_Parity=USART_Parity_No;
		USART_Ini.USART_BaudRate=baud;
		
	USART_Init(USART1,&USART_Ini);
	show_string(0,0,"buad:",&Font8X16,COLOR);
	show_uint32(40,0,baud,7,&Font8X16,COLOR);

}
void clear_USART1_RX_buff()
{
	for(uint16_t i=0;i<USART_RX_FLAG;i++)
	USART1_RX_CATCH[i]=0;
	USART_RX_FLAG=0;
}

