#include "Task1.h"
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
	����1
*/
//�������ȼ�
#define Task1task_PRIO		10
//�����ջ��С	
#define Task1task_STK_SIZE 		250
//������
TaskHandle_t Task1task_Handler;
//������
void Task1task(void *pvParameters);

void CreatTask1(void)
{
	xTaskCreate((TaskFunction_t )Task1task,            	//������
							(const char*    )"Task1task",           //��������
							(uint16_t       )Task1task_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )Task1task_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&Task1task_Handler);   //������  

}

/*****************************************************************************/
//                        ����1
/*****************************************************************************/

/************************************************/
//                 ����
/************************************************/
extern float My_Sqrt(float x)  ;
extern SemaphoreHandle_t TIMER_BinarySemaphore;	//��ֵ�ź������

extern float XIerror;
extern float YIerror;

#define L1 88.0f  //cm
#define rad (3.1415f/180.0f)
#define deg (180.0f/3.1415f)

extern struct Exp_angle exp_Angle;
extern struct Current_Angle current_Angle;

uint8_t i_1=1,q_1=0;
uint8_t YON=1,XON=1;

void Task1task(void *pvParameters)
{
	float S=0;//����ʰڶ��뾶  cm
	float S_x,S_y;
	int16_t offset_degree=0;// x y �����ϵİڶ�ƫ��  ��
	
	const float priod = 1655.0;  //��������(����)   1655.0
	static uint32_t MoveTimeCnt = 0;
	float Ax = 0.0 , Ay = 0.0;
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
		LCD_DisplayString(30,162,12,"Deg=  ");  //offset_degree=0;
		LCD_DisplayNum(66,162,(uint32_t)offset_degree,3,12,0);
		LCD_Fill_onecolor(10,150,16,162,WHITE);
		LCD_Fill_onecolor(10,162,16,174,WHITE);
		LCD_DisplayString(10,150,12,"*");
		taskEXIT_CRITICAL();            //�˳��ٽ���
		
			TIM6Close();
			i_1=1;
			q_1=0;
			flag=0;
		while(i_1)
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
				if(j==2)//�� offset_degree
				{
					offset_degree+=5;
					if(offset_degree>180)offset_degree=0;
					printf("offset_degree= %d\n",offset_degree);
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
				if(j==2)//�� offset_degree
				{
					offset_degree-=5;
					if(offset_degree<0)offset_degree=180;
					printf("offset_degree= %d\n",offset_degree);
					flag=1;
				}
			}
			
			if(flag)//  ����Ķ�����ʾ
			{
				taskENTER_CRITICAL();           //�����ٽ���
				LCD_Fill_onecolor(66,150,120,162,WHITE);
				LCD_Fill_onecolor(66,162,120,174,WHITE);
				LCD_DisplayNum(66,150,(uint32_t)(S*10),3,12,0);
				LCD_DisplayNum(66,162,(uint32_t)offset_degree,3,12,0);
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
					LCD_DisplayString(10,162,12,"*");
					taskEXIT_CRITICAL();            //�˳��ٽ���
				}
				if(j==3)
				{
					taskENTER_CRITICAL();           //�����ٽ���
					LCD_Fill_onecolor(10,150,16,162,WHITE);
					LCD_Fill_onecolor(10,162,16,174,WHITE);
					LCD_DisplayString(100,150,12,"RUN");
					taskEXIT_CRITICAL();            //�˳��ٽ���
					
					S_x=S*cos(offset_degree*rad);// cm
					S_y=S*sin(offset_degree*rad);// cm
//					printf("S_x= %f\n",S_x);
//					printf("S_y= %f\n",S_y);
					Ax = atan((S_x/L1))*57.2958f;				 //���ݰڷ�����Ƕ�Ax,L1Ϊ�ڸ˾�����泤��cm
					Ay = atan((S_y/L1))*57.2958f;				 //���ݰڷ�����Ƕ�Ax,L1Ϊ�ڸ˾�����泤��cm
//					printf("A_x= %f\n",A_x);
//					printf("A_y= %f\n",A_y);
					j=1;
					i_1=0;//������ѭ��
					q_1=1;//��ʼ��һ��ѭ������ʽִ������
					TIM6Open();
				}
			}
		}
		
		MoveTimeCnt=0;
		XIerror=0;
		YIerror=0;
		while(q_1)
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
								
	MoveTimeCnt += 10;							 //ÿ10ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega = 2.0*3.14159*Normalization;			 //��2�н��й�һ������
				
				if((current_Angle.X>=50.0f||current_Angle.X<=-50.0f)||(current_Angle.Y>=50.0f||current_Angle.Y<=-50.0f))//��λ   
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
						exp_Angle.X=Ax*sin(Omega); //������
						X_UK=X_PID(current_Angle.X, exp_Angle.X);
					}
					if(YON)
					{
						exp_Angle.Y=Ay*sin(Omega); //������
						Y_UK=Y_PID(current_Angle.Y, exp_Angle.Y);
					}
					
						moto(X_UK,Y_UK);
					
				}
				
				if(Debug)OUTPUT ();
				if(Get_KEY(3))
				{	
					TIM6Close();
					q_1=0;//������ѭ��
					i_1=0;//��ʼ����һ��ѭ��
				}
			}
		}
	}
}

