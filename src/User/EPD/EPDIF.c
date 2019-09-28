#include "epdif.h"
#include "port.h"

/*GPIO设置*/

//extern SPI_HandleTypeDef hspi1;

EPD_Pin epd_cs_pin =	//结构体！！！
{
  SPI_CS_GPIO_Port,
  SPI_CS_Pin,
};


EPD_Pin epd_dc_pin = 
{
  DC_GPIO_Port,
  DC_Pin,
};

EPD_Pin pins[4];//定义一个名称pins的EPD_Pin类型的结构体，只定义，不赋值

void EpdDigitalWriteCallback(int pin_num, int value) 
	{
		if(pin_num==CS_PIN){
				if (value == HIGH)
				{
				GPIO_SetBits(SPI_CS_GPIO_Port,SPI_CS_Pin);
				//GPIO_SetBits((GPIO_TypeDef*)pins[pin_num].port, pins[pin_num].pin);
				} 
			else 
				{
				GPIO_ResetBits(SPI_CS_GPIO_Port,SPI_CS_Pin);
				//GPIO_ResetBits((GPIO_TypeDef*)pins[pin_num].port, pins[pin_num].pin);
				}
		}

	else if(pin_num==DC_PIN)
			{

				if (value == HIGH)
				{
			GPIO_SetBits(DC_GPIO_Port,DC_Pin);
			//GPIO_SetBits((GPIO_TypeDef*)pins[pin_num].port, pins[pin_num].pin);
				} 
			else 
				{
				GPIO_ResetBits(DC_GPIO_Port,DC_Pin);
				//GPIO_ResetBits((GPIO_TypeDef*)pins[pin_num].port, pins[pin_num].pin);
				}

			}
}

int EpdDigitalReadCallback(int pin_num)
	{
  if (GPIO_ReadInputDataBit(pins[pin_num].port, pins[pin_num].pin) == Bit_SET) 
		{
			return HIGH;
		} 
		else 
			{
				return LOW;
			}
	}

	//延时函数
void EpdDelayMsCallback(unsigned int delaytime) 
	{
  delay_ms(delaytime);
	}
//调用硬件SPI
void EpdSpiTransferCallback(unsigned char data) 
	{	
		uint16_t temp=0x00;
		temp=temp|data;
		GPIO_ResetBits(SPI_CS_GPIO_Port, SPI_CS_Pin);
		//SPI_BiDirectionalLineConfig(SPI1,SPI_Direction_Tx);
		SPI_I2S_SendData(SPI1, temp);
		
		//while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==SET);
		delay_us(2);
		GPIO_SetBits(SPI_CS_GPIO_Port, SPI_CS_Pin);
	}

int EpdInitCallback(void)
	{
		pins[CS_PIN] = epd_cs_pin;
		pins[DC_PIN] = epd_dc_pin;
		return 0;
	}

