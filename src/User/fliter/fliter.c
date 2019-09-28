#ifndef FLITER_C
#define	FLITER_C
#include	"fliter.h"
#include	"ADC.H"
#include	"dispatcher.h"
#include	"rev.h"

#ifdef USE_HIGN_PASS
float b=0.05;//高通滤波器滤波参数
#endif

float a_parameter[a_end];//低通滤波器参数


extern uint16_t current_offset;//电流计偏移量
extern float	  current_ratio;
extern float    voltage_ratio;
extern uint16_t voltage_offset;
extern uint16_t ADC_sourse[end];
extern uint16_t ADC_Fliter[end];

output_data	out_data;
all_data data;

void Low_Pass_init(void)	//滤波器参数初始化
{
	a_parameter[a_KEY]=		0.5;
	a_parameter[a_poten]=	0.05;
	a_parameter[a_bat]=		0.15;
	a_parameter[a_cur]=		0.1;
	a_parameter[a_weight]=0.05;
	out_data.clock_overturn=100;

}

//==================================//
//低通滤波器
//Y(n)=a*X(n)+(1-a)*Y(n-1)
//a:滤波系数
//X(n)本次采样值
//Y(n)滤波输出值
//Y(n-1)上次滤波输出值
//输入：本次采样数据，上次输出数据
//返回滤波值
//由于计算中需要使用到Pi,所以，必须要进行类型转换
//==================================//
void LOWPASS(void)
{
	uint8_t	i=0;
	
	//ADC_Fliter[weight]=ADC_sourse[weight];//???这里为什么是这样
	
	ADC_Fliter[poten]=a_parameter[poten]*((float)ADC_sourse[poten])
				+((1-a_parameter[poten])*((float)ADC_Fliter[poten]));

	ADC_Fliter[BAT]=a_parameter[BAT]*((float)ADC_sourse[BAT])
				+((1-a_parameter[BAT])*((float)ADC_Fliter[BAT]));

	ADC_Fliter[CUR]=a_parameter[CUR]*((float)ADC_sourse[CUR])
				+((1-a_parameter[CUR])*((float)ADC_Fliter[CUR]));

	
}

uint16_t KEY_Pass()
{
	return ADC_Fliter[KEY]=a_parameter[KEY]*((float)ADC_sourse[KEY])
										+((1-a_parameter[KEY])*((float)ADC_Fliter[KEY]));
}
uint16_t poten_Pass()
{
	return	ADC_Fliter[poten]=a_parameter[poten]*((float)ADC_sourse[poten])
										+((1-a_parameter[poten])*((float)ADC_Fliter[poten]));
}

/*
将参数转化为国标参数
*/
void parameter_cuc(void)
{
	data.current	=	(ADC_Fliter[CUR]-current_offset+100)*current_ratio;//
	data.voltage	=	(ADC_Fliter[BAT]-voltage_offset+100)*voltage_ratio;//单位：v
	data.power	=	(data.current*data.voltage);//单位：W
	data.pull	=	ADC_Fliter[weight];//单位：g
	data.efficiency	=	data.pull/data.power;//单位：g/w
	data.throttle	=	ADC_Fliter[poten];		//范围是0-4000
	if(data.throttle>4000)data.throttle=4000;
	data.tempture0	=	ADC_Fliter[CUR];
	data.tempture1	=	ADC_sourse[CUR];
	data.rmp	=	get_rmp();
}


#ifdef USE_AVERAGE_FLITER

uint16_t cur_buf[10]={0};
uint16_t bat_buf[10]={0};

//对电流和电压进行均值滤波
//
uint16_t average_fliter(uint8_t vaule)
{
	uint16_t m;
	uint16_t temp_buff[cur_area]={0};
	uint16_t i;
	cur_buf[i]=ADC_sourse[CUR];
	bat_buf[i]=ADC_sourse[BAT];
	i++;
	
	if(i==cur_area)
	{
	for(uint8_t a=0;a<cur_area;a++)
		{
		
		}
	
	}
	
}
#endif	/*USE_AVERAGE_FLITER*/

#ifdef USE_HIGN_PASS
void HighPass(void)
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
	HighPassACC[i]=((float)Accel[i]-(float)OldAccel[i]+HighPassACC[i])*b;
	}
	
}

#endif
#endif 	/*FLITER_C*/

