/*
		λ��ʽPID
*/

#include "PID.h"
#include "Function.h"

#define T 0.01f   //����ʱ��
uint16_t r=30;  //���ַ�����ֵ
float IE=1,DE=1;//�Ƿ���ֻ�΢��

#define Coefficient 0.5f  //���ϵ��


//********************************************************************************************************
//																					X����
//********************************************************************************************************

float X_P=150.0f,X_I=0.9f,X_D=0.55f;      //PID����  


float Xe=0.0f,Xe_1=0.0f;  //ƫ�����£�  Xe:��ǰƫ���e(k)  Xe_1����һ��ƫ��e(k-1)  Xe:����һ�ε�ƫ��e(k-2)
float XIerror=0.0f;

float X_PID(float X_Rlt, float X_E)
{
	float N_Reality=0,N_Exp=0;//ʵ��ֵ������ֵ
	float XU_Out=0;//�������
	float K_P=0,K_I=0,K_D=0;//PID����
	uint8_t f=0;//���ַ������
	
	N_Reality=X_Rlt;
	N_Exp=X_E;
	//���㵱ǰƫ��
	Xe=N_Exp-N_Reality;//������-��ǰ��
	//���ַ���
	if(Xe>r||Xe<-r)f=0;
	else f=1;
	//�ֱ����P I D
	K_P=X_P*Xe;
	
	XIerror+=Xe;//���»���ƫ��
	if(XIerror>1000)XIerror=1000;//�����޷�
	else if(XIerror<-1000)XIerror=-1000;
	K_I=(X_P*T/X_I)*XIerror;
	
	K_D=(X_P*X_D/T)*(Xe-Xe_1);
	
	//����λ��U
	XU_Out=K_P +K_I*f *IE +K_D;

	//����ƫ��
	Xe_1=Xe;

	return XU_Out;	
}
//********************************************************************************************************
//																					Y����
//********************************************************************************************************

float Y_P=150.0f,Y_I=0.85f,Y_D=0.55f;      //PID����  


float Ye=0.0f,Ye_1=0.0f;  //ƫ�����£�  Ye:��ǰƫ���e(k)  Ye_1����һ��ƫ��e(k-1)  Ye:����һ�ε�ƫ��e(k-2)
float YIerror=0.0f;

float Y_PID(float Y_Rlt, float Y_E)
{
	float N_Reality=0,N_Exp=0;//ʵ��ֵ������ֵ
	float YU_Out=0;//�������
	float K_P=0,K_I=0,K_D=0;//PID����
	uint8_t f=0;//���ַ������
	
	N_Reality=Y_Rlt;
	N_Exp=Y_E;
	//���㵱ǰƫ��
	Ye=N_Exp-N_Reality;//������-��ǰ��
	//���ַ���
	if(Ye>r||Xe<-r)f=0;
	else f=1;
	//�ֱ����P I D
	K_P=Y_P*Ye;
	
	YIerror+=Ye;//���»���ƫ��
	if(YIerror>1000)YIerror=1000;//�����޷�
	if(YIerror<-1000)YIerror=-1000;
	K_I=(Y_P*T/Y_I)*YIerror;
	
	K_D=(Y_P*Y_D/T)*(Ye-Ye_1);
	
	//����λ��U
	YU_Out=K_P +K_I*f *IE +K_D;

	//����ƫ��
	Ye_1=Ye;
	

	return YU_Out;	
}

void moto(float XU_Out, float YU_Out )
{
	uint16_t speed=0;
	
		
	//�����
	if(XU_Out>=0)
	{
		speed=(uint16_t)(Coefficient*XU_Out);
		Set_MotoSpeed(speed+2000,3);
		Set_MotoSpeed(0,1);
	}
	else
	{
		speed=(uint16_t)(Coefficient*(-XU_Out));
		Set_MotoSpeed(speed+2000,1);
		Set_MotoSpeed(0,3);
	}
//	printf("%d  ",speed);
	
	
		//�����
	if(YU_Out>=0)
	{
		speed=(uint16_t)(Coefficient*YU_Out);
		Set_MotoSpeed(speed+2000,2);
		Set_MotoSpeed(0,4);
	}
	else
	{
		speed=(uint16_t)(Coefficient*(-YU_Out));
		Set_MotoSpeed(speed+2000,4);
		Set_MotoSpeed(0,2);
	}
//	printf("%d\n",speed);
	
}

void SetPID(uint8_t tasknum)
{
	switch(tasknum)
	{
		case 1:{
							IE=1;
							X_P=150.0f; X_I=0.9f; X_D=0.55f;
							Y_P=150.0f; Y_I=0.85f; Y_D=0.55f;
							break;
						}
		case 2:{
							break;
						}
		case 3:{
							break;
						}
		case 4:{
							IE=0;
							X_P=80.0f; X_I=9.0f; X_D=0.5f;
							Y_P=50.0f; Y_I=9.0f; Y_D=0.55f;
							break;
						}
	}
}


