#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#include	"stm32f10x.h"


#define	RCC_APB2Periph_MLX	RCC_APB2Periph_GPIOB
#define	GPIO_MLX_GPIO	GPIOB
#define	GPIO_MLX_SDA	GPIOB
#define	GPIO_MLX_SCL	GPIOB
#define	MLX_SDA_Pin		GPIO_Pin_12
#define	MLX_SCL_Pin		GPIO_Pin_13

#define MLX_SCLK_Clr() GPIO_ResetBits(GPIO_MLX_SCL,MLX_SCL_Pin)//SCL
#define MLX_SCLK_Set() GPIO_SetBits(GPIO_MLX_SCL,MLX_SCL_Pin)

#define MLX_SDIN_Clr() GPIO_ResetBits(GPIO_MLX_SDA,MLX_SDA_Pin)//SDA
#define MLX_SDIN_Set() GPIO_SetBits(GPIO_MLX_SDA,MLX_SDA_Pin)

#define	Read_MLX_SDA()		GPIO_ReadInputDataBit(GPIO_MLX_SDA,MLX_SDA_Pin)




#define MLX90614_I2CADDR 0x5A

// RAM
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 	0x06
#define MLX90614_TOBJ1 	0x07
#define MLX90614_TOBJ2 	0x08
// EEPROM
#define MLX90614_TOMAX 	0x20
#define MLX90614_TOMIN 	0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_EMISS 	0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x0E
#define MLX90614_ID1 0x3C
#define MLX90614_ID2 0x3D
#define MLX90614_ID3 0x3E
#define MLX90614_ID4 0x3F

uint16_t Read_MLX_IIC_Data(u8 res);
void MLX90614_Init();

#endif	/*TEMPERATURE_H*/
