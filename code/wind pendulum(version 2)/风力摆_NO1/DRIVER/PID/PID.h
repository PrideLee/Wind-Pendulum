#ifndef __PID_H
#define __PID_H
#include "stm32f4xx.h"

extern uint16_t r;  //���ַ�����ֵ

extern float X_P,X_I,X_D;      //PID����

extern float Y_P,Y_I,Y_D;      //PID����

extern float IE,DE;//�Ƿ���ֻ�΢��

float X_PID(float X_Rlt, float X_E);
float Y_PID(float Y_Rlt, float Y_E);

void moto(float XU_Out, float YU_Out );

void SetPID(uint8_t tasknum);

#endif
