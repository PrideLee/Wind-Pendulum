/******************** (C) COPYRIGHT 2013 CEPARK多功能开发学习板********************
 * 文件名  ：oled.c
 * 描述    ：oled 应用函数库
 * 硬件连接：-----------------------
 * 单片机IO | PA1  - JP51 - DIN     | OLED模块
 *          | PA2  - JP51 - SCK     | 
 *          | PA3  - JP51 - DC      |
 *          | PA4  - JP51 - RES     | 
 *          | PA5  - JP51 - CS      |       
 * 实验平台：CEPARK多功能开发学习板
																				 
**********************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"	
#include "key.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
u8 OLED_GRAM[128][8];	 

/*******************************************************
字体：宋体
字宽：16
字高：16
点阵格式：阴码
取模走向：顺向
取模方式：列行式
输出进制：16进制
像素大小：8
*******************************************/
u8 const F16x16[] = {
0x00, 0x20, 0x20, 0x20, 0xFF, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0xFF, 0x20, 0x20, 0x20, 0x00, 0x00,
0x88, 0x88, 0x91, 0xA1, 0xC9, 0x89, 0x89, 0xBF, 0x89, 0x89, 0xC9, 0xA1, 0x91, 0x88, 0x88, 0x00,//基0

0x00, 0x08, 0x08, 0x08, 0x08, 0x0B, 0x0C, 0xFF, 0x0C, 0x0B, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
0x08, 0x10, 0x20, 0x40, 0x90, 0x10, 0x10, 0xFF, 0x10, 0x10, 0x90, 0x40, 0x20, 0x10, 0x08, 0x00,//本1

0x00, 0x40, 0x4F, 0x49, 0x49, 0x7F, 0x49, 0x49, 0x49, 0x7F, 0x49, 0x49, 0x4F, 0x40, 0x00, 0x00,
0x00, 0x40, 0x41, 0x49, 0x59, 0x6A, 0xC6, 0x44, 0x4C, 0x52, 0x62, 0x42, 0x41, 0x40, 0x00, 0x00,//要2

0x00, 0x10, 0x14, 0x12, 0x11, 0x10, 0x10, 0xFF, 0x11, 0x10, 0x91, 0x52, 0x14, 0x10, 0x00, 0x00,
0x00, 0x08, 0x08, 0x10, 0x20, 0x42, 0x81, 0xFE, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x08, 0x00,//求3

0x00, 0x00, 0x18, 0x68, 0x08, 0x0B, 0x1D, 0xE9, 0x09, 0x09, 0x09, 0x49, 0x29, 0x08, 0x00, 0x00,
0x00, 0x04, 0x08, 0x31, 0xC1, 0x01, 0x82, 0x62, 0x14, 0x08, 0x14, 0x22, 0xC2, 0x01, 0x01, 0x00,//发4

0x08, 0x08, 0x08, 0xFF, 0x08, 0x19, 0x64, 0x44, 0x47, 0x5C, 0x45, 0x44, 0x44, 0x54, 0x60, 0x00,
0x20, 0x22, 0x41, 0xFE, 0x80, 0x08, 0x08, 0xC8, 0x48, 0x48, 0xFF, 0x48, 0x48, 0x48, 0x08, 0x00,//挥5

0x02, 0x22, 0x2A, 0xA6, 0x62, 0x22, 0x26, 0x2A, 0x22, 0x02, 0x7F, 0x40, 0x44, 0x5B, 0x60, 0x00,
0x00, 0x00, 0x7E, 0x44, 0x44, 0x44, 0x44, 0x7E, 0x00, 0x00, 0xFF, 0x10, 0x08, 0x10, 0xE0, 0x00,//部6

0x01, 0x02, 0x04, 0x09, 0x11, 0x61, 0x01, 0x01, 0x01, 0xC1, 0x31, 0x08, 0x04, 0x02, 0x01, 0x00,
0x00, 0x01, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x02, 0x01, 0x02, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00,//分7


0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x09, 0x05, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,//→8

0x01, 0x01, 0x01, 0x01, 0xFF, 0x01, 0x01, 0x05, 0x09, 0x11, 0x21, 0x41, 0x01, 0x01, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0x02, 0x84, 0x48, 0x20, 0x10, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00,//长9

0x00, 0x00, 0x3F, 0x24, 0x24, 0x24, 0x3F, 0xA4, 0x64, 0x24, 0x3F, 0x24, 0x24, 0x24, 0x20, 0x00,
0x02, 0x0C, 0xF1, 0x01, 0x21, 0x32, 0xAA, 0xA4, 0xA4, 0xA4, 0xAA, 0x32, 0x01, 0x01, 0x01, 0x00,//度10

0x04, 0x08, 0x17, 0x24, 0xE4, 0x24, 0x24, 0x27, 0x24, 0x2C, 0x34, 0x04, 0x07, 0x00, 0x00, 0x00,
0x01, 0x06, 0xF8, 0x90, 0x90, 0x90, 0x90, 0xFE, 0x90, 0x90, 0x92, 0x91, 0xFE, 0x00, 0x00, 0x00,//角11

0x00, 0x00, 0x3F, 0x24, 0x24, 0x24, 0x3F, 0xA4, 0x64, 0x24, 0x3F, 0x24, 0x24, 0x24, 0x20, 0x00,
0x02, 0x0C, 0xF1, 0x01, 0x21, 0x32, 0xAA, 0xA4, 0xA4, 0xA4, 0xAA, 0x32, 0x01, 0x01, 0x01, 0x00,//度12


0x00,0x00,0x42,0x22,0x1A,0x02,0x02,0xFF,0x02,0x02,0x0A,0x12,0x62,0x00,0x00,0x00,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,//半13

0x00,0x08,0x11,0x23,0xCC,0x01,0x41,0x42,0x44,0x48,0x58,0x64,0x44,0x02,0x01,0x00,
0x40,0x80,0x00,0xFF,0x00,0x02,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0x02,0x00,//径14



0x02,0x42,0x33,0x00,0x00,0x22,0x2A,0x2A,0x2A,0xFE,0x2A,0x2A,0x2A,0x22,0x02,0x00,
0x00,0x00,0xFE,0x04,0x08,0x00,0xFF,0xA8,0xA8,0xA8,0xAA,0xA9,0xFE,0x00,0x00,0x00,//请15

0x24,0x24,0x25,0x7F,0xC4,0x44,0x00,0x47,0x34,0x04,0xFC,0x04,0x14,0x67,0x00,0x00,
0x10,0x60,0x80,0xFF,0x80,0x60,0x00,0xFF,0x90,0x90,0x90,0x92,0x91,0xFE,0x00,0x00,//稍16

0x00,0x00,0x00,0x3F,0x24,0x24,0x24,0x24,0x44,0x44,0x44,0xC4,0x44,0x04,0x04,0x00,
0x02,0x04,0x18,0xE0,0x00,0x7F,0x42,0x42,0x42,0x42,0x42,0x42,0x7F,0x00,0x00,0x00,//后17

0x02,0x02,0x42,0x33,0x00,0x00,0x3F,0x24,0x25,0x24,0x44,0x44,0xC5,0x46,0x00,0x00,
0x00,0x02,0x04,0xF8,0x04,0x1A,0xE2,0x0A,0x12,0xA2,0x42,0xA2,0x12,0x0A,0x02,0x00,//返18

0x00,0x00,0x7F,0x40,0x40,0x4F,0x48,0x48,0x48,0x4F,0x40,0x40,0x7F,0x00,0x00,0x00,
0x00,0x00,0xFE,0x04,0x04,0xE4,0x24,0x24,0x24,0xE4,0x04,0x04,0xFE,0x00,0x00,0x00,//回19

0x02,0x0C,0xF7,0x24,0x24,0x01,0x27,0x39,0x08,0x2A,0x2A,0xFF,0x2A,0x3E,0x08,0x00,
0x80,0x80,0xFE,0x84,0x8A,0x64,0x18,0xE4,0x22,0xA2,0xA2,0xFA,0xA2,0xA2,0x22,0x00,//键20

0x02,0x02,0x42,0x33,0x00,0x00,0xFF,0x92,0x92,0x93,0x92,0x92,0xFE,0x01,0x00,0x00,
0x00,0x02,0x04,0xF8,0x04,0x02,0xFA,0x12,0x22,0x02,0x82,0x42,0xA2,0x1A,0x02,0x00,//退21

0x00,0x00,0x3E,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x02,0x02,0x3F,0x00,0x00,0x00,
0x00,0x3E,0x02,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x02,0x02,0x3F,0x00,0x00,//出22

0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//一23

0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,//二24

0x00,0x20,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x20,0x00,0x00,
0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//三25

0x00,0x3F,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x3F,0x00,0x00,
0x00,0xFE,0x14,0x24,0xC4,0x04,0x04,0x04,0x04,0x84,0x44,0x44,0x44,0xFE,0x00,0x00,//四26

0x00,0x40,0x42,0x42,0x42,0x43,0x7E,0x42,0x42,0x42,0x42,0x43,0x40,0x40,0x00,0x00,
0x02,0x02,0x02,0x02,0x1E,0xE2,0x02,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x00,//五27

0x04,0x04,0x04,0x04,0x04,0x04,0x84,0x44,0x34,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
0x00,0x02,0x04,0x08,0x30,0xC0,0x00,0x00,0x00,0x80,0x40,0x20,0x18,0x06,0x00,0x00,//六28

0x00,0x40,0x30,0x07,0x02,0x04,0x1F,0xF2,0x12,0x92,0x7F,0x12,0x12,0x12,0x10,0x00,
0x20,0x20,0xFE,0x00,0x00,0x00,0xFF,0x44,0x44,0x44,0xFC,0x44,0x44,0x44,0x04,0x00,//准29

0x01,0x09,0x09,0x12,0x32,0xEA,0x24,0x24,0x24,0x2A,0x32,0x22,0x01,0x01,0x01,0x00,
0x00,0x00,0x00,0xFF,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0xFF,0x00,0x00,0x00,0x00,//备30

0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0xFF,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00,
0x00,0x00,0xF0,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0xF0,0x00,0x00,0x00,//中31
};


u8 const F8x16[] = {
0x00,0x0E,0x11,0x10,0x10,0x10,0x1C,0x00,0x00,0x1C,0x04,0x84,0x84,0x44,0x38,0x00,/*"S",0*/
0x18,0x10,0x10,0x1F,0x10,0x10,0x18,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,/*"T",1*/
0x10,0x1F,0x1F,0x00,0x1F,0x1F,0x10,0x00,0x04,0xFC,0x00,0xFC,0x00,0xFC,0x04,0x00,/*"M",2*/
0x00,0x0C,0x10,0x11,0x11,0x12,0x0C,0x00,0x00,0x18,0x04,0x04,0x04,0x88,0x70,0x00,/*"3",3*/
0x00,0x0E,0x10,0x10,0x10,0x11,0x0E,0x00,0x00,0x0C,0x14,0x24,0x44,0x84,0x0C,0x00,/*"2",4*/
0x10,0x1F,0x11,0x11,0x17,0x10,0x08,0x00,0x04,0xFC,0x04,0x00,0xC0,0x00,0x00,0x00,/*"F",5*/
0x00,0x08,0x08,0x1F,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,/*"1",6*/
0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0xF0,0x08,0x04,0x04,0x08,0xF0,0x00,/*"0",7*/
0x00,0x0C,0x10,0x11,0x11,0x12,0x0C,0x00,0x00,0x18,0x04,0x04,0x04,0x88,0x70,0x00,/*"3",8*/
0x03,0x0C,0x10,0x10,0x10,0x10,0x1C,0x00,0xE0,0x18,0x04,0x04,0x04,0x08,0x10,0x00,/*"C",9*/
0x00,0x0E,0x11,0x10,0x10,0x11,0x0E,0x00,0x00,0x38,0x44,0x84,0x84,0x44,0x38,0x00,/*"8",10*/
0x18,0x10,0x10,0x1F,0x10,0x10,0x18,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,/*"T",11*/
0x00,0x07,0x08,0x11,0x11,0x18,0x00,0x00,0x00,0xF0,0x88,0x04,0x04,0x88,0x70,0x00,/*"6",12*/   
0x10,0x1F,0x10,0x00,0x00,0x00,0x00,0x00,0x04,0xFC,0x04,0x04,0x04,0x04,0x0C,0x00,/*"L",0*/
0x10,0x1F,0x11,0x11,0x17,0x10,0x08,0x00,0x04,0xFC,0x04,0x04,0xC4,0x04,0x18,0x00,/*"E",1*/
0x00,0x00,0x03,0x1C,0x07,0x00,0x00,0x00,0x04,0x3C,0xC4,0x40,0x40,0xE4,0x1C,0x04,/*"A",2*/
0x10,0x1F,0x11,0x11,0x11,0x11,0x0E,0x00,0x04,0xFC,0x04,0x00,0xC0,0x30,0x0C,0x04,/*"R",3*/
0x10,0x1F,0x0C,0x03,0x00,0x10,0x1F,0x10,0x04,0xFC,0x04,0x00,0xE0,0x18,0xFC,0x00,/*"N",4*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",5*/
0x00,0x0E,0x11,0x10,0x10,0x10,0x1C,0x00,0x00,0x1C,0x04,0x84,0x84,0x44,0x38,0x00,/*"S",6*/
0x18,0x10,0x10,0x1F,0x10,0x10,0x18,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,/*"T",7*/
0x00,0x00,0x03,0x1C,0x07,0x00,0x00,0x00,0x04,0x3C,0xC4,0x40,0x40,0xE4,0x1C,0x04,/*"A",8*/
0x10,0x1F,0x11,0x11,0x11,0x11,0x0E,0x00,0x04,0xFC,0x04,0x00,0xC0,0x30,0x0C,0x04,/*"R",9*/
0x18,0x10,0x10,0x1F,0x10,0x10,0x18,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,/*"T",10*/
0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0x0C,0x00,0x00,0x00,/*"!",11*/	
};
//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}


//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   
//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 //PD3,PD6推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
  							  
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
}  


void LCD_Set_Pos(u8 x, u8 y)
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
} 

//16*16字写函数								//***********************************
void LCD_P16x16Ch(u8 x,u8 y,u8 N)
{
	u8 wm=0;
	u16 adder=32*N;    	
  LCD_Set_Pos(x , y);
  	for(wm = 0;wm < 16;wm++)               
  	{
  		OLED_WR_Byte(F16x16[adder],OLED_DATA); 
  		adder += 1;
  	}      
  	LCD_Set_Pos(x,y-1); 
  	for(wm = 0;wm < 16;wm++)          
  	{
  		OLED_WR_Byte(F16x16[adder],OLED_DATA); 
  		adder += 1;
  	} 	  	
}

//8*16字符写函数
void LCD_P8x16Ch(u8 x,u8 y,u8 N)
{
	u8 wm=0;
	u16 adder=16*N;    	
  LCD_Set_Pos(x , y);
  	for(wm = 0;wm < 8;wm++)               
  	{
  		OLED_WR_Byte(F8x16[adder],OLED_DATA); 
  		adder += 1;
  	}
  	LCD_Set_Pos(x,y-1); 
  	for(wm = 0;wm < 8;wm++)          
  	{
  		OLED_WR_Byte(F8x16[adder],OLED_DATA); 
  		adder += 1;
  	} 	  	
}


//----------------------------------------------------------------------------------
/*
  自定义显示函数
*/
void LCD_numAletter(u8 x,u8 y,u8 N)
{
	u8 wm=0;
	u16 adder=16*N;    	
  LCD_Set_Pos(x , y);
  	for(wm = 0;wm < 8;wm++)               
  	{
  		OLED_WR_Byte(numAletter[adder],OLED_DATA); 
  		adder += 1;
  	}
  	LCD_Set_Pos(x,y-1); 
  	for(wm = 0;wm < 8;wm++)          
  	{
  		OLED_WR_Byte(numAletter[adder],OLED_DATA); 
  		adder += 1;
  	} 	  	
}


void OledShowWelcome()
{
	OLED_Clear();
	
	LCD_P16x16Ch(1,5,29);//准备中	
	LCD_P16x16Ch(17,5,30);//
	LCD_P16x16Ch(33,5,31);//
	LCD_numAletter(49,5,36);//,
	
	LCD_P16x16Ch(33,3,15);//请	
	LCD_P16x16Ch(49,3,16);//稍
	LCD_P16x16Ch(65,3,17);//后
	LCD_numAletter(81,3,38);// .
	LCD_numAletter(89,3,38);// .
	LCD_numAletter(97,3,38);// .
}

void OledShowMainInterface(u8 ArrowPosition)
{
	u8 ArrowColumn,ArrowLine;
	
	OLED_Clear();
	
	LCD_P16x16Ch(1,7,0);//基	
	LCD_P16x16Ch(18,7,1);//本
	LCD_P16x16Ch(34,7,2);//要
	LCD_P16x16Ch(50,7,3);//求
    
	LCD_P16x16Ch(1,3,4);//发		
	LCD_P16x16Ch(18,3,5);//挥	
	LCD_P16x16Ch(34,3,6);//部
	LCD_P16x16Ch(50,3,7);//分
	
    LCD_numAletter(16,5,1);
	LCD_numAletter(48,5,2);
	LCD_numAletter(80,5,3);
	LCD_numAletter(112,5,4);
	LCD_numAletter(16,1,1);
	LCD_numAletter(48,1,2);
    LCD_numAletter(80,1,3);
	switch(ArrowPosition)
	{
		case 1:
			ArrowLine=5;
			ArrowColumn=0;
			break;
		case 2:
			ArrowLine=5;
			ArrowColumn=32;
			break;
		case 3:
			ArrowLine=5;
			ArrowColumn=64;
			break;
		case 4:
			ArrowLine=5;
			ArrowColumn=96;
			break;
		case 5:
			ArrowLine=1;
			ArrowColumn=0;
			break;
		case 6:
			ArrowLine=1;
			ArrowColumn=32;
			break;
		case 7:
			ArrowLine=1;
			ArrowColumn=64;
			break;
		default:
			break;
	}
	LCD_P16x16Ch(ArrowColumn,ArrowLine,8);//箭头
}

void OledShowWorking(void)
{
	OLED_Clear();
	
	LCD_P16x16Ch(1,5,15);//请	
	LCD_P16x16Ch(17,5,16);//稍
	LCD_P16x16Ch(33,5,17);//后
	LCD_numAletter(49,5,38);// .
	LCD_numAletter(57,5,38);// .
	LCD_numAletter(65,5,38);// .
    
	LCD_P16x16Ch(1,3,18);//返回键退出		
	LCD_P16x16Ch(17,3,19);//	
	LCD_P16x16Ch(33,3,20);//
	LCD_P16x16Ch(49,3,21);//
	LCD_P16x16Ch(65,3,22);//
	
}


void OledShowSecondInterface(u8 WaveKind , int Length , int Angle)
{
	u8 LengthTen;	//存放幅值的十位数
	u8 LengthOne;	//存放幅值的个位数
	
	u8 AngleHundred;//存放频率的百位数
	u8 AngleTen;
	u8 AngleOne;

	LengthTen = Length/10;
	LengthOne = (Length-LengthTen*10)/1;
	
	AngleHundred = Angle/100;
	AngleTen = (Angle-AngleHundred*100)/10;
	AngleOne = Angle%10;
	
	OLED_Clear();
	
	switch(WaveKind)
	{
		case 1:
			LCD_P16x16Ch(1,7,0);//基	
			LCD_P16x16Ch(18,7,1);//本
			LCD_P16x16Ch(34,7,2);//要
			LCD_P16x16Ch(50,7,3);//求
			LCD_P16x16Ch(66,7,23);//一	    
			break;
		
		case 2:
			LCD_P16x16Ch(1,7,0);//基	
			LCD_P16x16Ch(18,7,1);//本
			LCD_P16x16Ch(34,7,2);//要
			LCD_P16x16Ch(50,7,3);//求
			LCD_P16x16Ch(66,7,24);//二	
			LCD_P16x16Ch(1,5,9);
			LCD_P16x16Ch(18,5,10);
			LCD_numAletter(40,5,LengthTen);//显示长度大小
			LCD_numAletter(50,5,LengthOne);
			break;
		
		case 3:
			LCD_P16x16Ch(1,7,0);//基	
			LCD_P16x16Ch(18,7,1);//本
			LCD_P16x16Ch(34,7,2);//要
			LCD_P16x16Ch(50,7,3);//求
			LCD_P16x16Ch(66,7,25);//三	
			LCD_P16x16Ch(1,5,11);
			LCD_P16x16Ch(18,5,12);
			LCD_numAletter(76,5,AngleHundred);//显示频率大小
			LCD_numAletter(84,5,AngleTen);
			LCD_numAletter(92,5,AngleOne);	
			LCD_P16x16Ch(1,3,9);
			LCD_P16x16Ch(18,3,10);
			LCD_numAletter(76,3,LengthTen);//显示长度大小
			LCD_numAletter(84,3,LengthOne);
			break;
		
		case 4:
			LCD_P16x16Ch(1,7,0);//基	
			LCD_P16x16Ch(18,7,1);//本
			LCD_P16x16Ch(34,7,2);//要
			LCD_P16x16Ch(50,7,3);//求
			LCD_P16x16Ch(66,7,26);//四
			break;
		
		case 5:
			LCD_P16x16Ch(1,7,4);//发		
			LCD_P16x16Ch(18,7,5);//挥	
			LCD_P16x16Ch(34,7,6);//部
			LCD_P16x16Ch(50,7,7);//分
			LCD_P16x16Ch(66,7,23);//一	
			LCD_P16x16Ch(1,5,13);
			LCD_P16x16Ch(18,5,14);
			LCD_numAletter(76,5,LengthTen);//显示直径大小
			LCD_numAletter(84,5,LengthOne);
			break;
		
		case 6:
			LCD_P16x16Ch(1,7,4);//发		
			LCD_P16x16Ch(18,7,5);//挥	
			LCD_P16x16Ch(34,7,6);//部
			LCD_P16x16Ch(50,7,7);//分	
			LCD_P16x16Ch(66,7,24);//二
			LCD_P16x16Ch(1,5,13);
			LCD_P16x16Ch(18,5,14);
			LCD_numAletter(76,5,LengthTen);//显示直径大小
			LCD_numAletter(84,5,LengthOne);
			break;		
		case 7:
			LCD_P16x16Ch(1,7,4);//发		
			LCD_P16x16Ch(18,7,5);//挥	
			LCD_P16x16Ch(34,7,6);//部
			LCD_P16x16Ch(50,7,7);//分	
			LCD_P16x16Ch(66,7,25);//三
			LCD_P16x16Ch(1,5,13);
			LCD_P16x16Ch(18,5,14);
			LCD_numAletter(76,5,LengthTen);//显示直径大小
			LCD_numAletter(84,5,LengthOne);
			break;
		
		default:
			break;
	}
}






















