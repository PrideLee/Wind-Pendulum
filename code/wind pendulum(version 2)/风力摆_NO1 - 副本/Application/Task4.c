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
	����4
*/
//�������ȼ�
#define Task4task_PRIO		10
//�����ջ��С	
#define Task4task_STK_SIZE 		250
//������
TaskHandle_t Task4task_Handler;
//������
void Task4task(void *pvParameters);

void CreatTask4(void)
{
	xTaskCreate((TaskFunction_t )Task4task,            	//������
							(const char*    )"Task4task",           //��������
							(uint16_t       )Task4task_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )Task4task_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&Task4task_Handler);   //������  

}

/*****************************************************************************/
//                        ����4
/*****************************************************************************/
extern SemaphoreHandle_t TIMER_BinarySemaphore;	//��ֵ�ź������

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
		//����PID
		SetPID(4);
		
		err=xSemaphoreTake(TIMER_BinarySemaphore,portMAX_DELAY);	//��ȡ�ź���
		if(err==pdTRUE)
		{
				Get_MPU6050(&mpu) ;//��ȡ����
				IMUupdate(mpu.Gx, mpu.Gy, mpu.Gz, mpu.Ax, mpu.Ay, mpu.Az, &AT);//��������
				
				current_Angle.X=(AT.X+history_Angle.X_1+history_Angle.X_2)/3;//��ǰ��
				current_Angle.Y=(AT.Y+history_Angle.Y_1+history_Angle.Y_2)/3;
				
				history_Angle.X_2=history_Angle.X_1;  
				history_Angle.X_1=AT.X;
				
				history_Angle.Y_2=history_Angle.Y_1;
				history_Angle.Y_1=AT.Y;
				
				if((current_Angle.X>=50.0f||current_Angle.X<=-50.0f)||(current_Angle.Y>=50.0f||current_Angle.Y<=-50.0f))//��λ      //(current_Angle.X>=-0.3f&&current_Angle.X<=0.3f)||
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

