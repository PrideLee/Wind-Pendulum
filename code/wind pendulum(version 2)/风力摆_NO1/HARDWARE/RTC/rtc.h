#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////	 

extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;
	
u8 RTC_InitConfig(void);						//RTC��ʼ��

ErrorStatus RTC_SetTimes(u8 hour,u8 min,u8 sec);//����ʱ��
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date);//��������
void RTC_GetTimes(void);//RTC_GetTimes(uint32_t RTC_Format);//��ȡʱ������

void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec);	//��������ʱ��(����������,24Сʱ��)
void RTC_SetWakeUp(u32 wktime,u16 autodata);				//���û��Ѷ�ʱ����ʱ��

void RTC_CloseAlarmA(void);
void RTC_OpenAlarmA(void);

u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday);//�ڲ�����

void ShowTime(void);
void ShowDay(void);
void ShowDayTime(void);

void RTC_AlarmSta(uint8_t Num,uint8_t Sta);
void RTC_WakeUpSta(uint8_t Sta);

#endif

















