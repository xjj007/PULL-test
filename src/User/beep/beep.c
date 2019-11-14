#ifndef BEEP_C
#define	BEEP_C

#include	"beep.h"

//并不需要设置全局变量
//音量大小并不开放给用户设置
//蜂鸣器声音太小了
/*此变量以失效*/
uint16_t beep_fre	=	5000;//单位HZ，取值20-20000
uint16_t beep_Sound	=	0; //声音大小，0-10档		

typedef struct
{
	u16 B_fre;	//频率/HZ
	u8 	B_vol;	//大小
	u16 B_onT;	//时间/ms
}beep_seq;

const beep_seq beep[1]={1000,10,500};
const beep_seq B_power_on[5]={1000,10,500,2000,10,500,500,10,500};//开机音乐

/*
现在控制蜂鸣器还得指定声音的频率
*/

/*
蜂鸣器定义方法
*/

/*
蜂鸣器设置
可变音调
*/
void beep_set()
{
	TIM_TimeBaseInitTypeDef TIM_Base_SET;
	GPIO_InitTypeDef GPIO_config;
	TIM_OCInitTypeDef	TIM_OCSet={0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_config.GPIO_Mode	=	GPIO_Mode_AF_PP;//复用推挽
	GPIO_config.GPIO_Speed	=	GPIO_Speed_50MHz;
	
    /*beep GPIO set*/
	GPIO_config.GPIO_Pin	=	beep_GPIO_Pin;
	GPIO_Init(beep_GPIO,&GPIO_config);
	
	GPIO_SetBits(beep_GPIO,beep_GPIO_Pin);
	
	/*20-20000hz*/
	TIM_Base_SET.TIM_Prescaler		=	Beep_TIM_Prescaler;
	
	TIM_Base_SET.TIM_CounterMode	=	TIM_CounterMode_Up;
	TIM_Base_SET.TIM_Period			=	beep_TIM_Period;		//控制音调
	TIM_Base_SET.TIM_ClockDivision	=	TIM_CKD_DIV1;	
	TIM_TimeBaseInit(beep_TIM,&TIM_Base_SET);
       
	

	TIM_OCSet.TIM_OCMode			=	TIM_OCMode_PWM1;
	TIM_OCSet.TIM_OutputState		=	TIM_OutputState_Enable;
	TIM_OCSet.TIM_Pulse				=	beep_TIM_Pulse;//控制响度
	TIM_OCSet.TIM_OCPolarity		=	TIM_OCPolarity_High;

	TIM_OC1Init(beep_TIM,&TIM_OCSet);
	TIM_CtrlPWMOutputs(beep_TIM,ENABLE);
	TIM_OC1PreloadConfig(beep_TIM, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(beep_TIM, ENABLE);
	TIM_Cmd(beep_TIM, ENABLE);
	
}

/*
设置蜂鸣器声音频率和响度

*/
void set_beep_fre_vol(u16 fre, u16 vol)
{   
	beep_TIM->ARR	=   beep_FRE(fre);
    beep_TIM->CNT   =   beep_TIM->ARR;
	beep_TIM->CCR1	=   beep_Volume(vol,fre);
    beep_TIM->CR1   =   0x01;
}

/*
beep驱动
怎么知道序列长度呢
在声音格式里加入帧尾
输入：beep 叫声序列地址
*/
void beep_drive(beep_seq *seq)
{

}


#endif	/*BEEP_C*/
