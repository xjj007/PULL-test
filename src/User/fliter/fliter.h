#ifndef FLITER_H
#define	FLITER_H
#include "stm32f10x.h"   

//�˲�����ͨ��Ƶ��Ҫ���Ը���ת��ʵʱ����
//��ˣ�ÿ��һ��ʱ�䶼Ҫ���¼������Ƶ�ʺ��˲�ϵ��

//#define a		10		//��ͨ�˲����˲�ϵ��
////�˲�ϵ��ԽС���˲����Խƽ�ȣ�����������Խ�ͣ��˲�ϵ��Խ��������Խ�ߣ������˲����Խ���ȶ���
//#define f		//a/(6.18*t)	//��ֹƵ��	a:�˲�ϵ����t������ʱ��������λS

//������ѹ�ľ�ֵ�˲�����
#define cur_area	10
#define	bat_area	10

enum a_lowpass		//��ͨ�˲����Ĳ���a
{	
	a_KEY,
	a_poten,
	a_bat,
	a_cur,
	a_weight,
	a_end,
};

enum Average_num	//��ֵ�˲���
{
	A_KEY=0,
	A_poten,
	A_BAT,
	A_CUR,
	A_weight,
	e_average_num,
};

typedef struct	{
	float			current;	 //����
	float			voltage;	 //��ѹ
	float 			power;		 //����
	float			efficiency;//��Ч
	float			pull;			 //����
	uint16_t			throttle;	 //����λ��
	uint16_t  		rmp;			 //ת��
	float			tempture0; //�¶�0
	float 			tempture1; //�¶�1
	uint16_t			clock;		 //ʱ��
	
}all_data;
 
typedef struct	output_data{	//�����ʾԤ��
	uint16_t		current;	 	//����		99.99A
	uint16_t		voltage;	 	//��ѹ		16.51V
	uint16_t 	power;		 	//����		1500W
	uint16_t		efficiency;		//��Ч		10.123G/W
	uint16_t		pull;			 //����		4999g
	uint16_t		throttle;	 	//���� 	4096;(99%)//������ʾģʽ
	uint16_t  	rmp;			//ת��		3500r/S
	uint16_t	tempture0; //�¶�0
	uint16_t 	tempture1; //�¶�1
	int			clock_overturn;//��ת��ʱ��
	uint32_t	clock;		 //ʱ��	
	//��������ʾģʽ���̶�ʱ�䷭ת����ֵ���߼�¼��ǰ��ȷʱ��
	
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
