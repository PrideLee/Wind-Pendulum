#include "LED.h"
#include "GPIO.h"

/* 
	���ܣ�LED����
	���룺��
	�������
*/
void LED_Init(void)
{
	GPIO_GeneralOutInit(PE,GPIO_Pin_3);
	GPIO_GeneralOutInit(PE,GPIO_Pin_4);
	GPIO_GeneralOutInit(PG,GPIO_Pin_9);
}

/* 
	���ܣ�LED����
	���룺Num ��LED���
				Set ��0 ��  1 ��
	�������
*/
void LED_set(uint8_t Num,uint8_t Set)
{
	uint8_t num=0,set=0;
	num=Num;
	set=Set;
	switch(num)
	{
		case 0:LED0=set;break;
		case 1:LED1=set;break;
		case 2:LED2=set;break;
	}
		
}
