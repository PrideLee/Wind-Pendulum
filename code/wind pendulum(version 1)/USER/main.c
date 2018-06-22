#include "main.h"
#include <string.h>
#include <stdio.h>
#include "key.h" 
#include "oled.h"
#include "bazz.h"
int phshu = 0;							//����ͣ���ж���
int16_t LAST_PITCH=0,LAST_ROLL=0;		//��¼�ϴεĸ����������
int16_t delta_PITCH=0,delta_ROLL=0;		//ǰ�����εĽǶȱ仯ֵ

int16_t last_delta_PITCH=0,last_delta_ROLL=0;		//����Ҫ��2 ��һ�εĽǶȱ仯ֵ
float PresentAngleRange_fb=0;						//����Ҫ��2 ��ǰǰ��ڶ���ֵ���Ƕ�ֵ��
float PresentAngleRange_lr=0;						//����Ҫ��2 ��ǰ���Ұڶ���ֵ���Ƕ�ֵ��

u8 InputRange_fb=0; 						//����Ҫ��2 ���õİڶ����ȣ���λΪcm
u8 InputRange_lr=0;

int   	slash_angle=30;					//����Ҫ��3 �趨�ĽǶ�ֵ��0-360��
u8 		Radius=15;						//��չҪ��1 �趨Բ�İ뾶��cm��
int choose_task_count=0;
int  slash_range=25;
u8 Divisiontime=0;						//��ʱ����ֵ
u16 swing_ready_count=0;				//�ڶ��Ƿ����Ҫ�����ֵ
float	error_range=0; double   EllipseDuan=10,EllipseChang=15;


double RangeOut_fb;			//ͨ�ù̶��ڷ�PID���ֵ
double RangeOut_lr;

double SlashFbPidIn;		//б��ǯλPID����
double SlashLrPidIn;

double SlashOutFb;			//б��ǯλPID���
double SlashOutLr;

double RoundOutFb;			//��ԲǯλPID���
double RoundOutLr;

double RoundLrPidIn;		//��ԲǯλPID����			
double RoundFbPidIn;

double EllipseOutFb;		//����ԲǯλPID���
double EllipseOutLr;

double EllipseLrPidIn;		//����ԲǯλPID����			
double EllipseFbPidIn;

double pid_slash_fb_Derivative=0;
double pid_slash_lr_Derivative=0;
u8 slash_special=0;			//Ϊ0ʱ��һ��б�ߴ���Ϊ1ʱ��0�ȴ���Ϊ2ʱ��90�ȴ���

double pid_round_fb_Proportion=0;
double pid_round_lr_Proportion=0;
double pid_round_fb_Integral  =0;
double pid_round_lr_Integral  =0;
double pid_round_fb_Derivative=0;
double pid_round_lr_Derivative=0;

double pid_ellipse_fb_Proportion=0;
double pid_ellipse_lr_Proportion=0;
double pid_ellipse_fb_Integral=0;
double pid_ellipse_lr_Integral=0;
double pid_ellipse_fb_Derivative=0;
double pid_ellipse_lr_Derivative=0;

/*************************************************************  λ��ʽPID�㷨   ***************************************************************/
typedef struct pid {
        double  SetPoint;           //  �趨Ŀ�� Desired Value
        double  Proportion;         //  �������� Proportional Const        
        double  Integral;           //  ���ֳ��� Integral Const        
        double  Derivative;         //  ΢�ֳ��� Derivative Const
        double  LastError;          //  Error[-1]        
        double  PrevError;          //  Error[-2]        
        double  SumError;           //  sums of Errors
		double  Out;
		double  OutMax;
		double  OutMin;
		double  SumErrorMax;
		double  SumErrorMin;
}pid;

/*================================  pid���㲿��===============================*/
double pidCalc( struct pid *pp, double NextPoint )
{    
    double  dError,Error;
        
    Error = pp->SetPoint -  NextPoint;          // ƫ��      
    pp->SumError += Error;                      // ����        
    dError = pp->LastError - pp->PrevError;     // ��ǰ΢��   
	
    pp->PrevError = pp->LastError;        
    pp->LastError = Error;    
    
	//�����ֱ���
	if(pp->SumError > pp->SumErrorMax)	pp->SumError=pp->SumErrorMax;	
	if(pp->SumError < pp->SumErrorMin)	pp->SumError=pp->SumErrorMin;
	
	pp->Out=pp->Proportion * Error              // ������           
		 +   pp->Integral * pp->SumError         // ������            
		 +   pp->Derivative * dError    ;         // ΢���� 
	
	//����޷�
	if(pp->Out > pp->OutMax)	pp->Out=pp->OutMax;
	if(pp->Out < pp->OutMin)	pp->Out=pp->OutMin;
	
	
    return pp->Out;
}

/*=======================  ��ʼ����pid�ṹ  Initialize pid Structure===========================*/
void pidInit(struct pid *pp)
{    
        memset(pp,0,sizeof(struct pid));
}
/*************************************************************  λ��ʽPID�㷨  ���� ***************************************************************/




int main(void)
{ 	
	pid pid_lr_to_zero;		//����PID
	pid pid_fb_to_zero;
	pid pid_fb_range;		//ǰ��ڷ�PID
	pid pid_lr_range;		//���Ұڷ�PID
	pid pid_slash_fb;			//б��ǰ��PID
	pid pid_slash_lr;			//б������PID
	pid pid_round_fb;			//��Բǰ��
	pid pid_round_lr;			//��Բ����
	pid pid_ellipse_fb;			//��Բ
	pid pid_ellipse_lr;
	
	SystemInit();						//ϵͳʱ�ӵȳ�ʼ��
	delay_init(72);	     				//��ʱ��ʼ��
	NVIC_Configuration();				//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);					//���ڳ�ʼ��Ϊ115200	
	OLED_Init();
	OledShowWelcome();					//��ʾ��ӭҳ
	mpu_6050_usart_init();					
	Time3pwmGPIO_Config();				//TIM3����Ϊ���4·PWM��
	TIM3_Config(0);	
	Key_GPIO_Config();	
	BuzzInit();
	//*************************������ ��ʼ*************************
	MainInterface:
	BazzStop();
	swing_ready_count=0;
	while(1)												
	{
		OledShowMainInterface(choose_task_count+1);
		delay_ms(70);
		if( Key_Scan(KEY2) == KEY_ON  )
		{
			choose_task_count++;
			if(choose_task_count > 6)
				choose_task_count = 0;
		}
		if( Key_Scan(KEY1) == KEY_ON  )
		{
			choose_task_count--;
			if(choose_task_count < 0)
				choose_task_count = 6;
		}
		
		
		if( Key_Scan(KEY6) == KEY_ON )
		{
			while(Key_Scan(KEY6) == KEY_ON);
			break;
		}
	}
	//*************************������ ����*************************
	
	//*************************�������� ��ʼ*************************
	switch (choose_task_count)
	{
		case 0: 												//***����һ***
			InputRange_fb=30;
			//ˢ����ʾ
			OledShowSecondInterface(choose_task_count+1,0,0);
			turn_frontORback( 59999);						
			delay_ms(300);
			break;
		
		case 1:													//***�����***		
			InputRange_fb=25;
			while(1)
			{
				//ˢ����ʾ
				OledShowSecondInterface(choose_task_count+1,InputRange_fb,0);
				delay_ms(70);
				if( Key_Scan(KEY1) == KEY_ON  )
				{
					InputRange_fb++;
					if(InputRange_fb > 30)
						InputRange_fb = 15;
				}
				if( Key_Scan(KEY2) == KEY_ON  )
				{
					InputRange_fb--;
					if(InputRange_fb < 15)
						InputRange_fb = 30;
				}
				
				if( Key_Scan(KEY6) == KEY_ON )
				{
					while(Key_Scan(KEY6) == KEY_ON);
					break;
				}	
				if( Key_Scan(KEY5) == KEY_ON )
				{
					goto MainInterface;
				}
			}
			turn_frontORback( 29999);						
			delay_ms(300);
			break;
			
		case 2:													//***������***
			slash_special=0;
			while(1)
			{
				//ˢ����ʾ
				OledShowSecondInterface(choose_task_count+1,slash_range,slash_angle);
				delay_ms(60);
				//���µ����ڷ�
				if( Key_Scan(KEY1) == KEY_ON  )
				{
					slash_range++;
					if(slash_range > 30)
						slash_range = 15;
				}
				if( Key_Scan(KEY2) == KEY_ON  )
				{
					slash_range--;
					if(slash_range < 15)
						slash_range = 30;
				}
				
				//���ҵ����ڽ�
				if( Key_Scan(KEY4) == KEY_ON  )	
				{
					slash_angle=slash_angle+5;
					if(slash_angle > 355)
						slash_angle = 0;
				}
				if( Key_Scan(KEY3) == KEY_ON  )
				{
					slash_angle=slash_angle-5;
					if(slash_angle < 0)
						slash_angle = 355;
				}
				
				
				
				if( Key_Scan(KEY6) == KEY_ON )
				{
					while(Key_Scan(KEY6) == KEY_ON);
					
					/*    б�����Ұڷ��ϳɼ���   ��ʼ */
					InputRange_fb=ROUND(slash_range*fabs(sin(slash_angle*3.1416/180.0)));
					InputRange_lr=ROUND(slash_range*fabs(cos(slash_angle*3.1416/180.0)));
					/*    б�����Ұڷ��ϳɼ���   ���� */
					
					if(slash_angle>180)
						slash_angle=slash_angle-180;
					
					
					if(slash_angle<5)					//0�ȸ������⴦��
					{
						slash_special=1;
						turn_leftORright(59999);
						delay_ms(300);
					}
					if(slash_angle>=5 && slash_angle<15)
					{
						 pid_slash_fb_Derivative=500000;
						 pid_slash_lr_Derivative=50000;
						error_range=5;
						turn_leftORright(59999);
						delay_ms(300);
					}
					else if(slash_angle<45)
					{
						pid_slash_fb_Derivative=500000;
						pid_slash_lr_Derivative=500000;
						error_range=4;
						turn_leftORright(59999);
						turn_frontORback(59999);						
						delay_ms(300);
					}
					else if(slash_angle<55)
					{
						pid_slash_fb_Derivative=50000;
						pid_slash_lr_Derivative=500000;
						error_range=4;
						turn_leftORright(49999);
						turn_frontORback(59999);						
						delay_ms(300);
					}
					else if(slash_angle<65)
					{
						pid_slash_fb_Derivative=50000;
						pid_slash_lr_Derivative=5000000;
						error_range=6;
						turn_leftORright(29999);
						turn_frontORback(59999);						
						delay_ms(300);
					}
					else if(slash_angle<85)
					{
						pid_slash_fb_Derivative=50000;
						pid_slash_lr_Derivative=5000000;
						error_range=6;
						turn_leftORright(19999);
						turn_frontORback(59999);						
						delay_ms(300);
					}
					else if(slash_angle<95)				//90�ȸ������⴦��
					{
						slash_special=2;
						turn_frontORback(59999);
						delay_ms(300);
					}
					else if(slash_angle<115)
					{
						pid_slash_fb_Derivative=50000;
						pid_slash_lr_Derivative=5000000;
						error_range=6;
						turn_leftORright(-19999);
						turn_frontORback(59999);						
						delay_ms(300);
					}
					else if(slash_angle<145)
					{
						pid_slash_fb_Derivative=500000;
						pid_slash_lr_Derivative=500000;
						error_range=6;
						turn_leftORright(-59999);
						turn_frontORback(39999);						
						delay_ms(300);
					}
					else if(slash_angle<175)
					{
						pid_slash_fb_Derivative=500000;
						pid_slash_lr_Derivative=50000;
						error_range=5;
						turn_leftORright(-59999);
						turn_frontORback( 29999);						
						delay_ms(300);
					}
					else if(slash_angle<=180)			//���⴦��
					{		
						slash_special=1;
					}
					
					break;
				}	
				if( Key_Scan(KEY5) == KEY_ON )
				{
					goto MainInterface;
				}
			}
			break;
		case 3:														//***������***
			break;
		
		case 4:														//***������***
		case 5:														//***������***
			while(1)
			{
				OledShowSecondInterface(choose_task_count+1,Radius,0);	
				delay_ms(70);
				//���µ����뾶
				if( Key_Scan(KEY1) == KEY_ON  )
				{
					Radius++;
					if(Radius > 35)
						Radius = 15;
				}
				if( Key_Scan(KEY2) == KEY_ON  )
				{
					Radius--;
					if(Radius < 15)
						Radius = 35;
				}
			
				if( Key_Scan(KEY6) == KEY_ON )
				{
					while(Key_Scan(KEY6) == KEY_ON);
					
					if(Radius < 17.5 )
					{
						pid_round_fb_Proportion=95;
						pid_round_lr_Proportion=95;
						pid_round_fb_Integral  =800000;
						pid_round_lr_Integral  =800000;
						pid_round_fb_Derivative=0;
						pid_round_lr_Derivative=0;
						error_range=70;
					}
					else if(Radius < 22.5 )
					{
						pid_round_fb_Proportion=90;
						pid_round_lr_Proportion=90;
						pid_round_fb_Integral  =850000;
						pid_round_lr_Integral  =850000;
						pid_round_fb_Derivative=0;
						pid_round_lr_Derivative=0;
						error_range=90;
					}
					else if(Radius < 27.5 )
					{
						pid_round_fb_Proportion=70;
						pid_round_lr_Proportion=70;
						pid_round_fb_Integral  =1200000;
						pid_round_lr_Integral  =1200000;
						pid_round_fb_Derivative=0;
						pid_round_lr_Derivative=0;
						error_range=120;
						
					}
					else if(Radius < 32.5 )
					{
						pid_round_fb_Proportion=50;
						pid_round_lr_Proportion=50;
						pid_round_fb_Integral  =1500000;
						pid_round_lr_Integral  =1500000;
						pid_round_fb_Derivative=0;
						pid_round_lr_Derivative=0;
						error_range=300;
					}
					else
					{
						pid_round_fb_Proportion=30;
						pid_round_lr_Proportion=30;
						pid_round_fb_Integral  =1500000;
						pid_round_lr_Integral  =1500000;
						pid_round_fb_Derivative=0;
						pid_round_lr_Derivative=0;
						error_range=400;
					}		
					turn_frontORback( 29999);	//ǿ�����					
					delay_ms(300);
					break;
				}
				if( Key_Scan(KEY5) == KEY_ON )
				{
					goto MainInterface;
				}
			}
			break;
		case 6:													//***������***		
			InputRange_fb=15;
			error_range=120;
			while(1)
			{
				//ˢ����ʾ
				OledShowSecondInterface(choose_task_count+1,InputRange_fb,0);
				delay_ms(70);
				if( Key_Scan(KEY1) == KEY_ON  )
				{
					InputRange_fb=25;
				}
				if( Key_Scan(KEY2) == KEY_ON  )
				{
					InputRange_fb=15;
				}
				
				if( Key_Scan(KEY6) == KEY_ON )
				{
					while(Key_Scan(KEY6) == KEY_ON);
					
					if(InputRange_fb == 15)
					{                                    //25_15    15-10
						pid_ellipse_fb_Proportion=250;     //400       250 
						pid_ellipse_lr_Proportion=200;      //190       200
						pid_ellipse_fb_Derivative=250;     //100        250
						pid_ellipse_lr_Derivative=250;       //100      250
						
						EllipseDuan=10;
						EllipseChang=15;
					}
					else
					{
						pid_ellipse_fb_Proportion=400;    
						pid_ellipse_lr_Proportion=200;     
						pid_ellipse_fb_Integral=800000;     
						pid_ellipse_lr_Integral=800000;     
						
						EllipseDuan=15;
						EllipseChang=25;
					}					
					turn_frontORback( 29999);	//ǿ�����					
					delay_ms(300);					
					break;
				}
				
				if( Key_Scan(KEY5) == KEY_ON )
				{
					goto MainInterface;
				}
			}
			break;	
	} 
	//*************************�������� ����*************************
		
	
	/************************
	*	���һ���PID���� ��ʼ*/	
	pidInit(&pid_lr_to_zero);		
	 pid_lr_to_zero.Proportion 		 = -100;
	 pid_lr_to_zero.Integral   		 = 0;
	 pid_lr_to_zero.Derivative		 = 4000;	
	pid_lr_to_zero.OutMax	  = 59999;
	pid_lr_to_zero.OutMin	  =-59999;
	pid_lr_to_zero.SumErrorMax= 40000;
	pid_lr_to_zero.SumErrorMin=-40000;		
	pid_lr_to_zero.SetPoint   = 0  ;		
	/* ���һ���PID���� ����  *
	*************************/
	
	/************************
	*	ǰ�����PID���� ��ʼ*/	
	pidInit(&pid_fb_to_zero);		
	 pid_fb_to_zero.Proportion		 = -100	;
	 pid_fb_to_zero.Integral  		 = 0	;
	 pid_fb_to_zero.Derivative		 = 4000	;	
	pid_fb_to_zero.OutMax	  = 59999;
	pid_fb_to_zero.OutMin	  =-59999;
	pid_fb_to_zero.SumErrorMax= 40000;
	pid_fb_to_zero.SumErrorMin=-40000;	
	pid_fb_to_zero.SetPoint   = 0  ;	
	/* ǰ�����PID���� ����  *
	*************************/
	
	/************************
	*	ǰ��ڷ�PID���� ��ʼ*/	
	pidInit(&pid_fb_range);		
	 pid_fb_range.Proportion 		= 100	;
	 pid_fb_range.Integral   		= 1		;
	 pid_fb_range.Derivative 		= 0		;	
	pid_fb_range.OutMax	    = 59999;
	pid_fb_range.OutMin	    =-59999;
	pid_fb_range.SumErrorMax= 10000;
	pid_fb_range.SumErrorMin=-10000;	
	pid_fb_range.SetPoint   = Angle_Data[InputRange_fb];
	/* ǰ��ڷ�PID���� ����  *
	*************************/
	
	/************************
	*	���Ұڷ�PID���� ��ʼ*/	
	pidInit(&pid_lr_range);		
	 pid_lr_range.Proportion 		= 100;
	 pid_lr_range.Integral   		= 1;
	 pid_lr_range.Derivative 		= 0;	
	pid_lr_range.OutMax	  = 59999;
	pid_lr_range.OutMin	  =-59999;
	pid_lr_range.SumErrorMax= 10000;
	pid_lr_range.SumErrorMin=-10000;	
	pid_lr_range.SetPoint   = Angle_Data[InputRange_lr];
	/* ���Ұڷ�PID���� ����  *
	*************************/
	
	/************************
	*	  б��PID���� ��ʼ  */	
	pidInit(&pid_slash_fb);	
	 pid_slash_fb.Proportion 		= 0	;
	 pid_slash_fb.Integral   		= 0		;
	 pid_slash_fb.Derivative 		= pid_slash_fb_Derivative 	;	//80 50000 //70  50000   //60  50000  //50    50000   //40    500000  //30  500000  //20   500000  //10  500000��Ҫ���
													//100 50000  //110 50000 //120 500000  //130 500000 //140 500000    //150 500000�����	//160 500000 //170 500000��Ҫ���	 
	pid_slash_fb.OutMax	  = 59999;
	pid_slash_fb.OutMin	  =-59999;
	pid_slash_fb.SumErrorMax= 100;
	pid_slash_fb.SumErrorMin=-100;	
 	pid_slash_fb.SetPoint   = 0;  	
	
	pidInit(&pid_slash_lr);	
	 pid_slash_lr.Proportion 		= 0 	;
	 pid_slash_lr.Integral   		= 0		;
	 pid_slash_lr.Derivative 		= pid_slash_lr_Derivative 	;	//80  5000000 //70  5000000  //60  5000000 //50 500000 //40  500000    //30  500000  //20   500000  //10   50000��Ҫ���
													//100 5000000  //110 5000000 //120 500000  //130 500000 //140 500000   //150 50000�����  //160 50000 //170  50000��Ҫ���
	pid_slash_lr.OutMax	  = 59999;
	pid_slash_lr.OutMin	  =-59999;
	pid_slash_lr.SumErrorMax= 100;
	pid_slash_lr.SumErrorMin=-100;	
	pid_slash_lr.SetPoint   = 0; 
	/*    б��PID����  ����  *
	*************************/
	
	
	/************************
	*	  ��ԲPID���� ��ʼ  */	
	pidInit(&pid_round_fb);	
	 pid_round_fb.Proportion 		= pid_round_fb_Proportion	; 	//15CM 250  20CM  25CM 150  30CM   35CM  100
	 pid_round_fb.Integral   		= pid_round_fb_Integral	;
	 pid_round_fb.Derivative 		= pid_round_fb_Derivative;//15CM 250  20cm   25CM 1000  30CM    35CM 	
	pid_round_fb.OutMax	  = 59999;
	pid_round_fb.OutMin	  =-59999;
	pid_round_fb.SumErrorMax= 0.009;
	pid_round_fb.SumErrorMin=-0.009;	
 	pid_round_fb.SetPoint   = 0;  	
	
	pidInit(&pid_round_lr);	
	 pid_round_lr.Proportion 		= pid_round_lr_Proportion 	;
	 pid_round_lr.Integral   		= pid_round_lr_Integral		;
	 pid_round_lr.Derivative 		= pid_round_lr_Derivative	;	
	pid_round_lr.OutMax	  = 59999;
	pid_round_lr.OutMin	  =-59999;
	pid_round_lr.SumErrorMax= 0.009;
	pid_round_lr.SumErrorMin=-0.009;	
	pid_round_lr.SetPoint   = 0; 
	/*    ��ԲPID����  ����  *
	*************************/
	
	
	/************************
	*	  ����ԲPID���� ��ʼ  */	
	pidInit(&pid_ellipse_fb);	
	 pid_ellipse_fb.Proportion 		= pid_ellipse_fb_Proportion	; 	//15CM 250  20CM  25CM 150  30CM   35CM  100
	 pid_ellipse_fb.Integral   		= 0	;
	 pid_ellipse_fb.Derivative 		= pid_ellipse_fb_Derivative;//15CM 250  20cm   25CM 1000  30CM    35CM 	
	pid_ellipse_fb.OutMax	  = 59999;
	pid_ellipse_fb.OutMin	  =-59999;
	pid_ellipse_fb.SumErrorMax= 40;
	pid_ellipse_fb.SumErrorMin=-40;	
 	pid_ellipse_fb.SetPoint   = 0;  	
	
	pidInit(&pid_ellipse_lr);	
	 pid_ellipse_lr.Proportion 		= pid_ellipse_lr_Proportion 	;
	 pid_ellipse_lr.Integral   		= 0		;
	 pid_ellipse_lr.Derivative 		= pid_ellipse_lr_Derivative	;	
	pid_ellipse_lr.OutMax	  = 59999;
	pid_ellipse_lr.OutMin	  =-59999;
	pid_ellipse_lr.SumErrorMax= 40;
	pid_ellipse_lr.SumErrorMin=-40;	
	pid_ellipse_lr.SetPoint   = 0; 
	/*    ����ԲPID����  ����  *
	*************************/
	
	

//	delay_ms(2000);						//���
//	turn_frontORback(59999);			
//	delay_ms(600);
//	turn_frontORback(0);
//	
//	turn_frontORback(64851.0*cos(slash_angle*3.1416/180));
//	turn_leftORright(64851.0*-sin(slash_angle*3.1416/180));
//	delay_ms(600);
//	turn_frontORback(0);
//	turn_leftORright(0);
	
	OledShowWorking();
	while(1)
	{	
		if(flag==1)
		{
			delta_PITCH=PITCH - LAST_PITCH;
			delta_ROLL =ROLL  - LAST_ROLL;
			
			if(choose_task_count <=1 )																						//*********   ����һ ���� *********
			{
				//*********** ����ǰ��һ���ķ��� ��ʼ ***********//
				if( ( delta_PITCH >= 0  && last_delta_PITCH < 0 )||( delta_PITCH > 0  && last_delta_PITCH == 0 )) 
				{
					PresentAngleRange_fb=fabs(PITCH);
					RangeOut_fb=pidCalc(&pid_fb_range,PresentAngleRange_fb);		
					RangeOut_fb = OutputAdjustment(RangeOut_fb);
					
					//************* ����Ƿ��� ��ʼ*************//	
					if(fabs(  83.5*tan(PresentAngleRange_fb*3.1416/18000.0) -  InputRange_fb )  < 2.5   )
						swing_ready_count++;				
					else
						swing_ready_count=0;
					if(swing_ready_count > 1)
						BazzBeee();
					else
						BazzStop();
					//************  ����Ƿ��� ���� ************//
					
					
				}
				StartWingFrontOrBack(RangeOut_fb);
				//*********** ����ǰ��һ���ķ��� ���� ***********//

				//************* ���һ������ ��ʼ*************//	
				RangeOut_lr=pidCalc(&pid_lr_to_zero,ROLL);
//				if(fabs(delta_ROLL)< 8 && fabs(ROLL) <170)
//					phshu++;
//				else 
//					phshu=0;			
//				if(phshu > 10)
//					RangeOut_lr=0;	
				RangeOut_lr=OutputAdjustment(RangeOut_lr);
				turn_leftORright(RangeOut_lr);
				//************  ���һ������ ���� ************//
				
			}
								

			
			if(choose_task_count == 2)																					//*********   ������  *********
			{
				if(slash_special == 0)	  																	//һ��б��
				{
					//*********** ����ǰ��һ���ķ��� ��ʼ ***********//
					if( ( delta_PITCH >= 0  && last_delta_PITCH < 0 )||( delta_PITCH > 0  && last_delta_PITCH == 0 )) 
					{
						PresentAngleRange_fb=fabs(PITCH);
						RangeOut_fb=pidCalc(&pid_fb_range,PresentAngleRange_fb);		
						RangeOut_fb = OutputAdjustment(RangeOut_fb);
					}
					if(Divisiontime%2==0)
						StartWingFrontOrBack(RangeOut_fb);
					//*********** ����ǰ��һ���ķ��� ���� ***********//
					
					
					//*********** ��������һ���ķ��� ��ʼ ***********//
					if(( delta_ROLL >= 0  && last_delta_ROLL < 0 )||( delta_ROLL > 0  && last_delta_ROLL == 0 ))
					{
						PresentAngleRange_lr=fabs(ROLL);
						RangeOut_lr=pidCalc(&pid_lr_range,PresentAngleRange_lr);		
						RangeOut_lr = OutputAdjustment(RangeOut_lr);
					}
					if(Divisiontime%2==0)
						StartWingLeftOrRight(RangeOut_lr);	
					//*********** ��������һ���ķ��� ���� ***********//
					
					//********  ��һ���ǶȰڶ�  *********			
					pid_slash_lr.SetPoint   =  83.5*tan(PITCH*3.1416/18000.0) / tan(slash_angle*3.1416/180.0);
					SlashLrPidIn = 83.5*tan(ROLL *3.1416/18000.0);
					SlashOutLr=pidCalc(&pid_slash_lr,SlashLrPidIn );					
					SlashOutLr=OutputAdjustment(SlashOutLr);
					if(Divisiontime%2==1)
						turn_leftORright(SlashOutLr);
					
					pid_slash_fb.SetPoint   = 83.5*tan(ROLL *3.1416/18000.0) * tan(slash_angle*3.1416/180.0);
					SlashFbPidIn = 83.5*tan(PITCH *3.1416/18000.0);
					SlashOutFb=pidCalc(&pid_slash_fb, SlashFbPidIn );					
					SlashOutFb=OutputAdjustment(SlashOutFb);
					if(Divisiontime%2==1)
						turn_frontORback(SlashOutFb);
					//********��һ���ǶȰڶ� ����*********
					
					//************* ����Ƿ��� ��ʼ*************//	
					if(  ((fabs(pid_slash_lr.SetPoint - SlashLrPidIn) < error_range) && (fabs(pid_slash_fb.SetPoint - SlashFbPidIn) < error_range) )      &&      ( (fabs(  83.5*tan(PresentAngleRange_fb*3.1416/18000.0) -  InputRange_fb )  < error_range )  && (fabs(  83.5*tan(PresentAngleRange_fb*3.1416/18000.0) -  InputRange_fb )  < error_range ))  )
						swing_ready_count++;
					else
						swing_ready_count=0;
					if(swing_ready_count > 50)
						BazzBeee();
					else
						BazzStop();
					//************* ����Ƿ��� ����*************//	
				}
				else if(slash_special == 1)  																//0��б��
				{
					//*********** ��������һ���ķ��� ��ʼ ***********//
					if(( delta_ROLL >= 0  && last_delta_ROLL < 0 )||( delta_ROLL > 0  && last_delta_ROLL == 0 ))
					{
						PresentAngleRange_lr=fabs(ROLL);
						RangeOut_lr=pidCalc(&pid_lr_range,PresentAngleRange_lr);		
						RangeOut_lr = OutputAdjustment(RangeOut_lr);
						
						//************* ����Ƿ��� ��ʼ*************//	
						if(fabs(  83.5*tan(PresentAngleRange_lr*3.1416/18000.0) -  InputRange_lr )  < 3.5   )
							swing_ready_count++;				
						else
							swing_ready_count=0;
						if(swing_ready_count > 1)
							BazzBeee();
						else
							BazzStop();
						//************  ����Ƿ��� ���� ************//
					}
					StartWingLeftOrRight(RangeOut_lr);	
					//*********** ��������һ���ķ��� ���� ***********//
					
					//************* ǰ�����һ������ ��ʼ*************//	
					RangeOut_fb=pidCalc(&pid_fb_to_zero,PITCH);
//					if(fabs(delta_ROLL)< 8 && fabs(ROLL) <170)
//						phshu++;
//					else 
//						phshu=0;			
//					if(phshu > 10)
//						RangeOut_lr=0;	
					RangeOut_fb=OutputAdjustment(RangeOut_fb);
					turn_frontORback(RangeOut_fb);
					//************  ǰ�����һ������ ���� ************//
				}
				else  																							//90��б��
				{
					//*********** ����ǰ��һ���ķ��� ��ʼ ***********//
					if( ( delta_PITCH >= 0  && last_delta_PITCH < 0 )||( delta_PITCH > 0  && last_delta_PITCH == 0 )) 
					{
						PresentAngleRange_fb=fabs(PITCH);
						RangeOut_fb=pidCalc(&pid_fb_range,PresentAngleRange_fb);		
						RangeOut_fb = OutputAdjustment(RangeOut_fb);
						
						//************* ����Ƿ��� ��ʼ*************//	
						if(fabs(  83.5*tan(PresentAngleRange_fb*3.1416/18000.0) -  InputRange_fb )  < 3.5   )
							swing_ready_count++;				
						else
							swing_ready_count=0;
						if(swing_ready_count > 1)
							BazzBeee();
						else
							BazzStop();
						//************  ����Ƿ��� ���� ************//
					}
					StartWingFrontOrBack(RangeOut_fb);
					//*********** ����ǰ��һ���ķ��� ���� ***********//

					//************* ���һ������ ��ʼ*************//	
					RangeOut_lr=pidCalc(&pid_lr_to_zero,ROLL);
//					if(fabs(delta_ROLL)< 8 && fabs(ROLL) <170)
//						phshu++;
//					else 
//						phshu=0;			
//					if(phshu > 10)
//						RangeOut_lr=0;	
					RangeOut_lr=OutputAdjustment(RangeOut_lr);
					turn_leftORright(RangeOut_lr);
					//************  ���һ������ ���� ************//
				}
			}
			
			if(choose_task_count == 3)																					//*********   ������ *********
			{
				//************* ǰ�����һ������ ��ʼ*************//	
				RangeOut_lr=pidCalc(&pid_lr_to_zero,ROLL);
				RangeOut_fb=pidCalc(&pid_fb_to_zero,PITCH);
				if(fabs(delta_ROLL)< 8 && fabs(ROLL) <170)
					phshu++;
				else 
					phshu=0;			
				if(phshu > 10)
					RangeOut_lr=0;	
//				RangeOut_lr=OutputAdjustment(RangeOut_lr);
//				RangeOut_fb=OutputAdjustment(RangeOut_fb);
				turn_leftORright(RangeOut_lr);
				turn_frontORback(RangeOut_fb);
				//************  ǰ�����һ������ ���� ************//
				
				//************* ����Ƿ��� ��ʼ*************//	
				if(( fabs ( 83.5*tan(PITCH*3.1416/18000.0) ) < 2.5 ) && ( fabs ( 83.5*tan(ROLL*3.1416/18000.0) ) < 2.5 ) )
					swing_ready_count++;				
				else
					swing_ready_count=0;
				if(swing_ready_count > 100)
					BazzBeee();
				else
					BazzStop();
				//************  ����Ƿ��� ���� ************//
			}
			
			if(choose_task_count == 4  ||  choose_task_count == 5 )																	//*********   �����塢��  *********
			{
				//********  ��Բ  *********
				if(delta_PITCH*delta_ROLL != 0)
				{
					pid_round_lr.SetPoint   =    delta_PITCH/fabs(delta_PITCH)  *   (   Radius*Radius   -    83.5*tan(PITCH*3.1416/18000.0) * 83.5*tan(PITCH*3.1416/18000.0)   ) ;
					RoundLrPidIn=delta_PITCH/fabs(delta_PITCH)  *    (83.5*tan(ROLL *3.1416/18000.0)) * (83.5*tan(ROLL *3.1416/18000.0));
					RoundOutLr=pidCalc(&pid_round_lr ,  RoundLrPidIn) ;
					RoundOutLr=OutputAdjustment(RoundOutLr);
					turn_leftORright(RoundOutLr);
					
					pid_round_fb.SetPoint   =   -delta_ROLL/fabs(delta_ROLL)   *   (  Radius*Radius   -   83.5*tan(ROLL *3.1416/18000.0) * 83.5*tan(ROLL *3.1416/18000.0)  );
					RoundFbPidIn=-delta_ROLL/fabs(delta_ROLL)   *   (83.5*tan(PITCH *3.1416/18000.0)) * (83.5*tan(PITCH *3.1416/18000.0));
					RoundOutFb=pidCalc(&pid_round_fb,    RoundFbPidIn   );
					RoundOutFb=OutputAdjustment(RoundOutFb);
					turn_frontORback(RoundOutFb);
				}
				
				//************* ����Ƿ��� ��ʼ*************//	
				if( ( fabs( RoundLrPidIn - pid_round_lr.SetPoint ) < error_range )  &&  ( fabs( pid_round_fb.SetPoint  -  RoundFbPidIn  )  < error_range   )  )
					swing_ready_count++;				
				else
					swing_ready_count=0;
				if(swing_ready_count > 150)
					BazzBeee();
				else
					BazzStop();
				//************  ����Ƿ��� ���� ************//
				
				//********  ��Բ  ���� *********
			}
			
			if(choose_task_count == 6 )																	//*********   ������  *********
			{
				//********  ��Բ  *********
				if(delta_PITCH*delta_ROLL != 0)
				{
					pid_ellipse_lr.SetPoint   =    delta_PITCH/fabs(delta_PITCH)  *   (   EllipseChang*EllipseChang   -    83.5*tan(PITCH*3.1416/18000.0) * 83.5*tan(PITCH*3.1416/18000.0)*EllipseChang*EllipseChang/EllipseDuan/EllipseDuan   ) ;
					EllipseLrPidIn=delta_PITCH/fabs(delta_PITCH)  *    (83.5*tan(ROLL *3.1416/18000.0)) * (83.5*tan(ROLL *3.1416/18000.0));
					EllipseOutLr=pidCalc(&pid_ellipse_lr ,  EllipseLrPidIn) ;
					EllipseOutLr=OutputAdjustment(EllipseOutLr);
						turn_leftORright(EllipseOutLr);
					
					pid_ellipse_fb.SetPoint   =   -delta_ROLL/fabs(delta_ROLL)   *   (  EllipseDuan*EllipseDuan   -   83.5*tan(ROLL *3.1416/18000.0) * 83.5*tan(ROLL *3.1416/18000.0)*EllipseDuan*EllipseDuan/EllipseChang/EllipseChang  );
					EllipseFbPidIn=-delta_ROLL/fabs(delta_ROLL)   *   (83.5*tan(PITCH *3.1416/18000.0)) * (83.5*tan(PITCH *3.1416/18000.0));
					EllipseOutFb=pidCalc(&pid_ellipse_fb,    EllipseFbPidIn   );
					EllipseOutFb=OutputAdjustment(EllipseOutFb);
						turn_frontORback(EllipseOutFb);
				}
				//******** ��Բ  ���� *********
				
				
				//************* ����Ƿ��� ��ʼ*************//	
				if( ( fabs( EllipseLrPidIn - pid_ellipse_lr.SetPoint ) < error_range )  &&  ( fabs( pid_ellipse_fb.SetPoint  -  EllipseFbPidIn  )  < error_range   )  )
					swing_ready_count++;				
				else
					swing_ready_count=0;
				if(swing_ready_count > 100)
					BazzBeee();
				else
					BazzStop();
				//************  ����Ƿ��� ���� ************//
			}
			
			//			if(choose_task_count < 6  &&  choose_task_count != 3)
//			{
//				//*********** ����ǰ��һ���ķ��� ��ʼ ***********//
//				if( ( delta_PITCH >= 0  && last_delta_PITCH < 0 )||( delta_PITCH > 0  && last_delta_PITCH == 0 )) 
//				{
//					PresentAngleRange_fb=fabs(PITCH);
//					RangeOut_fb=pidCalc(&pid_fb_range,PresentAngleRange_fb);		
//					RangeOut_fb = OutputAdjustment(RangeOut_fb);
//				}
//				if(Divisiontime%2==0)
//					StartWingFrontOrBack(RangeOut_fb);
//				//*********** ����ǰ��һ���ķ��� ���� ***********//
//				
//				
//				//*********** ��������һ���ķ��� ��ʼ ***********//
//				if(( delta_ROLL >= 0  && last_delta_ROLL < 0 )||( delta_ROLL > 0  && last_delta_ROLL == 0 ))
//				{
//					PresentAngleRange_lr=fabs(ROLL);
//					RangeOut_lr=pidCalc(&pid_lr_range,PresentAngleRange_lr);		
//					RangeOut_lr = OutputAdjustment(RangeOut_lr);
//				}
//				if(Divisiontime%2==0)
//					StartWingLeftOrRight(RangeOut_lr);	
//				//*********** ��������һ���ķ��� ���� ***********//
//			}
			
			//*****   ���ݸ���   *****
			LAST_PITCH=PITCH;
			LAST_ROLL=ROLL;			

			last_delta_PITCH=delta_PITCH;
			last_delta_ROLL=delta_ROLL;
			
			Divisiontime++;
			
			DataOutput();
			flag=0;
			//***** ���ݸ��� ����*****			
		}
		if( Key_Scan(KEY5) == KEY_ON  )
			break;
	}
	turn_frontORback(0);
	turn_leftORright(0);
	goto MainInterface;
 }

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 //*************************************************************************************************************
//************************************************����Ϊ�Ӻ���***************************************************
 

/*******************************
* �������
********************************/
void DataOutput()
{
//	USART_SendData(USART1,'r');
//	USART_SendData(USART1,':');
	dis_play(ROLL,0);
	USART_SendData(USART1,' ');
	
//	USART_SendData(USART1,'p');
//	USART_SendData(USART1,':');
	dis_play(PITCH,0);
	USART_SendData(USART1,' ');
	
//	USART_SendData(USART1,'L');
//	USART_SendData(USART1,':');
//	dis_play_out(RangeOut_lr,0);
//	USART_SendData(USART1,' ');
//	
//	USART_SendData(USART1,'F');
//	USART_SendData(USART1,':');
//	dis_play_out(RangeOut_fb,0);
//	USART_SendData(USART1,' ');
//	
//	USART_SendData(USART1,'d');
//	USART_SendData(USART1,':');
//	dis_play(delta_ROLL,0);
	USART_SendData(USART1,0X0d);
	USART_SendData(USART1,0X0a);
}
 
 
void mpu_6050_usart_init(void) 
{
	USART_SendData(USART1,0xa5);
	USART_SendData(USART1,0x54);//У��
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
//	delay_ms(1500);
//	delay_ms(500);

	USART_SendData(USART1,0xa5);
	USART_SendData(USART1,0x52);//�Զ�ģʽ
}
void dis_play(int16_t num,u8 n)	
{
	if(num<0)
	{
		num=-num;
		USART_SendData(USART1,'-');
	}
	else
		USART_SendData(USART1,'+');
	//if(n)
	USART_SendData(USART1,0x30|(num/1000000));	
	USART_SendData(USART1,0x30|(num%1000000/100000));
	USART_SendData(USART1,0x30|(num%100000/10000));	
	USART_SendData(USART1,0x30|(num%10000/1000));
	USART_SendData(USART1,0x30|(num%1000/100));
	USART_SendData(USART1,0x2e);
	USART_SendData(USART1,0x30|(num%100/10));
	USART_SendData(USART1,0x30|(num%10));
//	USART_SendData(USART1,',');
}

void dis_play_out(long int num,u8 n)	
{
	if(num<0)
	{
		num=-num;
		USART_SendData(USART1,'-');
	}
	else
		USART_SendData(USART1,'+');
	
	USART_SendData(USART1,0x30|(num/10000));	
	USART_SendData(USART1,0x30|(num%10000/1000));
	USART_SendData(USART1,0x30|(num%1000/100));
	USART_SendData(USART1,0x30|(num%100/10));
	USART_SendData(USART1,0x30|(num%10));
	USART_SendData(USART1,',');
}

/******************************
* �����ƶ�����
* ���ratio����0����ǰ��
* С��0�������
******************************/
void turn_frontORback(long int ratio)
{
	if(ratio > 0)
	{
		TIM_SetCompare1(TIM3 , 0 );
		TIM_SetCompare2(TIM3 , ratio );
	}
	else
	{
		TIM_SetCompare1(TIM3 , (-ratio) );
		TIM_SetCompare2(TIM3 , 0 );
	}
}

/*******************************
* �����ƶ�����
* ���ratio����0��������
* С��0������
********************************/
void turn_leftORright(long int ratio)
{
	if(ratio > 0)
	{
		TIM_SetCompare3(TIM3 , 0 );
		TIM_SetCompare4(TIM3 , ratio );
	}
	else
	{
		TIM_SetCompare3(TIM3 , (-ratio) );
		TIM_SetCompare4(TIM3 , 0 );
	}
}

/*******************************
* ǰ����ڳ��򣬴����������С
* ����Ϊ������ֹ�ڶ�
********************************/
void StartWingFrontOrBack(double wind)
{
	if(delta_PITCH > 0)
		turn_frontORback(wind);
	else
		turn_frontORback(-wind);

}


/*******************************
* ������ڳ��򣬴����������С
* ����Ϊ������ֹ�ڶ�
********************************/
void StartWingLeftOrRight(double wind)
{
	if(delta_ROLL > 0)
		turn_leftORright(wind);
	else
		turn_leftORright(-wind);
}


/**************************************
* Ϊ�������������Ӱ�죬��ռ�ձ����������
***************************************/
double OutputAdjustment(double x)
{
	if( x > 0 )
		return (x*0.8+12000);
	else
		return (x*0.8-12000);
}

