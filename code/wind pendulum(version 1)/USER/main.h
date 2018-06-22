#ifndef MAIN_H
#define MAIN_H

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "Time3_pwm_mode.h"
#include <math.h>
void dis_play(int16_t num,u8 n);
void dis_play_out(long int num,u8 n);
void mpu_6050_usart_init(void);			//6050����ģʽ��ʼ����������ʱ

void turn_frontORback(long int ratio);	//�����İڶ�����
void turn_leftORright(long int ratio);

void KeepLeftAndRightToZero(void);		//����Ҫ��2 �������Ҿ�ֹ
void KeepSetedRange(void);				//����Ҫ��2 ����ǰ��һ���ķ���
void StartWingFrontOrBack(double wind);
void StartWingLeftOrRight(double wind);	//����Ҫ��3 �����ƶ�����


double fabs(double x);					//ȡ����ֵ����
int ROUND(double x);
double OutputAdjustment(double x);
void DataOutput(void);					//�������


float Angle_Data[]=
{
0,
69	,
137	,
206	,
274	,
343	,
411	,
479	,
547	,
615	,
683	,
750	,
818	,
885	,
952	,
1018	,
1085	,
1151	,
1217	,
1282	,
1347	,
1412	,
1476	,
1540	,
1604	,
1667	,
1730	,
1792	,
1854	,
1915	,
1976	,
2037    ,
2097    ,
2156    ,
2216    ,
2274    ,
2332,
2390,
2447,
2504,
2560,
};

int ROUND(double x)
{
	if( x > ((int)x+0.5) )
		return (x+1);
	else
		return (int)x;
}

#endif
