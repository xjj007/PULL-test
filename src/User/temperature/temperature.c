#ifndef TEMPERATURE_C
#define	TEMPERATURE_C

#include	"temperature.h"
 

void MLX90614_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_MLX, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = MLX_SDA_Pin|MLX_SCL_Pin;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIO_MLX_GPIO, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIO_MLX_GPIO,MLX_SDA_Pin|MLX_SCL_Pin);	

}
static	void IIC_delay()
{
	for(u8 i=40;i;i--);	//15几乎是极限了

}	
	
static void IIC_Start()
{
	MLX_SDIN_Set();
	//IIC_delay();
	MLX_SCLK_Set();
	IIC_delay();
	
	MLX_SDIN_Clr();
	IIC_delay();
	MLX_SCLK_Clr();
	//IIC_delay();
}

static void IIC_Stop()
{
	
	IIC_delay();
	MLX_SCLK_Clr();
	MLX_SDIN_Clr();
	IIC_delay();
	MLX_SCLK_Set() ;
	IIC_delay();
	MLX_SDIN_Set();
	
}

static void IIC_Wait_Ack()
{	
	//MLX_SDIN_Clr();
	//while(Read_MLX_SDA());
	//for(u8 a=20;a;a--);
	MLX_SCLK_Set();	//拉高时钟，读取应答
	IIC_delay();
	MLX_SCLK_Clr();//读取完毕，拉低时钟
	IIC_delay();
}
static void IIC_Sent_Ack()
{	
	MLX_SDIN_Clr();	
	//for(u8 a=20;a;a--);
	MLX_SCLK_Set();
	IIC_delay();
	MLX_SCLK_Clr();
	IIC_delay();
}

static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	MLX_SCLK_Clr();
	
	for(i=0;i<8;i++)		
	{
		//IIC_delay();
		m=da;
		//	MLX_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		MLX_SDIN_Set();
		else MLX_SDIN_Clr();	//数据改变后可以立即拉高时钟读取
			da=da<<1;
		//IIC_delay();
		MLX_SCLK_Set();		//拉高时钟
		IIC_delay();		//等待读取
		MLX_SCLK_Clr();		//读取完毕，拉低时钟
		for(u8 a=10;a;a--);
	}
	
		for(u8 a=10;a;a--);	//延时等待稳定
		MLX_SDIN_Set();		//释放SDA
		IIC_delay();		//等待应答
		//IIC_delay();		//等待应答

}
static uint8_t Read_IIC_Byte()
{	
	u8 temp=0;
	for(u8 i=8;i;i--)
	{
	MLX_SDIN_Set();		//释放SDA
	IIC_delay();		//等待拉低
	MLX_SCLK_Set();		//拉高时钟，开始读

	if(Read_MLX_SDA())
	temp|=(1<<(i-1));
	IIC_delay();
	MLX_SCLK_Clr();	//读完拉低
	//IIC_delay();	
	
	}
	return temp;

}

uint16_t Read_MLX_IIC_Data(u8 res)
{	
	u16 dat=0;
	u16 PEC=0;
	IIC_Start();
	Write_IIC_Byte(MLX90614_I2CADDR<<1);	
	IIC_Wait_Ack();
	Write_IIC_Byte(res);
	IIC_Wait_Ack();
	IIC_Start();
	Write_IIC_Byte((MLX90614_I2CADDR<<1)+1);
	IIC_Wait_Ack();
	dat=Read_IIC_Byte();
	IIC_Sent_Ack();
	dat|=(Read_IIC_Byte()<<8);
	IIC_Sent_Ack();
	PEC=Read_IIC_Byte();
	IIC_Stop();
	return dat;
	
	
}	
	
#endif	/*TEMPERATURE_C*/



