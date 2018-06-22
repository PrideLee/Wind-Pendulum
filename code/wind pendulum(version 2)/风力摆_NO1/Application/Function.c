/*
	��Ӧ�������ܼ�ͷ�ļ�
*/

#include "Function.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "PWMOut8.h"
#include "lcd.h"

//*************************************************************************
	struct Exp_angle exp_Angle;
	struct Current_Angle current_Angle;
//*************************************************************************



/*
	������������Ƿ񱻰���
	���룺NUM  ����Ǹ�����
	��������·���1  ���򷵻�0
	˵����
*/

//�������ź������   ��APP.c�ж���
extern SemaphoreHandle_t KEY1_CountSemaphore;//�������ź���
extern SemaphoreHandle_t KEY2_CountSemaphore;
extern SemaphoreHandle_t KEY3_CountSemaphore;
extern SemaphoreHandle_t KEY4_CountSemaphore;

extern SemaphoreHandle_t BEEP_CountSemaphore;
extern SemaphoreHandle_t LED_CountSemaphore;

uint8_t Get_KEY(uint8_t NUM)
{
	uint8_t i=0;
	
	switch(NUM)
	{
		case 1:	if(xSemaphoreTake(KEY1_CountSemaphore,2)) i=1 ;
						break;
		
	  case 2: if(xSemaphoreTake(KEY2_CountSemaphore,2)) i=1 ;
						break;
			
		case 3:	if(xSemaphoreTake(KEY3_CountSemaphore,2)) i=1 ;
						break;
		
		case 4:	if(xSemaphoreTake(KEY4_CountSemaphore,2)) i=1 ;
						break;
	}
	return i;
}

void beep(void)
{
 xSemaphoreGive(BEEP_CountSemaphore);
}

void led1(void)
{
 xSemaphoreGive(LED_CountSemaphore);
}

/*
	���������ڵ���ٶ�
	���룺speed���ٶ�ֵ       num��������
	��������·���1  ���򷵻�0
	˵����speed ��� 20000
*/
void Set_MotoSpeed(uint16_t speed, uint8_t num)
{
	if(speed>3500)speed=3500;
	if(speed<2000)speed=2000;
  SetTIM3Compare (speed, num);
}

/*
	��������ȡMPU6050����
	���룺struct MPU6050 
	������ɹ�����1  ���򷵻�0
	˵����
*/
uint8_t Get_MPU6050(struct MPU6050 *mpu)
{
	uint8_t flag=0;
	
	taskENTER_CRITICAL();           //�����ٽ���
	flag=ReadMpu(mpu) ;
	taskEXIT_CRITICAL();            //�˳��ٽ���
	
	if(flag) return 1;//�ɹ�����1
	else return 0;
}

void OUTput(uint8_t a)
{
	uint8_t q=0x03,p=0xfc,H,L;
	int16_t now_X=0,now_Y=0;
	int16_t expX=0,expY=0;
	
	now_X=((current_Angle.X*10));
	now_Y=((current_Angle.Y*10));
	
	expX=((int16_t)(exp_Angle.X*10));
	expY=((int16_t)(exp_Angle.Y*10));
	
		if(a)
	{
   printf_1_(&q,1);printf_1_(&p,1);//֡ͷ
	
		H=now_X>>8;
		L=now_X;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
		
		H=now_Y>>8;
		L=now_Y;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
		
		
		H=expX>>8;
		L=expX;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
		
		H=expY>>8;
		L=expY;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
	
   printf_1_(&p,1);printf_1_(&q,1);//֡β
  }
  else
	{
	printf("%.2f %.2f\n",current_Angle.X,current_Angle.Y);
//	printf("%f %f \n",exp_Angle.X,exp_Angle.Y);
	}
}

extern SemaphoreHandle_t USART_OUT_BinarySemaphore;//��APP.C
void OUTPUT ()
{
	xSemaphoreGive(USART_OUT_BinarySemaphore);	//�ͷŶ�ֵ�ź���
}

