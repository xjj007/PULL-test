/**
  ******************************************************************************
  * @file    soft_I2C.H
  * @author  XJJ
  * @version ######
  * @date    2018年2月24日
  * @brief   这个文件用来配置软件I2C
  ******************************************************************************
  */

//硬件调用I2C过程，

#ifndef	soft_I2C_H
#define	soft_I2C_H

#define OLED_SDA_port		GPIO_Pin_7
#define	OLED_SCL_port		GPIO_Pin_6


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,OLED_SCL_port)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,OLED_SCL_port)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,OLED_SDA_port)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,OLED_SDA_port)

//#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//RES
//#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void I2C_port_init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Byte(unsigned char IIC_Byte);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void OLED_WR_Byte(unsigned dat,unsigned cmd);



#endif	/*soft_I2C_H*/
