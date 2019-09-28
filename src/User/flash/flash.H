#ifndef FLASH_H
#define	FLASH_H

#include	"stm32f10x.h"
#define	BYTE(dwTemp)		( *( (uint32_t *)(&dwTemp)) )

//这个是啥，我怎么看不太懂
#define first_key					0xfabf
	
/*在这里手动指定各个变量的基地址*/
/*这个可以改为自动的*/
#define	base_addr					((uint32_t)0x08000000)//内存基地址
#define	page						0X3F		//内存页数
#define START_ADDR					((uint32_t)(base_addr+page*0x400))
#define	first_key_addr				((uint32_t)START_ADDR+0x2*0)
#define	cur_offset_addr				((uint32_t)START_ADDR+0x2*1)
#define	vlo_offset_addr				((uint32_t)START_ADDR+0x2*2)
#define	_step_addr					((uint32_t)START_ADDR+0x2*3)
#define	_step_t_addr				((uint32_t)START_ADDR+0x2*4)
#define	pwm_fre_addr				((uint32_t)START_ADDR+0x2*5)
#define	_buad_addr					((uint32_t)START_ADDR+0x2*6)
#define	voltage_ratio_addr			((uint32_t)START_ADDR+0x2*7)
#define	current_ratioaddr			((uint32_t)START_ADDR+0x2*9)
#define	run_time_addr				((uint32_t)START_ADDR+0x2*10)



void flash_init(void);
void write_flash(void);
void read_flash(void);

#endif	/*FLASH_H*/
