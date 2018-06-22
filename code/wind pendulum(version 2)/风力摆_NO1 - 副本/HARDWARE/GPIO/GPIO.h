#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"

#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PF 5
#define PG 6
void GPIO_GeneralOutInit(uint8_t Px , uint32_t GPIO_Pin);//IO����ͨ�����������
void GPIO_GeneralInInit(uint8_t Px , uint32_t GPIO_Pin, uint8_t Sta);//IO����ͨ��������������
void EXTI_GeneralInInit(uint8_t Px , uint32_t GPIO_Pin, uint8_t Sta , uint8_t Preemption , uint8_t Sub);//IO����ͨ�ж���������

#endif
