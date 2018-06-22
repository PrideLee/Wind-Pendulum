#ifndef TIME3_PWM_MODE_H
#define TIME3_PWM_MODE_H

#include "stm32f10x.h"

void Time3pwmGPIO_Config(void);
void TIM3_Config(u16 duty_ratio);
void pwm_ratio(u16 duty_ratio);

// TIM_SetCompare1();可调用的库函数

#endif
