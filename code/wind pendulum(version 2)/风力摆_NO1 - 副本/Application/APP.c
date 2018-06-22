#include "APP.h"
#include "usart.h"
#include "usart2.h"	
#include "delay.h"

#include "LED.h"
#include "KEY.h"
#include "BEED.h" 
#include "lcd.h"
#include "Timer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "semphr.h"

#include "Function.h"

extern uint8_t tasknum;
uint8_t START=0;

extern TaskHandle_t Task1task_Handler;
extern TaskHandle_t Task2task_Handler;
extern TaskHandle_t Task3task_Handler;
extern TaskHandle_t Task4task_Handler;
extern TaskHandle_t Task5task_Handler;

/*
*****************************************************************************************************
																	����궨��
*****************************************************************************************************
*/

SemaphoreHandle_t USART_IN_BinarySemaphore;
SemaphoreHandle_t USART_OUT_BinarySemaphore;

/*
	LED0��˸����ʾϵͳ��������
*/
//�������ȼ�
#define LEDtask_PRIO		10
//�����ջ��С	
#define LEDtask_STK_SIZE 		50
//������
TaskHandle_t LEDtask_Handler;
//������
void LEDtask(void *pvParameters);

//�������ź������
SemaphoreHandle_t KEY1_CountSemaphore;//�������ź���
SemaphoreHandle_t KEY2_CountSemaphore;
SemaphoreHandle_t KEY3_CountSemaphore;
SemaphoreHandle_t KEY4_CountSemaphore;

SemaphoreHandle_t BEEP_CountSemaphore;

SemaphoreHandle_t LED_CountSemaphore;

/*
	����
*/
//�������ȼ�
#define DataINtask_PRIO		9
//�����ջ��С	
#define DataINtask_STK_SIZE 		250
//������
TaskHandle_t DataINtask_Handler;
//������
void DataINtask(void *pvParameters);

/*
	���
*/
//�������ȼ�
#define DataOUTtask_PRIO		9
//�����ջ��С	
#define DataOUTtask_STK_SIZE 		250
//������
TaskHandle_t DataOUTtask_Handler;
//������
void DataOUTtask(void *pvParameters);

/*
	KEY�������
*/
//�������ȼ�
#define KEYtask_PRIO		10
//�����ջ��С	
#define KEYtask_STK_SIZE 		250
//������
TaskHandle_t KEYtask_Handler;
//������
void KEYtask(void *pvParameters);

/*
	ENDtask
*/
//�������ȼ�
#define ENDtask_PRIO		11
//�����ջ��С	
#define ENDtask_STK_SIZE 		250
//������
TaskHandle_t ENDtask_Handler;
//������
void ENDtask(void *pvParameters);

/*
	BEEP
*/
//�������ȼ�
#define BEEPtask_PRIO		10
//�����ջ��С	
#define BEEPtask_STK_SIZE 		50
//������
TaskHandle_t BEEPtask_Handler;
//������
void BEEPtask(void *pvParameters);

/*
	LED1
*/
//�������ȼ�
#define LED1task_PRIO		10
//�����ջ��С	
#define LED1task_STK_SIZE 		50
//������
TaskHandle_t LED1task_Handler;
//������
void LED1task(void *pvParameters);

/*
	��ʼ����
*/
//�������ȼ�
#define Starttask_PRIO		11
//�����ջ��С	
#define Starttask_STK_SIZE 		250
//������
TaskHandle_t Starttask_Handler;
//������
void Starttask(void *pvParameters);


/*
*****************************************************************************************************
																  ����
*****************************************************************************************************
*/

uint8_t AllAppInit(void)
{
	uint8_t i=1;
	
	//�����������ź���
	KEY1_CountSemaphore=xSemaphoreCreateCounting(200,0);	
	KEY2_CountSemaphore=xSemaphoreCreateCounting(200,0);	
	KEY3_CountSemaphore=xSemaphoreCreateCounting(200,0);	
	KEY4_CountSemaphore=xSemaphoreCreateCounting(200,0);	
	
	BEEP_CountSemaphore=xSemaphoreCreateCounting(200,0);	
	LED_CountSemaphore=xSemaphoreCreateCounting(200,0);	
	
	USART_IN_BinarySemaphore=xSemaphoreCreateBinary();	
	USART_OUT_BinarySemaphore=xSemaphoreCreateBinary();
	
	if(KEY1_CountSemaphore==NULL||KEY2_CountSemaphore==NULL||KEY3_CountSemaphore==NULL||KEY4_CountSemaphore==NULL||BEEP_CountSemaphore==NULL||LED_CountSemaphore==NULL) 
	{
		printf("�����ź�������ʧ��\n");
		i=0;
	}
	
	xTaskCreate((TaskFunction_t )DataINtask,            	//������
							(const char*    )"DataINtask",           //��������
							(uint16_t       )DataINtask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )DataINtask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&DataINtask_Handler);   //������  
							
	xTaskCreate((TaskFunction_t )DataOUTtask,            	//������
							(const char*    )"DataOUTtask",           //��������
							(uint16_t       )DataOUTtask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )DataOUTtask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&DataOUTtask_Handler);   //������  
	
	xTaskCreate((TaskFunction_t )KEYtask,            	//������
							(const char*    )"KEYtask",           //��������
							(uint16_t       )KEYtask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )KEYtask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&KEYtask_Handler);   //������     
							
	xTaskCreate((TaskFunction_t )BEEPtask,            	//������
							(const char*    )"BEEPtask",           //��������
							(uint16_t       )BEEPtask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )BEEPtask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&BEEPtask_Handler);   //������     						

	xTaskCreate((TaskFunction_t )ENDtask,            	//������
							(const char*    )"ENDtask",           //��������
							(uint16_t       )ENDtask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )ENDtask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&ENDtask_Handler);   //������     										

  xTaskCreate((TaskFunction_t )LEDtask,            	//������
							(const char*    )"LEDtask",           //��������
							(uint16_t       )LEDtask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )LEDtask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&LEDtask_Handler);   //������   	
	xTaskCreate((TaskFunction_t )LED1task,            	//������
							(const char*    )"LED1task",           //��������
							(uint16_t       )LED1task_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )LED1task_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&LED1task_Handler);   //������   

	xTaskCreate((TaskFunction_t )Starttask,            	//������
							(const char*    )"Starttask",           //��������
							(uint16_t       )Starttask_STK_SIZE,    //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )Starttask_PRIO,       	//�������ȼ�
							(TaskHandle_t*  )&Starttask_Handler);   //������     
	
	CreatTask1();
	CreatTask2();
	CreatTask3();
	CreatTask4();
	CreatTask5();
	
							
	return i;
}

/*
	������LED��ʾϵͳ��������
*/
void LEDtask(void *pvParameters)
{
	while(1)
	{
		LED0=~LED0;
		vTaskDelay(1000);
	}
}

/*
	��������ⰴ��
*/
void KEYtask(void *pvParameters)
{
	BaseType_t err;
	while(1)
	{
		if(KEY_Scan(1))           	//ɨ�谴��
		{err=xSemaphoreGive(KEY1_CountSemaphore); beep();} //�ͷż������ź���
		
		if(KEY_Scan(2))           	//ɨ�谴��
		{err=xSemaphoreGive(KEY2_CountSemaphore); beep();} //�ͷż������ź���
		
		if(KEY_Scan(3))           	//ɨ�谴��
		{err=xSemaphoreGive(KEY3_CountSemaphore); beep();} //�ͷż������ź���
		
		if(KEY_Scan(4))           	//ɨ�谴��
		{err=xSemaphoreGive(KEY4_CountSemaphore); beep();} //�ͷż������ź���
				
	  
		
		vTaskDelay(20);     //��ʱ20ms��Ҳ����20��ʱ�ӽ���	
	}
}
/*
	�����BBEEPһ��
*/
void BEEPtask(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(BEEP_CountSemaphore,portMAX_DELAY);
		BEED_1(50);
	}
}

/*
	�����BLED1��һ��
*/
void LED1task(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(LED_CountSemaphore,portMAX_DELAY);
		LED1=0;
		delay_ms(50);
		LED1=1;
	}
}

void ENDtask(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(KEY4_CountSemaphore,portMAX_DELAY);
		taskENTER_CRITICAL();           //�����ٽ���
		
		led1();
		if(tasknum)
		switch(tasknum)
		{
			case 1: vTaskSuspend(Task1task_Handler); break;
			case 2: vTaskSuspend(Task2task_Handler); break;
			case 3: vTaskSuspend(Task3task_Handler); break;
			case 4: vTaskSuspend(Task4task_Handler); break;
			case 5: vTaskSuspend(Task5task_Handler); break;
		}
		tasknum=0;
		if(!START)vTaskResume(Starttask_Handler);	//�ָ�����
		START=1;
		
		LCD_Fill_onecolor(90,50,200,62,WHITE);
		LCD_DisplayString(90,50,12,"Start ");
		
		taskEXIT_CRITICAL();            //�˳��ٽ���
		delay_ms(100);
	}
}


extern uint8_t YON,XON;
extern int8_t ex,ey;
void DataINtask(void *pvParameters)
{
	uint8_t eH,eL;
	BaseType_t err=pdFALSE;
	float P=0,I=0,D=0;
	while(1)
	{
		err=xSemaphoreTake(USART_IN_BinarySemaphore,portMAX_DELAY);	//��ȡ�ź���
		if(err==pdTRUE)
		{
			if((USART_RX_STA&0x8000)!=0)//���յ�������
			{
				USART_RX_STA=0;	
				//  X�����PID
				if(USART_RX_BUF[0]=='X'&&USART_RX_BUF[1]=='P'&&USART_RX_BUF[5]=='I'&&USART_RX_BUF[10]=='D')//��PID
				{
					//����
					if(USART_RX_BUF[2]-48==0&&USART_RX_BUF[3]-48==0&&USART_RX_BUF[4]-48==0&&USART_RX_BUF[6]-48==0&&USART_RX_BUF[7]-48==0&&USART_RX_BUF[8]-48==0&&USART_RX_BUF[9]-48==0&&USART_RX_BUF[11]-48==0&&USART_RX_BUF[12]-48==0&&USART_RX_BUF[13]-48==0&&USART_RX_BUF[14]-48==0)
						printf("nowXP=%.1f  I=%.4f  D=%.5f\n",X_P,X_I,X_D);
					else
					{
						P=(USART_RX_BUF[2]-48)*100 +(USART_RX_BUF[3]-48)*10 +(USART_RX_BUF[4]-48);
						I=(float)(USART_RX_BUF[6]-48) +(float)(USART_RX_BUF[7]-48)/10 +(float)(USART_RX_BUF[8]-48)/100 +(float)(USART_RX_BUF[9]-48)/1000;
						D=(float)(USART_RX_BUF[11]-48)*10+(float)(USART_RX_BUF[12]-48)+(float)(USART_RX_BUF[13]-48)/10+(float)(USART_RX_BUF[14]-48)/100+(float)(USART_RX_BUF[15]-48)/1000;
						X_P=P;
						X_I=I;
						X_D=D;
						printf("XP=%.1f  I=%.4f  D=%.5f\n",X_P,X_I,X_D);
					}
					//���
				}
				//  Y�����PID
				if(USART_RX_BUF[0]=='Y'&&USART_RX_BUF[1]=='P'&&USART_RX_BUF[5]=='I'&&USART_RX_BUF[10]=='D')//��PID
				{
					//����
					if(USART_RX_BUF[2]-48==0&&USART_RX_BUF[3]-48==0&&USART_RX_BUF[4]-48==0&&USART_RX_BUF[6]-48==0&&USART_RX_BUF[7]-48==0&&USART_RX_BUF[8]-48==0&&USART_RX_BUF[9]-48==0&&USART_RX_BUF[11]-48==0&&USART_RX_BUF[12]-48==0&&USART_RX_BUF[13]-48==0&&USART_RX_BUF[14]-48==0)
						printf("nowYP=%.1f  I=%.4f  D=%.5f\n",Y_P,Y_I,Y_D);
					else
					{
						P=(USART_RX_BUF[2]-48)*100 +(USART_RX_BUF[3]-48)*10 +(USART_RX_BUF[4]-48);
						I=(float)(USART_RX_BUF[6]-48) +(float)(USART_RX_BUF[7]-48)/10 +(float)(USART_RX_BUF[8]-48)/100 +(float)(USART_RX_BUF[9]-48)/1000;
						D=(float)(USART_RX_BUF[11]-48)*10+(float)(USART_RX_BUF[12]-48)+(float)(USART_RX_BUF[13]-48)/10+(float)(USART_RX_BUF[14]-48)/100+(float)(USART_RX_BUF[15]-48)/1000;
						Y_P=P;
						Y_I=I;
						Y_D=D;
						printf("YP=%.1f  I=%.4f  D=%.5f\n",Y_P,Y_I,Y_D);
					}
					//���
				}
				
				if(USART_RX_BUF[0]=='A'&&USART_RX_BUF[1]=='P'&&USART_RX_BUF[5]=='I')//���˲�
				{
					if((USART_RX_BUF[2]-48==0)&&(USART_RX_BUF[3]-48==0)&&(USART_RX_BUF[4]-48==0)&&(USART_RX_BUF[6]-48==0)&&(USART_RX_BUF[7]-48==0)&&(USART_RX_BUF[8]-48==0)&&(USART_RX_BUF[9]-48==0))
						printf("Kp=%f  Ki=%f\n",Kp,Ki);
					else
					{
						//����
						P=(float)(USART_RX_BUF[2]-48)*10+(float)(USART_RX_BUF[3]-48)+(float)(USART_RX_BUF[4]-48)/10;
						I=(float)(USART_RX_BUF[6]-48)+(float)(USART_RX_BUF[7]-48)/10+(float)(USART_RX_BUF[8]-48)/100+(float)(USART_RX_BUF[9]-48)/1000;
						Kp=P;
						Ki=I;
						//���
						printf("Kp=%f  Ki=%f\n",Kp,Ki);
					}
				}
				
				if(USART_RX_BUF[0]=='I'&&USART_RX_BUF[1]=='E')//�Ƿ����
				{
					if((USART_RX_BUF[2]-48==2))
						printf("I=%f\n",IE);
					else
					{
						//����
						IE=(float)(USART_RX_BUF[2]-48);
						//���
						printf("I=%f\n",IE);
					}
				}
				
				if(USART_RX_BUF[0]=='Y'&&USART_RX_BUF[1]=='O'&&USART_RX_BUF[2]=='N')//��Y��
				{
					if((USART_RX_BUF[3]-48==2))
						printf("YON=%d\n",YON);
					else
					{
						//����
						YON=(USART_RX_BUF[3]-48);
						//���
						printf("YON=%d\n",YON);
					}
				}
				if(USART_RX_BUF[0]=='X'&&USART_RX_BUF[1]=='O'&&USART_RX_BUF[2]=='N')//��X��
				{
					if((USART_RX_BUF[3]-48==2))
						printf("XON=%d\n",XON);
					else
					{
						//����
						XON=(USART_RX_BUF[3]-48);
						//���
						printf("XON=%d\n",XON);
					}
				}		
				if(USART_RX_BUF[0]=='e'&&USART_RX_BUF[1]=='x')// x ���ƫ��
				{
					if((USART_RX_BUF[2]-48==9)&&(USART_RX_BUF[3]-48==9))
						printf("ex=%d\n",ex);
					else
					{
						//����
						ex=(USART_RX_BUF[2]-48)*10+(USART_RX_BUF[3]-48);
						//���
						printf("ex=%d\n",ex);
					}
				}
				
				if(USART_RX_BUF[0]=='e'&&USART_RX_BUF[1]=='y')// y ���ƫ��
				{
					if((USART_RX_BUF[2]-48==9)&&(USART_RX_BUF[3]-48==9))
						printf("ey=%d\n",ey);
					else
					{
						//����
						ey=(USART_RX_BUF[2]-48)*10+(USART_RX_BUF[3]-48);
						//���
						printf("ey=%d\n",ey);
					}
				}
			}
				
//////////  ����2  /////////////	
			if((USART_RX_STA2&0x8000)!=0)//���յ�������
			{
				USART_RX_STA2=0;
				
				if(USART_RX_BUF2[0]=='x'&&USART_RX_BUF2[4]=='y')// x\y ���ƫ��
				{	 
					led1();
					if((USART_RX_BUF2[2]-48==9)&&(USART_RX_BUF2[3]-48==9))
					{
						eH=ex/10+48;
						eL=ex%10+48;
						printf_2_(&eH,1);
						printf_2_(&eL,1);
						printf_2_("  ",1);
						
							eH=ex/10+48;
							eL=ex%10+48;
							printf_2_(&eH,1);
							printf_2_(&eL,1);
							printf_2_("\n",1);
					}
					else
					{
						//����
						ex=(USART_RX_BUF2[2]-48)*10+(USART_RX_BUF2[3]-48);
						ey=(USART_RX_BUF2[6]-48)*10+(USART_RX_BUF2[7]-48);
						if(USART_RX_BUF2[1]-48==0)ex=-ex;
						if(USART_RX_BUF2[5]-48==0)ey=-ey;
						//���
						printf_2_(&USART_RX_BUF2[2],1);
						printf_2_(&USART_RX_BUF2[3],1);
						printf_2_("  ",1);
						printf_2_(&USART_RX_BUF2[6],1);
						printf_2_(&USART_RX_BUF2[7],1);
						printf_2_("\n",1);
					}
				}
				
				if(USART_RX_BUF2[0]=='X'&&USART_RX_BUF2[1]=='P'&&USART_RX_BUF2[5]=='I'&&USART_RX_BUF2[10]=='D')//��PID
				{
					//����
						P=(USART_RX_BUF2[2]-48)*100 +(USART_RX_BUF2[3]-48)*10 +(USART_RX_BUF2[4]-48);
						I=(float)(USART_RX_BUF2[6]-48) +(float)(USART_RX_BUF2[7]-48)/10 +(float)(USART_RX_BUF2[8]-48)/100 +(float)(USART_RX_BUF2[9]-48)/1000;
						D=(float)(USART_RX_BUF2[11]-48)*10+(float)(USART_RX_BUF2[12]-48)+(float)(USART_RX_BUF2[13]-48)/10+(float)(USART_RX_BUF2[14]-48)/100+(float)(USART_RX_BUF2[15]-48)/1000;
						X_P=P;
						X_I=I;
						X_D=D;
						beep();
					//���
				}
				if(USART_RX_BUF2[0]=='Y'&&USART_RX_BUF2[1]=='P'&&USART_RX_BUF2[5]=='I'&&USART_RX_BUF2[10]=='D')//��PID
				{
					//����
						P=(USART_RX_BUF2[2]-48)*100 +(USART_RX_BUF2[3]-48)*10 +(USART_RX_BUF2[4]-48);
						I=(float)(USART_RX_BUF2[6]-48) +(float)(USART_RX_BUF2[7]-48)/10 +(float)(USART_RX_BUF2[8]-48)/100 +(float)(USART_RX_BUF2[9]-48)/1000;
						D=(float)(USART_RX_BUF2[11]-48)*10+(float)(USART_RX_BUF2[12]-48)+(float)(USART_RX_BUF2[13]-48)/10+(float)(USART_RX_BUF2[14]-48)/100+(float)(USART_RX_BUF2[15]-48)/1000;
						Y_P=P;
						Y_I=I;
						Y_D=D;
						beep();
					//���
				}
				
			}
		}
	}
}

void DataOUTtask(void *pvParameters)
{
	BaseType_t err=pdFALSE;
	while(1)
	{
		err=xSemaphoreTake(USART_OUT_BinarySemaphore,portMAX_DELAY);	//��ȡ�ź���
		if(err==pdTRUE)
		{
			OUTput(1);
		}
	}
}


/****************************************************************************************************************************/
//                                            ��ʼ����
/****************************************************************************************************************************/


/*
	��������ʼ����
*/
void Starttask(void *pvParameters)
{
	uint8_t i=1,changshow=0;
	uint8_t task_num=1;
	while(1)
	{
		taskENTER_CRITICAL();           //�����ٽ���
		Set_MotoSpeed(2000,1);//�ض���
		Set_MotoSpeed(2000,2);
		Set_MotoSpeed(2000,3);
		Set_MotoSpeed(2000,4);
		
		vTaskSuspend(Task1task_Handler);//��������
		vTaskSuspend(Task2task_Handler);
		vTaskSuspend(Task3task_Handler);
		vTaskSuspend(Task4task_Handler);
		vTaskSuspend(Task5task_Handler);
		
		TIM6Close();//�ض�ʱ��
		
		//������
		tasknum=0;
		START=1;
		ClearTaskDATA();
		
		LCD_Fill_onecolor(10,150,240,300,WHITE);
		LCD_Fill_onecolor(90,50,200,62,WHITE);
		LCD_DisplayString(90,50,12,"Start ");
		
		LCD_Fill_onecolor(80,100,120,240,WHITE);
		LCD_DisplayString(30,100,12,"Select Task ");
		LCD_DisplayNum(120,100,task_num,1,12,0);
		taskEXIT_CRITICAL();            //�˳��ٽ���
		
		delay_ms(100);
		
		i=1;
		
		while(i)
		{
//			  LED1=0;
			delay_ms(50);
			if(Get_KEY(1))
			{
				led1();
				task_num++;
				if(task_num==6)task_num=1;
				changshow=1;
			}
			if(changshow)
			{
				changshow=0;
				taskENTER_CRITICAL();           //�����ٽ���
				LCD_Fill_onecolor(120,100,130,120,WHITE);
				LCD_DisplayNum(120,100,task_num,1,12,0);
				taskEXIT_CRITICAL();            //�˳��ٽ���
			}
			if(Get_KEY(3))
			{
				led1();
				taskENTER_CRITICAL();           //�����ٽ���
				tasknum=task_num;
				
				LCD_Fill_onecolor(90,50,200,62,WHITE);
				switch(task_num)
				{
					case 1:LCD_DisplayString(90,50,12,"Task 1");break;
					case 2:LCD_DisplayString(90,50,12,"Task 2");break;
					case 3:LCD_DisplayString(90,50,12,"Task 3");break;
					case 4:LCD_DisplayString(90,50,12,"Task 4");break;
					case 5:LCD_DisplayString(90,50,12,"Task 5");break;
				}
				
				//�ڴ˿�������
				switch(task_num)
				{
					case 1: vTaskResume(Task1task_Handler); break;
					case 2: vTaskResume(Task2task_Handler); break;
					case 3: vTaskResume(Task3task_Handler); break;
					case 4: vTaskResume(Task4task_Handler); break;
					case 5: vTaskResume(Task5task_Handler); break;
				}
				
				TIM6Open();
				START=0;
				//������
				vTaskSuspend(Starttask_Handler);
				i=0;
				taskEXIT_CRITICAL();            //�˳��ٽ���
			}
		}
	}
}


extern uint8_t i_1,q_1;
extern uint8_t i_2,q_2;
extern uint8_t i_3,q_3;
extern uint8_t i_5,q_5;
extern float XIerror;
extern float YIerror;
void ClearTaskDATA(void)
{
	q_1=0;
	i_1=0;
	
	q_2=0;
	i_2=0;
	
	q_3=0;
	i_3=0;
	
	q_5=0;
	i_5=0;
	
	XIerror=0;
	YIerror=0;
}


