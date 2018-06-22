#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

 /*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
 
#define KEY_ON	0
#define KEY_OFF	1

#define KEY1	GPIOB,GPIO_Pin_10
#define KEY2	GPIOB,GPIO_Pin_11
#define KEY3	GPIOB,GPIO_Pin_12
#define KEY4	GPIOB,GPIO_Pin_13
#define KEY5	GPIOB,GPIO_Pin_14
#define KEY6	GPIOB,GPIO_Pin_15

void Key_GPIO_Config(void);
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

#endif /* __KEY_H */

