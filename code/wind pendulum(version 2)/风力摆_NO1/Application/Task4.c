#include "Task4.h"

#include "Function.h"

#include "usart.h"
#include "LED.h"
#include "KEY.h"
#include "BEED.h" 

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "semphr.h"

#include "PID.h"

/*
	任务4
*/
//任务优先级
#define Task4task_PRIO		10
//任务堆栈大小	
#define Task4task_STK_SIZE 		250
//任务句柄
TaskHandle_t Task4task_Handler;
//任务函数
void Task4task(void *pvParameters);

void CreatTask4(void)
{
	xTaskCreate((TaskFunction_t )Task4task,            	//任务函数
							(const char*    )"Task4task",           //任务名称
							(uint16_t       )Task4task_STK_SIZE,    //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )Task4task_PRIO,       	//任务优先级
							(TaskHandle_t*  )&Task4task_Handler);   //任务句柄  

}

/*****************************************************************************/
//                        任务4
/*****************************************************************************/
extern SemaphoreHandle_t TIMER_BinarySemaphore;	//二值信号量句柄

extern float XIerror;
extern float YIerror;
extern uint8_t YON,XON;

extern struct Exp_angle exp_Angle;
extern struct Current_Angle current_Angle;


void Task4task(void *pvParameters)
{
	float X_UK,Y_UK;
	struct MPU6050 mpu;
	struct Attitude AT;
	struct History_Angle history_Angle;
	BaseType_t err=pdFALSE;
	while(1)
	{
		//设置PID
		SetPID(4);
		
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
				
				if((current_Angle.X>=50.0f||current_Angle.X<=-50.0f)||(current_Angle.Y>=50.0f||current_Angle.Y<=-50.0f))//限位      //(current_Angle.X>=-0.3f&&current_Angle.X<=0.3f)||
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
						exp_Angle.X=0;
						
						X_UK=X_PID(current_Angle.X, exp_Angle.X);
					}
					if(YON)
					{
						exp_Angle.Y=0;
						
						Y_UK=Y_PID(current_Angle.Y, exp_Angle.Y);
					}
						moto(X_UK,Y_UK);
				}
		}
	}
}

