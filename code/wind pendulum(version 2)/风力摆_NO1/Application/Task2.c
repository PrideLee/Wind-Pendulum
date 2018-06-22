#include "Task2.h"
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

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "semphr.h"

/*
	����2
*/
//�������ȼ�
#define Task2task_PRIO		10
//�����ջ��С	
#define Task2task_STK_SIZE 		250
//������
TaskHandle_t Task2task_Handler;
//������
void Task2task(void *pvParameters);

void CreatTask2(void)
{
	xTaskCreate((TaskFunction_t )Task2task,            	//������
							(const char*    )"Task2task",           //��������
							(uint16_t       )Task2task_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )Task2task_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&Task2task_Handler);   //������  

}
	
/*****************************************************************************/
//                        ����2
/*****************************************************************************/
extern float My_Sqrt(float x)  ;
extern SemaphoreHandle_t TIMER_BinarySemaphore;	//��ֵ�ź������

#define L1 88.0f  //cm
#define rad (3.1415f/180.0f)
#define deg (180.0f/3.1415f)

extern struct Exp_angle exp_Angle;
extern struct Current_Angle current_Angle;

uint8_t i_2=1,q_2=0;
extern uint8_t YON,XON;

extern float XIerror;
extern float YIerror;

void Task2task(void *pvParameters)
{
	float S=0;//����ʰڶ�������һ��  cm
	float phase=0;//��λ ��
	
	const float priod = 1655.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;
	float A = 0.0;
	float Normalization = 0.0;
	float Omega = 0.0;
	
	
	float X_UK,Y_UK;
	
	uint8_t j=1;
	uint8_t flag=0;//����Ķ�����ʾ
	struct MPU6050 mpu;
	struct Attitude AT;
	struct History_Angle history_Angle;
	BaseType_t err=pdFALSE;
	

	while(1)
	{
		taskENTER_CRITICAL();           //�����ٽ���
		X_UK=0;
		Y_UK=0;
		Set_MotoSpeed(2000,1);//�ض���
		Set_MotoSpeed(2000,2);
		Set_MotoSpeed(2000,3);
		Set_MotoSpeed(2000,4);
		LCD_Fill_onecolor(100,150,118,162,WHITE);
		LCD_Fill_onecolor(10,150,240,240,WHITE);
		LCD_DisplayString(10,150,12,"*");
		LCD_DisplayString(30,150,12," S =  ");  //S=0;
		LCD_DisplayNum(66,150,(uint32_t)(S*10),3,12,0);
		LCD_DisplayString(30,174,12,"phase=  ");  //phase=0;
		LCD_DisplayNum(66,174,(uint32_t)phase,3,12,0);
		LCD_Fill_onecolor(10,150,16,162,WHITE);
		LCD_Fill_onecolor(10,162,16,174,WHITE);
		LCD_DisplayString(10,150,12,"*");
		taskEXIT_CRITICAL();            //�˳��ٽ���
			TIM6Close();
			i_2=1;
			q_2=0;
			flag=0;
			
		while(i_2)
		{
			delay_ms(50);
			if(Get_KEY(1))//  +
			{
				if(j==1)//�� S
				{
					S+=2.5f;
					if(S>40)S=0;
					printf("S= %f\n",S);
					flag=1;
				}
				if(j==2)//�� phase
				{
					phase+=5;
					if(phase>360)phase=0;
					printf("phase= %f\n",phase);
					flag=1;
				}
			}
			
			if(Get_KEY(2))//  -
			{
				if(j==1)//�� S
				{
					S-=2.5f;
					if(S<0)S=40;
					printf("S= %f\n",S);
					flag=1;
				}
				if(j==2)//�� phase
				{
					phase-=5;
					if(phase<0)phase=360;
					printf("phase= %f\n",phase);
					flag=1;
				}
			}
			
			if(flag)//  ����Ķ�����ʾ
			{
				taskENTER_CRITICAL();           //�����ٽ���
				LCD_Fill_onecolor(66,150,120,162,WHITE);
				LCD_Fill_onecolor(90,174,120,174,WHITE);
				LCD_DisplayNum(66,150,(uint32_t)(S*10),3,12,0);
				LCD_DisplayNum(66,174,(uint32_t)phase,3,12,0);
				taskEXIT_CRITICAL();            //�˳��ٽ���
				flag=0;
			}
			
			if(Get_KEY(3))//  ѡ��ȷ��
			{
					j++;
				if(j==2)
				{
					taskENTER_CRITICAL();           //�����ٽ���
					LCD_Fill_onecolor(10,150,16,162,WHITE);
					LCD_DisplayString(10,176,12,"*");
					taskEXIT_CRITICAL();            //�˳��ٽ���
				}
				if(j==3)
				{
					taskENTER_CRITICAL();           //�����ٽ���
					LCD_Fill_onecolor(10,150,16,162,WHITE);
					LCD_Fill_onecolor(10,162,16,174,WHITE);
					LCD_Fill_onecolor(10,174,16,186,WHITE);
					LCD_DisplayString(100,150,12,"RUN");
					taskEXIT_CRITICAL();            //�˳��ٽ���
					A = atan((S/L1))*57.2958f;			//���ݰڷ�����Ƕ�A,L1Ϊ�ڸ˾�����泤��cm
					j=1;
					i_2=0;//������ѭ��
					q_2=1;//��ʼ��һ��ѭ������ʽִ������
					TIM6Open();
				}
			}
		}
		
		XIerror=0;
		YIerror=0;
		
		while(q_2)
		{
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
				
	MoveTimeCnt += 10;							 //ÿ10ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega = 2.0*3.14159*Normalization;			 //��2�н��й�һ������
				
				if((current_Angle.X>=50.0f||current_Angle.X<=-50.0f))//��λ    //(current_Angle.X>=-0.3f&&current_Angle.X<=0.3f)||
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
						exp_Angle.X = A*sin(Omega); //������
						X_UK=X_PID(current_Angle.X, exp_Angle.X);
					}
					if(YON)
					{
						exp_Angle.Y =  A*sin(Omega+phase*rad); //������
						Y_UK=Y_PID(current_Angle.Y, exp_Angle.Y);
					}
						moto(X_UK,Y_UK);
				}
				
				if(Debug)OUTPUT ();
				if(Get_KEY(3))
				{	
					TIM6Close();
					q_2=0;//������ѭ��
					i_2=0;//��ʼ����һ��ѭ��
				}
			}
		}
	}
}

