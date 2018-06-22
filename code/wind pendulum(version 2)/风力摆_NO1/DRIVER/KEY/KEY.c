/*
	KEY_Init();����
	��ѯ��KEY_Scan();
*/

#include "KEY.h"
#include "GPIO.h"
#include "delay.h"

/* 
	���ܣ�KEY����
	���룺��
	�������
*/
void KEY_Init(void)
{
	GPIO_GeneralInInit(PF,GPIO_Pin_6,1);
	GPIO_GeneralInInit(PF,GPIO_Pin_7,1);
	GPIO_GeneralInInit(PF,GPIO_Pin_8,1);
	GPIO_GeneralInInit(PF,GPIO_Pin_9,1);
}

/* 
	���ܣ���ѯ�����Ƿ񱻰���
	���룺 key ����ѯ�Ǹ�������
	��������£�1
					��0
*/
uint8_t KEY_Scan(u8 key)
{	 
	uint8_t ON_flag=0;
	switch(key)
	{
		case 1:if(KEY1==0)ON_flag=1;break;
		case 2:if(KEY2==0)ON_flag=1;break;
		case 3:if(KEY3==0)ON_flag=1;break;
		case 4:if(KEY4==0)ON_flag=1;break;
		default : ON_flag=0;return 0;
	}
	
	if(ON_flag)
	{
		delay_ms(10);
		switch(key)
		{
			case 1:if(KEY1==0)while(KEY1==0){delay_ms(20);} ;return 1;
			case 2:if(KEY2==0)while(KEY2==0){delay_ms(20);} ;return 1;
			case 3:if(KEY3==0)while(KEY3==0){delay_ms(20);} ;return 1;
			case 4:if(KEY4==0)while(KEY4==0){delay_ms(20);} ;return 1;
			default : ON_flag=0;return 0;
		}
	}
	else return 0;
	
}
