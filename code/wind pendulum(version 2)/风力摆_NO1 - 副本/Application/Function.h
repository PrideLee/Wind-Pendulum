#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

#include "led.h"
#include "BEED.h" 
#include "KEY.h"
#include "MPU6050.h"
#include "PID.h"

#include "Task1.h"
#include "Task2.h"
#include "Task3.h"
#include "Task4.h"
#include "Task5.h"
#include "Attitude.h"

extern uint8_t Debug;//���������ж���
extern struct Exp_angle exp_Angle;//��ǰC�ļ�
extern struct Current_Angle current_Angle;;

struct Current_Angle
{
	float X;
	float Y;
};
struct Exp_angle
{
	float X;
	float Y;
};

struct History_Angle
{
	float X_1;
	float X_2;
	
	float Y_1;
	float Y_2;
};


//************************
void OUTput(uint8_t a);
//************************

void beep(void);//������BEEPһ��   ֱ�ӵ��� beep();
void led1(void);//LED1��һ��                led1();

uint8_t Get_KEY(uint8_t NUM);  // ���ĳ�����Ƿ񱻰���  ����NUM��������� 1 or 2 or 3   ������·��� 1 �� ���򷵻�0  

uint8_t Get_MPU6050(struct MPU6050 *mpu);// ��ȡMPU����

void OUTPUT (void);
	
void Set_MotoSpeed(uint16_t speed, uint8_t num); // ���õ���ٶ�  speed: 2000~4000      num�����: 1~4



#endif
