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

extern uint8_t Debug;//在主函数中定义
extern struct Exp_angle exp_Angle;//当前C文件
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

void beep(void);//蜂鸣器BEEP一下   直接调用 beep();
void led1(void);//LED1闪一下                led1();

uint8_t Get_KEY(uint8_t NUM);  // 检测某按键是否被按下  输入NUM：按键编号 1 or 2 or 3   如果按下返回 1 ， 否则返回0  

uint8_t Get_MPU6050(struct MPU6050 *mpu);// 读取MPU数据

void OUTPUT (void);
	
void Set_MotoSpeed(uint16_t speed, uint8_t num); // 设置电机速度  speed: 2000~4000      num电机号: 1~4



#endif
