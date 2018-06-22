#ifndef __OLED_H
#define __OLED_H			  	 

#include "stm32f10x.h"
		    						  
//-----------------OLED端口定义----------------  					   

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define OLED_RS_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define OLED_RS_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 
void LCD_Set_Pos(u8 x, u8 y);
void LCD_P16x16Ch(u8 x,u8 y,u8 N);
void LCD_P8x16Ch(u8 x,u8 y,u8 N);

//-------------------------------自定义函数---------------------------------
void LCD_numAletter(u8 x,u8 y,u8 N);
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

//显示主界面
//ArrowPosition参数为箭头的所在的行数，取值范围1~4
void OledShowMainInterface(u8);


//显示正弦波二级菜单
//WaveKind 为波形类型，取值范围为1~4
//Amplitude 为幅值，显示取值范围为0~9.9
//Frequency 为频率，显示取值范围为0~100
void OledShowSecondInterface(u8 WaveKind , int Amplitude , int Frequency);

////显示欢迎页面“二组欢迎你”
//void OledShowWelcome(void);

void OledShowWorking(void);

void OledShowWelcome(void);
#endif
