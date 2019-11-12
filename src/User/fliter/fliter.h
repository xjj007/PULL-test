#ifndef FLITER_H
#define	FLITER_H
#include "stm32f10x.h"   

//滤波器的通过频率要可以根据转速实时调整
//因此，每隔一段时间都要重新计算截至频率和滤波系数

//#define a		10		//低通滤波器滤波系数
////滤波系数越小，滤波结果越平稳，但是灵敏度越低；滤波系数越大，灵敏度越高，但是滤波结果越不稳定。
//#define f		//a/(6.18*t)	//截止频率	a:滤波系数，t：采样时间间隔，单位S

//电流电压的均值滤波缓存
#define cur_area	10
#define	bat_area	10

enum a_lowpass		//低通滤波器的参数a
{	
	a_KEY,
	a_poten,
	a_bat,
	a_cur,
	a_weight,
	a_end,
};

enum Average_num	//均值滤波器
{
	A_KEY=0,
	A_poten,
	A_BAT,
	A_CUR,
	A_weight,
	e_average_num,
};

typedef struct	{
	float			current;	 //电流
	float			voltage;	 //电压
	float 			power;		 //功率
	float			efficiency;//桨效
	float			pull;			 //拉力
	uint16_t			throttle;	 //油门位置
	uint16_t  		rmp;			 //转速
	float			tempture0; //温度0
	float 			tempture1; //温度1
	uint16_t			clock;		 //时钟
	
}all_data;
 
typedef struct	output_data{	//输出显示预览
	uint16_t		current;	 	//电流		99.99A
	uint16_t		voltage;	 	//电压		16.51V
	uint16_t 	power;		 	//功率		1500W
	uint16_t		efficiency;		//桨效		10.123G/W
	uint16_t		pull;			 //拉力		4999g
	uint16_t		throttle;	 	//油门 	4096;(99%)//两种显示模式
	uint16_t  	rmp;			//转速		3500r/S
	uint16_t	tempture0; //温度0
	uint16_t 	tempture1; //温度1
	int			clock_overturn;//翻转的时钟
	uint32_t	clock;		 //时钟	
	//有两种显示模式，固定时间翻转至定值或者记录当前精确时间
	
}output_data;

 


void Low_Pass_init(void);
void LOWPASS(void);
void fliter_parameter_adjust(void);
uint16_t KEY_Pass(void);
uint16_t poten_Pass(void);
void HighPass(void);
void PWM1_Out_H(uint16_t vaule);
void parameter_cuc(void);

#endif	/*FLITER_H*/
