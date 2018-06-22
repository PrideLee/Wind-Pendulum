#include "Task3.h"
#include "math.h"

#include "Function.h"

#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "KEY.h"
#include "BEED.h" 
#include "Timer.h"
#include "lcd.h"
#include "PID.h"
#include "APP.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "semphr.h"

void outputinit(void);

/*
	任务3
*/
//任务优先级
#define Task3task_PRIO		10
//任务堆栈大小	
#define Task3task_STK_SIZE 		250
//任务句柄
TaskHandle_t Task3task_Handler;
//任务函数
void Task3task(void *pvParameters);

void CreatTask3(void)
{
	xTaskCreate((TaskFunction_t )Task3task,            	//任务函数
							(const char*    )"Task3task",           //任务名称
							(uint16_t       )Task3task_STK_SIZE,    //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )Task3task_PRIO,       	//任务优先级
							(TaskHandle_t*  )&Task3task_Handler);   //任务句柄  

}

/*****************************************************************************/
//                        任务3
/*****************************************************************************/
extern SemaphoreHandle_t TIMER_BinarySemaphore;	//二值信号量句柄

extern struct Exp_angle exp_Angle;
extern struct Current_Angle current_Angle;

uint8_t i_3=1,q_3=0;
extern uint8_t YON,XON;

#define L1 88.0f  //cm
#define rad (3.1415f/180.0f)
#define deg (180.0f/3.1415f)

extern float XIerror;
extern float YIerror;

void Task3task(void *pvParameters)
{
	float X_UK,Y_UK;
	struct MPU6050 mpu;
	struct Attitude AT;
	struct History_Angle history_Angle;
	BaseType_t err=pdFALSE;
	
	float x=0.0, y=0.0;
	static float t=0.0;
	uint8_t cnt_t = 0;
	
	while(1)
	{
		taskENTER_CRITICAL();           //进入临界区
		X_UK=0;
		Y_UK=0;
		Set_MotoSpeed(2000,1);//关动力
		Set_MotoSpeed(2000,2);
		Set_MotoSpeed(2000,3);
		Set_MotoSpeed(2000,4);
		LCD_Fill_onecolor(100,150,240,162,WHITE);
		LCD_DisplayString(100,150,12,"STOP");
		
		ClearTaskDATA();
		taskEXIT_CRITICAL();            //退出临界区
			TIM6Close();
			i_3=1;
			q_3=0;
		
		while(i_3)
		{
			delay_ms(20);
			
			if(Get_KEY(3))//  选择确认
			{
				taskENTER_CRITICAL();           //进入临界区
				LCD_Fill_onecolor(100,150,240,162,WHITE);
				LCD_DisplayString(100,150,12,"RUN");
				taskEXIT_CRITICAL();            //退出临界区
				delay_ms(50);
				
					i_3=0;//结束此循环
					q_3=1;//开始下一个循环（正式执行任务）
					TIM6Open();
			}
		}
		
		cnt_t=0;
		t=0;
		XIerror=0;
		YIerror=0;
		while(q_3)
		{
			SetPID(1);
			err=xSemaphoreTake(TIMER_BinarySemaphore,portMAX_DELAY);	//获取信号量
			if(err==pdTRUE)
			{
				Get_MPU6050(&mpu) ;//获取数据
				IMUupdate(mpu.Gx, mpu.Gy, mpu.Gz, mpu.Ax, mpu.Ay, mpu.Az, &AT);//数据梳理
				
				current_Angle.X=(AT.X+history_Angle.X_1+history_Angle.X_2)/3;//当前角
				current_Angle.Y=(AT.Y+history_Angle.Y_1+history_Angle.Y_2)/3;
				
				history_Angle.X_2=history_Angle.X_1;  
				history_Angle.X_1=AT.X;
				
				history_Angle.Y_2=history_Angle.Y_1;
				history_Angle.Y_1=AT.Y;
				
				//////////////////////////////////////////////////
				cnt_t += 1;							 //每10ms运算1次
				if(cnt_t==1)
				{
					cnt_t=0;
					t+=0.01f;
				}
				
				x=20*sin(3*t)* cos(t);
				y=20*sin(3*t)* sin(t);
				
				exp_Angle.X = atan(x/L1)*deg;
				exp_Angle.Y = atan(y/L1)*deg;
				//////////////////////////////////////////////////
				
				
				if((current_Angle.X>=50.0f||current_Angle.X<=-50.0f)||(current_Angle.Y>=50.0f||current_Angle.Y<=-50.0f))//限位    //(current_Angle.X>=-0.3f&&current_Angle.X<=0.3f)||
				{
					XIerror=0;
					YIerror=0;
					Set_MotoSpeed(2000,1);
					Set_MotoSpeed(2000,2);
					Set_MotoSpeed(2000,3);
					Set_MotoSpeed(2000,4);
				}
				else
				{
					if(XON)
					{
						X_UK=X_PID(current_Angle.X, exp_Angle.X);
					}
					if(YON)
					{
						Y_UK=Y_PID(current_Angle.Y, exp_Angle.Y);
					}
					
						moto(X_UK,Y_UK);
				}
				if(Debug)OUTPUT ();
				if(Get_KEY(3))
				{	
					TIM6Close();
					q_3=0;//结束此循环
					i_3=0;//开始上面一个循环
				}
			}
		}
		
	}
}

void outputinit(void)
{
	uint16_t i=0;
	struct MPU6050 mpu;
	struct Attitude AT;
	struct History_Angle history_Angle;
	for(i=50;i>0;i--)
	{
		delay_ms(20);
		Get_MPU6050(&mpu) ;//获取数据
		IMUupdate(mpu.Gx, mpu.Gy, mpu.Gz, mpu.Ax, mpu.Ay, mpu.Az, &AT);//数据梳理
		
		current_Angle.X=(AT.X+history_Angle.X_1+history_Angle.X_2)/3;//当前角
		current_Angle.Y=(AT.Y+history_Angle.Y_1+history_Angle.Y_2)/3;
		
		history_Angle.X_2=history_Angle.X_1;  
		history_Angle.X_1=AT.X;
		
		history_Angle.Y_2=history_Angle.Y_1;
		history_Angle.Y_1=AT.Y;
		if(Debug)OUTPUT ();
	}
}
