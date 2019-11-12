#ifndef	ADC_C
#define	ADC_C

#include	"ADC.H"
#include "dispatcher.h"
#include	"fliter.h"

extern uint8_t key_vaule_buff;
uint16_t ADC_sourse[4]={0,0,0,0};//外设传回的源数据(DMA自动传输的源数据)
uint16_t ADC_Fliter[4]={0,0,0,0};//滤波后的数据(还没有经过单位转化)

uint16_t voltage_offset=85;
float  	 voltage_ratio=0.008862;


uint16_t current_offset=115;//电流计偏移量
float	   current_ratio=0.015072;//这个计算可能会损失精度


bool upKeyPress=false;
bool downKeyPress=false;
bool leftKeyPress=false;
bool RightKeyPress=false;


u8 key_effect	=	0;	//记录按键有效次数
u8 OLD_KEY	=	no_key;	//上一次的按键键值
extern  all_data data;

/*
这个需要写4个吗
*/
bool upKey()
{
	if(upKeyPress	==	true)
	{
	
		upKeyPress	=	false;
		return true;
	}
	return false;

}

bool downKey()
{
	if(upKeyPress	==	true)
	{
	
		upKeyPress	=	false;
		return true;
	}
	return false;

}
bool leftKey()
{
	if(upKeyPress	==	true)
	{
	
		upKeyPress	=	false;
		return true;
	}
	return false;

}
bool rightKey()
{
	if(upKeyPress	==	true)
	{
	
		upKeyPress	=	false;
		return true;
	}
	return false;

}
/*
采样电阻 0.5mR,增益100，
精确比率：0.01611328125
放大100倍，以uint16储存，显示自动加小数点
*/

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = 		ADC_PIN1|ADC_PIN2|		
										ADC_PIN3|ADC_PIN4;																	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN|GPIO_Mode_IN_FLOATING;

	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}

static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	DMA_DeInit(ADC_DMA_CHANNEL);	// 复位DMA控制器
	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&(ADC_x->DR));	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_sourse;		//存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				// 数据源来自外设
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;					// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 		// 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						// 循环传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 禁止存储器到存储器模式，因为是从外设到存储器
	
	
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC 模式配置

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	// 转换通道个数	
	
	ADC_Init(ADC_x, &ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); // 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_239Cycles5);

	

	ADC_DMACmd(ADC_x, ENABLE);
	ADC_Cmd(ADC_x, ENABLE);
	
	
	ADC_ResetCalibration(ADC_x);// 初始化ADC 校准寄存器  
	while(ADC_GetResetCalibrationStatus(ADC_x));	// 等待校准寄存器初始化完成

	ADC_StartCalibration(ADC_x);// ADC开始校准
	while(ADC_GetCalibrationStatus(ADC_x));	// 等待校准完成
	
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);// 由于没有采用外部触发，所以使用软件触发ADC转换 
}

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
//电压校准
void voltage_calibration(int offset)
{
	voltage_offset=offset/100;
}

void current_calibrater()
{	
	uint16_t temp=0;
	for(uint8_t i=0;i<10;i++)
	{	delay_ms(1);
		temp+=ADC_Fliter[CUR];
		
	}
	current_offset=temp/10;
}

//这个需要重新采样
//返回ADC对应的键值
uint8_t key_vaule()
{	
	
	if(ADC_Fliter[KEY]>580&&ADC_Fliter[KEY]<600)
		return no_key;
	if(ADC_Fliter[KEY]>650&&ADC_Fliter[KEY]<700)
		return down;
	if(ADC_Fliter[KEY]>795&&ADC_Fliter[KEY]<835)
		return up;
	if(ADC_Fliter[KEY]>1000&&ADC_Fliter[KEY]<1040)
		return right;
	if(ADC_Fliter[KEY]>1335&&ADC_Fliter[KEY]<1400)
		return mid;
	if(ADC_Fliter[KEY]>2000&&ADC_Fliter[KEY]<2050)
		return left;
	return ERROR;
	
}

//获取键值，按键扫描速度100hz

void ScanKey()
{

}


u8 getkey()
{
return 0;
}

u8  getK()
{
return 0;
}


/*
开机后首先运行
返回电池节数
电池下限电压调整为3.5V，上限4.35V
低于3.5V的电池就不应该继续测试了
主要目的是防止电池过放
电压参考范围：
  2V	-	4.35V	//1S	2v以下应该就挂了
5.5V	-	8.7V 	//2S
8.7V 	-	13.5V	//3S
13.6V	-	17.4V	//4S
17.5V	-	21.7V	//5S
21.8V	-	26.1V	//6S
26.1V	-	30.45V	//7S
30.6V	-	34.8V	//8S
34.9V	-	39.15V	//9S
39.2V	-	43.5V	//10S	//此电压硬件不支持
*/
u8 bettery_cells_check()
{	
	if(data.voltage<4.35)	return 1;
	if(data.voltage<8.7 )	return 2;
	if(data.voltage<13.5)	return 3;
	if(data.voltage<17.4)	return 4;
	if(data.voltage<21.7)	return 5;
	if(data.voltage<26.1)	return 6;
	if(data.voltage<30.45)	return 7;
	if(data.voltage<34.8)	return 8;
	if(data.voltage<39.15)	return 9;
	if(data.voltage<43.5)	return 10;
	return 0;	//错误！
}

#endif	/*ADC_C*/
