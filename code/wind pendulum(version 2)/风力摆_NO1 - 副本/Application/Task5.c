#include "Task5.h"
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

/*
	����5
*/
//�������ȼ�
#define Task5task_PRIO		10
//�����ջ��С	
#define Task5task_STK_SIZE 		250
//������
TaskHandle_t Task5task_Handler;
//������
void Task5task(void *pvParameters);

void CreatTask5(void)
{
	xTaskCreate((TaskFunction_t )Task5task,            	//������
							(const char*    )"Task5task",           //��������
							(uint16_t       )Task5task_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )Task5task_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&Task5task_Handler);   //������  

}

/*****************************************************************************/
//                        ����3
/*****************************************************************************/
extern SemaphoreHandle_t TIMER_BinarySemaphore;	//��ֵ�ź������

extern struct Exp_angle exp_Angle;
extern struct Current_Angle current_Angle;

uint8_t i_5=1,q_5=0;
extern uint8_t YON,XON;

int8_t ex=0,ey=0;

#define L1 88.0f  //cm
#define rad (3.1415f/180.0f)
#define deg (180.0f/3.1415f)

extern float XIerror;
extern float YIerror;

void Task5task(void *pvParameters)
{
	float X_UK,Y_UK;
	struct MPU6050 mpu;
	struct Attitude AT;
	struct History_Angle history_Angle;
	BaseType_t err=pdFALSE;
	
	ex=10;ey=0;
	
	while(1)
	{
		taskENTER_CRITICAL();           //�����ٽ���
		X_UK=0;
		Y_UK=0;
		Set_MotoSpeed(2000,1);//�ض���
		Set_MotoSpeed(2000,2);
		Set_MotoSpeed(2000,3);
		Set_MotoSpeed(2000,4);
		LCD_Fill_onecolor(100,150,240,162,WHITE);
		LCD_DisplayString(100,150,12,"STOP");
		
		ClearTaskDATA();
		taskEXIT_CRITICAL();            //�˳��ٽ���
			TIM6Close();
			i_5=1;
			q_5=0;
		
		while(i_5)
		{
			delay_ms(20);
			
			if(Get_KEY(3))//  ѡ��ȷ��
			{
				taskENTER_CRITICAL();           //�����ٽ���
				LCD_Fill_onecolor(100,150,240,162,WHITE);
				LCD_DisplayString(100,150,12,"RUN");
				taskEXIT_CRITICAL();            //�˳��ٽ���
				delay_ms(50);
				
					i_5=0;//������ѭ��
					q_5=1;//��ʼ��һ��ѭ������ʽִ������
					TIM6Open();
			}
		}
		
		XIerror=0;
		YIerror=0;
		while(q_5)
		{
			SetPID(1);
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
				
				if((current_Angle.X>=50.0f||current_Angle.X<=-50.0f)||(current_Angle.Y>=50.0f||current_Angle.Y<=-50.0f))//��λ    //(current_Angle.X>=-0.3f&&current_Angle.X<=0.3f)||
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
						
						exp_Angle.X=atan(ex/L1)*deg;
						
						X_UK=X_PID(current_Angle.X, exp_Angle.X);
					}
					if(YON)
					{
						exp_Angle.Y=atan(ey/L1)*deg;
						
						Y_UK=Y_PID(current_Angle.Y, exp_Angle.Y);
					}
						moto(X_UK,Y_UK);
				}
				if(Debug)OUTPUT ();
				if(Get_KEY(3))
				{	
					TIM6Close();
					q_5=0;//������ѭ��
					i_5=0;//��ʼ����һ��ѭ��
				}
			}
		}
		
	}
}
