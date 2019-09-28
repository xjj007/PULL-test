#ifndef	HX711_C
#define HX711_C

#include	"HX711.H"

uint32_t weight_sourse=0;
uint32_t set_zero=18799;
float GapValue=445.0;
uint8_t HX711_FLAG=1;
//max pull=5kg=5000g
//24bit ADC=16777216 128


void HX711_SET()
{
	GPIO_InitTypeDef	GPIO_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_config.GPIO_Pin	=	PD_SCK_Pin;
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_config.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PD_SCK,&GPIO_config);
	
	GPIO_config.GPIO_Pin	=	DOUT_Pin;
	GPIO_Init(GPIO_DOUT,&GPIO_config);
	GPIO_ResetBits(GPIO_PD_SCK,PD_SCK_Pin);//拉低时钟
	GPIO_SetBits(GPIO_DOUT,DOUT_Pin);
	
	GPIO_config.GPIO_Pin	=	DOUT_Pin;
	GPIO_config.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIO_DOUT,&GPIO_config);
	
	
}

uint32_t read_HX711()
{
	unsigned char i; 
  HX711_FLAG=0;
	//delay_us(1);
	//GPIO_ResetBits(GPIO_PD_SCK,PD_SCK_Pin);//拉低时钟
	//GPIO_SetBits(GPIO_PD_SCK,PD_SCK_Pin);
  	
  	weight_sourse=0;		
  	for(i=0;i<24;i++)
	{ 
	 GPIO_SetBits(GPIO_PD_SCK,PD_SCK_Pin);
	//delay_us(1);
	weight_sourse=weight_sourse<<1; 
	GPIO_ResetBits(GPIO_PD_SCK,PD_SCK_Pin);//拉低时钟
	//delay_us(1);
	if(Read_DOUT==SET)
			weight_sourse++; 
	} 
 	GPIO_SetBits(GPIO_PD_SCK,PD_SCK_Pin);
    weight_sourse=weight_sourse^0x800000;//第25个脉冲下降沿来时，转换数据
	//delay_us(1);
	GPIO_ResetBits(GPIO_PD_SCK,PD_SCK_Pin);//拉低时钟
	HX711_FLAG=1;
	return weight_sourse;

}


void set_GapValue(uint16_t vaule)
{
	GapValue=vaule;
}
//拉力机归零
void HX711set_to_0()
{
	while(Read_DOUT==SET);
	set_zero=read_HX711();

}

uint16_t get_wetght()
{	
	int32_t	temp=	read_HX711()-set_zero;
	if(temp<0)temp=-temp;
		return (uint16_t)(((float)(temp))/GapValue);
}

#endif	/*HX711_C*/
