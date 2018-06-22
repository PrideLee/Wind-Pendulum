/******************** (C) COPYRIGHT 2013 CEPARK多功能开发学习板********************
 * 文件名  ：key.c
 * 描述    ：按键应用函数库        
 * 实验平台：CEPARK多功能开发学习板
 * 硬件连接：-------------------------
 *          | PB08 - key1             |
 *          | PB09 - key2			  |
 *          | PB10 - key3             |
 *          | PB11 - key4			  |
 *          | PB12 - key5             |
 *          | PB13 - key6			  |
 *          | PB14 - key7             |
 *          | PB15 - key8			  |
 *           -------------------------
 * 库版本  ：ST3.5.0
**********************************************************************************/

#include "key.h" 
#include "delay.h" 

 /*
 * 函数名：Key_GPIO_Config
 * 描述  ：配置按键用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void Key_GPIO_Config(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	/*开启按键端口（PB0）的时钟*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

 	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
		/*检测是否有按键按下 */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	  {	   
	 	 /*延时消抖*/
	  	delay_ms(30);		
	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
					{	 
						/*等待按键释放 */
						//while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
			 			return 	KEY_ON;	 
					}
			else
					return KEY_OFF;
		}
	else
		return KEY_OFF;
}
