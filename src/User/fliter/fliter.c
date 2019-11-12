#ifndef FLITER_C
#define	FLITER_C
#include	"fliter.h"
#include	"ADC.H"
#include	"dispatcher.h"
#include	"rev.h"
#include	"temperature.h"

#ifdef USE_HIGN_PASS
float b=0.05;//��ͨ�˲����˲�����
#endif

float a_parameter[a_end];//��ͨ�˲�������


extern uint16_t current_offset;//������ƫ����
extern float	  current_ratio;
extern float    voltage_ratio;
extern uint16_t voltage_offset;
extern uint16_t ADC_sourse[4];
extern uint16_t ADC_Fliter[4];
extern uint16_t HX_weight;
extern uint16_t rmp;
/*
��ֵ�˲�������
�����������ڴ����������»��нϴ������
*/
#define AverageBuff		16	//��ֵ�˲�������
uint16_t ADC_Average[e_average_num][AverageBuff];
uint16_t Average_Result[e_average_num]={0};//��ֵ�˲����
u8 Average_Point=0;


output_data	out_data;
all_data data;

void Low_Pass_init(void)	//�˲���������ʼ��
{
	a_parameter[a_KEY]=		0.99;
	a_parameter[a_poten]=	0.9999;
	a_parameter[a_bat]=		0.1;
	a_parameter[a_cur]=		0.1;
	a_parameter[a_weight]=	1;
	out_data.clock_overturn=100;

}

/*
��ֵ�˲�
���룺ADC_sourse[4],HX_weight
�����Average_Result[e_average_num]

*/
void Average()
{
	ADC_Average[A_KEY][Average_Point]		=	ADC_sourse[KEY];
	ADC_Average[A_poten][Average_Point]		=	ADC_sourse[poten];
	ADC_Average[A_BAT][Average_Point]		=	ADC_sourse[BAT];
	ADC_Average[A_CUR][Average_Point]		=	ADC_sourse[CUR];
	ADC_Average[A_weight][Average_Point]	=	HX_weight;
	
	Average_Point++;
	if(Average_Point>=AverageBuff)Average_Point	=	0;
	
	u32 temp=0;
	for(u8 i=0;i<AverageBuff;i++)temp+=ADC_Average[A_KEY][i];
	Average_Result[A_KEY]=temp/AverageBuff;
	
	temp=0;
	for(u8 i=0;i<AverageBuff;i++)temp+=ADC_Average[A_poten][i];
	Average_Result[A_poten]=temp/AverageBuff;

	temp=0;
	for(u8 i=0;i<AverageBuff;i++)temp+=ADC_Average[A_BAT][i];
	Average_Result[A_BAT]=temp/AverageBuff;
	
	temp=0;
	for(u8 i=0;i<AverageBuff;i++)temp+=ADC_Average[A_CUR][i];
	Average_Result[A_CUR]=temp/AverageBuff;
	
	temp=0;
	for(u8 i=0;i<AverageBuff;i++)temp+=ADC_Average[A_weight][i];
	Average_Result[A_weight]=temp/AverageBuff;
	
}

//==================================//
//��ͨ�˲���
//Y(n)=a*X(n)+(1-a)*Y(n-1)
//a:�˲�ϵ��
//X(n)���β���ֵ
//Y(n)�˲����ֵ
//Y(n-1)�ϴ��˲����ֵ
//���룺���β������ݣ��ϴ��������
//�����˲�ֵ
//���ڼ�������Ҫʹ�õ�Pi,���ԣ�����Ҫ��������ת��
//==================================//
void LOWPASS(void)
{
	uint8_t	i=0;
	Average();
	
	ADC_Fliter[poten]=a_parameter[a_poten]*((float)Average_Result[A_poten])
				+((1-a_parameter[a_poten])*((float)ADC_Fliter[poten]));

	ADC_Fliter[BAT]=a_parameter[a_bat]*((float)Average_Result[A_BAT])
				+((1-a_parameter[a_bat])*((float)ADC_Fliter[BAT]));

	ADC_Fliter[CUR]=a_parameter[a_cur]*((float)Average_Result[A_CUR])
				+((1-a_parameter[a_cur])*((float)ADC_Fliter[CUR]))+0.5;

	
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
������ת��Ϊ�������
*/
void parameter_cuc(void)
{
	data.current	=	(ADC_Fliter[CUR]+100-current_offset)*current_ratio;//
	data.voltage	=	(ADC_Fliter[BAT]+100-voltage_offset)*voltage_ratio;//��λ��v
	data.power		=	(data.current*data.voltage);//��λ��W
	data.pull		=	Average_Result[A_weight];//��λ��g
	data.efficiency	=	data.pull/data.power;//��λ��g/w
	data.throttle	=	ADC_Fliter[poten];		//��Χ��0-4000
	if(data.throttle>4000)data.throttle=4000;	//�޷�
	//data.tempture0	=	Read_MLX_IIC_Data(0x07)*0.02-273.15; //�����ʽ�ǳ��ģ�
	//data.tempture1	=	0;//Read_MLX_IIC_Data(0x06);;//����Դ
	data.rmp		=	rmp;
}


#ifdef USE_AVERAGE_FLITER

#endif	/*USE_AVERAGE_FLITER*/

//��ͨ�˲���
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
