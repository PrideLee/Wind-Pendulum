#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"

//#define KEY KEY_Scan()

#define KEY1 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9) //
#define KEY2 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)	//
#define KEY3 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7) //
#define KEY4  	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)	//

void KEY_Init(void);
uint8_t KEY_Scan(u8 key);

#endif
