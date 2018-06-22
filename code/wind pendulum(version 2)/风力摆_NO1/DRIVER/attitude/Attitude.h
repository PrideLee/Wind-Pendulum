#ifndef __ATTITUDE_H
#define __ATTITUDE_H

#include "stm32f4xx.h"

extern float Kp;
extern float Ki;

struct Attitude
{
	float X;//  Êý¾Ý
	float Y;
};

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, struct Attitude *AT);
float My_Sqrt(float x)  ;

#endif
