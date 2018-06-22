#ifndef __SERVOPWMOUT_H
#define __SERVOPWMOUT_H
#include "stm32f4xx.h"

void PWMOut_TIM9(uint16_t Period, uint16_t Prescaler, uint16_t Duty);
void PWMOut_TIM12(uint16_t Period, uint16_t Prescaler, uint16_t Duty);

void SetPWMDuty_TIM9(uint8_t Duty, uint8_t Channel);
void SetPWMDuty_TIM12(uint8_t Duty, uint8_t Channel);

void SetTIM9Compare (uint16_t Data, uint8_t Channel);
void SetTIM12Compare(uint16_t Data, uint8_t Channel);

void TIM9Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);
void TIM12Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);

#endif
