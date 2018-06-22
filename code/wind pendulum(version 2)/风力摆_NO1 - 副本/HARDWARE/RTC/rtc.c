#include "rtc.h"
#include "LED.h"
#include "delay.h"
#include "usart.h"

/*********************************************************************************
*************************MCU���� STM32F407���Ŀ�����******************************
**********************************************************************************
* �ļ�����: rtc.c                                                                *
* �ļ�������RTC��ʼ����ʹ��                                                      *
* �������ڣ�2015.03.11                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����RTC��ʼ����ʹ��                                                      * 
**********************************************************************************
*********************************************************************************/

//RTCʱ�����ݽṹ�壬ʱ������ڷֿ�
/*  RTC_TimeStruct
(uint8_t)RTC_Hours
(uint8_t)RTC_Minutes
(uint8_t)RTC_Seconds
(uint8_t)RTC_H12       RTC_H12_AM=((uint8_t)0x00)   
											 RTC_H12_PM=((uint8_t)0x40)
*/
/*  RTC_DateStruct
(uint8_t)RTC_WeekDay
(uint8_t)RTC_Month
(uint8_t)RTC_Date
(uint8_t)RTC_Year
*/
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

NVIC_InitTypeDef   NVIC_InitStructure;

									 
u8 const month_amendBuf[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 
/****************************************************************************
* ��    ��: u8 RTC_GetWeek(u16 wyear,u8 wmonth,u8 wday)
* ��    �ܣ����ĳ�������ڼ�
* ��ڲ�����wyear����(���99)  wmonth����  wday����
* ���ز��������ڼ�
* ˵    ������2000~2099��Ч
            ����ֵ1-7���ζ�Ӧ����һ��������  
            �ú�����������ʱ��ʱ�������������ڼ���ȡʱ�����ڼ�ʱ����ʹ�øú�����
            ��ֱ�Ӷ�ȡ���ڼĴ���������ֵ����
            ע���βΡ��ꡱ��8λ����Ҫ����2λ����Ĳ���  15���Ϸ�    2015�����Ϸ�
�ҵı�ע��һ�㲻�õ����ڲ�����
****************************************************************************/																						 
u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday)  //
{	
	u16 middata;
	u8 yearL;
	
	yearL=wyear+100; 	 //��2000�꿪ʼ����100

	middata=yearL+yearL/4;
	middata=middata%7; 
	middata=middata+wday+month_amendBuf[wmonth-1];
	if (yearL%4==0&&wmonth<3)middata--;
	
	if((middata%7==0)) return 7;       //��STM32F103��rtc���̲�ͬ���ú��������췵��ֵΪ7
	return(middata%7);
}	
/****************************************************************************
* ��    ��: ErrorStatus RTC_SetTimes(u8 hour,u8 min,u8 sec)
* ��    �ܣ�����RTCʱ��
* ��ڲ�����������ʱ����
* ���ز������ɹ����  1���ɹ�   0��ʧ��
* ˵    ���� 
****************************************************************************/	
ErrorStatus RTC_SetTimes(u8 hour,u8 min,u8 sec)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;

	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	
	if(hour>12)
	     RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_PM;
	else RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
	
	return (RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure) );
}

//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date)
{
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=RTC_GetWeek(year,month,date);
//	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}


/****************************************************************************
* ��    ��: void RTC_GetTimes(uint32_t RTC_Format)
* ��    �ܣ���ȡRTCʱ��
* ��ڲ�����RTC_Format����ȡʱ�����ݵ���������  RTC_Format_BIN���ֽ���  RTC_Format_BCD��BCD����������
* ���ز�������
* ˵    ����   Ĭ�϶�ȡ�ֽ���  RTC_Format_BIN
****************************************************************************/
void RTC_GetTimes(void)//RTC_GetTimes(uint32_t RTC_Format)
{
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	  RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
}

//��һ����Ҫ����RTCʱ���øú���   // �ڲ�����
u8 RTC_Configuration(void)
{
  u16 wait=0;  
	RTC_InitTypeDef RTC_InitStructure;
	
	  RCC_LSEConfig(RCC_LSE_ON);   //����LSE   
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			{
			wait++;
				delay_ms(5);
				if(wait>=500)return 1;      //�糬��2.5s����û����,����������	 
			}
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//����ʱ���ʽΪ��24Сʱ��
    RTC_Init(&RTC_InitStructure);
			
		RTC_Set_Date(17,1,1);
		RTC_SetTimes(00,00,00);	
		return 0;
}
/****************************************************************************
* ��    ��: u8 RTC_InitConfig(void)
* ��    �ܣ�RTC��ʼ������
* ��ڲ�������
* ���ز������ɹ���� 0���ɹ�  1��ʧ��
* ˵    ���� 
****************************************************************************/
u8 RTC_InitConfig(void)
{
  u8 i=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	 //ʹ�ܺ󱸼Ĵ��� 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5555)		//�Ƿ��һ������
	{
		i=RTC_Configuration();
		if(i==1) return 1;  //RTC����ʧ��
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5555);	  //��ǳ�ʼ������
	} 
  RTC_SetWakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);	//����WAKE UP�ж�,1�����ж�һ��
	return 0;
}
/****************************************************************************
* ��    ��: void RTC_SetWakeUp(u32 wksel,u16 cnt)
* ��    �ܣ����û��Ѷ�ʱ����ʱ��
* ��ڲ�����wktime������ʱ��   autodata���Զ���װ��ֵ
* ���ز�������
* ˵    �������øú����Ļ���ʱ��Ϊ1S�ж�һ�Σ��൱��STM32F103�����е����ж� 
						���ģ��ں�����autodata-1��

wktime:
RTC_WakeUpClock_RTCCLK_Div16: RTC Wakeup Counter Clock = RTCCLK/16     	((uint32_t)0x00000000)
RTC_WakeUpClock_RTCCLK_Div8: RTC Wakeup Counter Clock = RTCCLK/8 				((uint32_t)0x00000001)
RTC_WakeUpClock_RTCCLK_Div4: RTC Wakeup Counter Clock = RTCCLK/4 				((uint32_t)0x00000002)
RTC_WakeUpClock_RTCCLK_Div2: RTC Wakeup Counter Clock = RTCCLK/2 				((uint32_t)0x00000003)
RTC_WakeUpClock_CK_SPRE_16bits: RTC Wakeup Counter Clock = CK_SPRE 			((uint32_t)0x00000004)
RTC_WakeUpClock_CK_SPRE_17bits: RTC Wakeup Counter Clock = CK_SPRE			((uint32_t)0x00000006)
****************************************************************************/
void RTC_SetWakeUp(u32 wktime,u16 autodata)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);        //�ر�WAKE UP
	
	RTC_WakeUpClockConfig(wktime); //����ʱ��ѡ��
	
	RTC_SetWakeUpCounter(autodata-1);//����WAKE UP�Զ���װ�ؼĴ���
		
	RTC_ClearITPendingBit(RTC_IT_WUT);  //���RTC WAKE UP�ı�־
  EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE); //����WAKE UP��ʱ���ж�
	RTC_WakeUpCmd( ENABLE);          //����WAKE UP ��ʱ����
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;            //LINE22
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //ʹ��LINE22
  EXTI_Init(&EXTI_InitStructure);                        //����
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        //�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);                              //����
}

/****************************************************************************
* ��    ��: void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
* ��    �ܣ���������ʱ��(����������,24Сʱ��)
* ��ڲ�����week:���ڼ�(1~7) ʱ����
* ���ز�������
* ˵    ���� �����޸Ŀ���P235 ������

****************************************************************************/
void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);  //�ȹر�����A 
	
  RTC_TimeTypeInitStructure.RTC_Hours=hour; 
	RTC_TimeTypeInitStructure.RTC_Minutes=min; 
	RTC_TimeTypeInitStructure.RTC_Seconds=sec; 
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;                               //����
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay; //��������
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;                        //ƥ�����ڣ�ʱ����
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
	RTC_ClearITPendingBit(RTC_IT_ALRA);  //���RTC����A�ı�־
  EXTI_ClearITPendingBit(EXTI_Line17); //���LINE17�ϵ��жϱ�־λ 
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);    //��������A�ж�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);    //��������A 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;            //LINE17
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //ʹ��LINE17
  EXTI_Init(&EXTI_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        //�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);                              //����
}


/****************************************************************************
* ��    ��: void RTC_WKUP_IRQHandler(void)
* ��    �ܣ�RTC WAKE UP�жϷ�����
* ��ڲ�������
* ���ز�������
* ˵    �����൱�����ж�
****************************************************************************/
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET) //WK_UP�ж�
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	          //����жϱ�־
		LED0=!LED0; 
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);      //����ж���22���жϱ�־ 								
}


//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET) //ALARM A�ж�  
	{
		LED2=!LED2;
		RTC_ClearFlag(RTC_FLAG_ALRAF);           //����жϱ�־
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	     //����ж���17���жϱ�־ 											 
}


void ShowTime(void)
{
	RTC_GetTimes();
	if(RTC_TimeStruct.RTC_H12==RTC_H12_PM)RTC_TimeStruct.RTC_Hours+=12;
	printf("\n");
	printf("%d-",RTC_TimeStruct.RTC_Hours);printf("%d-",RTC_TimeStruct.RTC_Minutes);printf("%d\n",RTC_TimeStruct.RTC_Seconds);
}
void ShowDay(void)
{
	RTC_GetTimes();
	printf("\n");
	printf("20%d-",RTC_DateStruct.RTC_Year);printf("%d-",RTC_DateStruct.RTC_Month);printf("%d  ",RTC_DateStruct.RTC_Date);printf("  week:%d\n",RTC_DateStruct.RTC_WeekDay);
}
void ShowDayTime(void)
{
	ShowDay();
	ShowTime();
}

void RTC_AlarmSta(uint8_t Num,uint8_t Sta)
{
	if(Num==1)
	{
		if(Sta==1)	RTC_AlarmCmd (RTC_Alarm_A,ENABLE);
		else RTC_AlarmCmd (RTC_Alarm_A,DISABLE);
	}
	if(Num==2)
	{
		if(Sta==1)	RTC_AlarmCmd (RTC_Alarm_B,ENABLE);
		else RTC_AlarmCmd (RTC_Alarm_B,DISABLE);
	}
}
void RTC_WakeUpSta(uint8_t Sta)
{
	if(Sta)	RTC_WakeUpCmd (ENABLE);
	else	RTC_WakeUpCmd (DISABLE);
}
//RTC_WakeUpCmd (); ENABLE / DISABLE
//RTC_AlarmCmd (,);  RTC_Alarm_A / RTC_Alarm_B  ,   ENABLE / DISABLE
                  



