#ifndef TEMPERATURE_C
#define	TEMPERATURE_C

#include	"temperature.h"
 

void MLX90614_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_MLX, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = MLX_SDA_Pin|MLX_SCL_Pin;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIO_MLX_GPIO, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIO_MLX_GPIO,MLX_SDA_Pin|MLX_SCL_Pin);	

}
static	void IIC_delay()
{
	for(u8 i=40;i;i--);	//15�����Ǽ�����

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
	MLX_SCLK_Set();	//����ʱ�ӣ���ȡӦ��
	IIC_delay();
	MLX_SCLK_Clr();//��ȡ��ϣ�����ʱ��
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
		else MLX_SDIN_Clr();	//���ݸı�������������ʱ�Ӷ�ȡ
			da=da<<1;
		//IIC_delay();
		MLX_SCLK_Set();		//����ʱ��
		IIC_delay();		//�ȴ���ȡ
		MLX_SCLK_Clr();		//��ȡ��ϣ�����ʱ��
		for(u8 a=10;a;a--);
	}
	
		for(u8 a=10;a;a--);	//��ʱ�ȴ��ȶ�
		MLX_SDIN_Set();		//�ͷ�SDA
		IIC_delay();		//�ȴ�Ӧ��
		//IIC_delay();		//�ȴ�Ӧ��

}
static uint8_t Read_IIC_Byte()
{	
	u8 temp=0;
	for(u8 i=8;i;i--)
	{
	MLX_SDIN_Set();		//�ͷ�SDA
	IIC_delay();		//�ȴ�����
	MLX_SCLK_Set();		//����ʱ�ӣ���ʼ��

	if(Read_MLX_SDA())
	temp|=(1<<(i-1));
	IIC_delay();
	MLX_SCLK_Clr();	//��������
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



