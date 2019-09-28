#ifndef FLASH_C
#define FLASH_C

#include	"flash.h"


extern uint16_t current_offset;//电流计偏移量
extern float	  current_ratio;
extern float    voltage_ratio;
extern uint16_t voltage_offset;
extern uint8_t baud;
extern uint16_t step,step_t;//每步长度
extern uint16_t fre;
 typedef struct
{	
	uint16_t firs_key;
	uint16_t cur_offset;
	uint16_t vlo_offset;
	uint16_t _step;
	uint16_t _step_t;
	uint16_t pwm_fre;
	uint16_t _buad;
	float		voltage_ratio;
	float		current_ratio;
	uint16_t run_time;
}Flash_data;

//拉力计的偏移值不储存，上电自动复位
void flash_init()
{	
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
	FLASHStatus	=	FLASH_ErasePage(START_ADDR);
	
	FLASH_ProgramHalfWord(first_key_addr,first_key);
	FLASH_ProgramHalfWord(cur_offset_addr,current_offset);
	FLASH_ProgramHalfWord(vlo_offset_addr,voltage_offset);
	FLASH_ProgramHalfWord(_step_addr,step);
	FLASH_ProgramHalfWord(_step_t_addr,step_t);
	FLASH_ProgramHalfWord(pwm_fre_addr,fre);
	FLASH_ProgramHalfWord(_buad_addr,baud);
	FLASH_ProgramWord(voltage_ratio_addr,BYTE(voltage_ratio));
	FLASH_ProgramWord(current_ratioaddr,BYTE(current_ratio));
}

void read_flash()
{
	current_offset=(*(__IO uint32_t *)cur_offset_addr);
	voltage_offset=(*(__IO uint32_t *)vlo_offset_addr);
	step		  =(*(__IO uint32_t *)_step_addr);
	step_t		  =(*(__IO uint32_t *)_step_t_addr);
	fre			  =(*(__IO uint32_t *)pwm_fre_addr);
	baud		  =(*(__IO uint32_t *)_buad_addr);
	voltage_ratio	=(*(__IO float *)voltage_ratio_addr);
	current_ratio	=(*(__IO float *)current_ratioaddr);
}

void write_flash()
{
		flash_init();
}

#endif	/*FLASH_C*/
