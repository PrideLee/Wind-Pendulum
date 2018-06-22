#include "MPU6050.h"
#include "myiic.h"
#include "delay.h"
#include "LED.h"
#include "usart.h"

struct MPU6050 MPU_Offset={0,0,0,0,0,0,0,0};

uint8_t MPU6050_Init(void)
{
	uint8_t reg=0;
	IIC_Init();
	
	reg=read_byte(MPU_DEVICE_ID_REG);
	if(reg!=0x68)
	{
		printf("reg: %d\n",reg);
		printf("read error\n");
		LED1=0;
		return 0;
	}
	
	write_byte(MPU_PWR_MGMT1_REG,0x00);//��Դ����Ĵ���1  0x6B

	write_byte(MPU_SAMPLE_RATE_REG,0x07);//����Ƶ�ʷ�Ƶ��   0x19

	write_byte(MPU_CFG_REG,0x06);//���üĴ���   0X1A

	write_byte(MPU_GYRO_CFG_REG,0x01);//���������üĴ���  0X1B   //Ϊʲô�� 0x01 

	write_byte(MPU_ACCEL_CFG_REG,0x00);//���ٶȼ����üĴ���  0X1C

	MPU_Set_Gyro_Fsr(3);//+-2000��/s  ���Լ�              //
	MPU_Set_Accel_Fsr(1);//+-4g  ���Լ죬���˲�          //
	MPU_Set_Rate(50);//������ʣ�50Hz                    //������ 100Hz

	write_byte(MPU_PWR_MGMT1_REG,0X01);//��Դ����Ĵ���1  0x6B
	write_byte(MPU_PWR_MGMT2_REG,0X00);//��Դ����Ĵ���2  0x6C
	
	reg=read_byte(MPU_PWR_MGMT1_REG);
	if(reg!=0x01)
	{
		printf("reg: %d\n",reg);
		printf("write error\n");
		LED1=0;
		return 0;
	}
	return 1;
}



uint8_t ReadMpu(struct MPU6050 *mpu)
{
	if(R_MPU_Original(mpu))
	{
		disposedata(mpu);
		return 1;
	}
	else return 0;
}
void disposedata(struct MPU6050 *mpu)
{
	
	mpu->Ax=mpu->Ax-MPU_Offset.Ax;//����ƫ��
	mpu->Ay=mpu->Ay-MPU_Offset.Ay;
	mpu->Az=mpu->Az-MPU_Offset.Az+8192;
	
	mpu->Gx=mpu->Gx-MPU_Offset.Gx;//����ƫ��
	mpu->Gy=mpu->Gy-MPU_Offset.Gy;
	mpu->Gz=mpu->Gz-MPU_Offset.Gz;
	
	
	
}

uint8_t R_MPU_Original(struct MPU6050 *mpu)
{
	uint8_t flag=0;
//	float temp=0;
	
	flag=MPU_Read_Len(MPU_ACCEL_XOUTH_REG,14,mpu->buf);
	
	if(flag)
	{
		return 0;
	}
		
	mpu->Ax=(int16_t)((mpu->buf[0]<<8)|mpu->buf[1]);
	mpu->Ay=(int16_t)((mpu->buf[2]<<8)|mpu->buf[3]);
	mpu->Az=(int16_t)((mpu->buf[4]<<8)|mpu->buf[5]);
	
	mpu->Gx=(int16_t)((mpu->buf[ 8]<<8)|mpu->buf[ 9]);
	mpu->Gy=(int16_t)((mpu->buf[10]<<8)|mpu->buf[11]);
	mpu->Gz=(int16_t)((mpu->buf[12]<<8)|mpu->buf[13]);
	
//	temp=((mpu->buf[6]<<8)|mpu->buf[7]);
//	mpu->Temprt=((temp-616.0f)/333.87f)+21.0f; 
	
//	mpu->Ax/=8192.0f;//  
//	mpu->Ay/=8192.0f;//
//	mpu->Az/=8192.0f;//
	
	mpu->Gx/=16.4f;//  ��/s
	mpu->Gy/=16.4f;
	mpu->Gz/=16.4f;
	
		return 1;
}
	


/*
 * ���ܣ���MPU6050дһ�ֽ�����
 * ���룺REG_Address ��ַ
         REG_data    Ҫд�������
 * ������ɹ��� 0
         ʧ�ܻ� 1
*/
uint8_t write_byte(uint8_t REG_Address,uint8_t REG_data)//��MPU6050дһ�ֽ�����

{

 IIC_Start(); 
	IIC_Send_Byte((MPU_SlaveAddress<<1)|0);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();	LED2=0;
		return 1;		
	}
    IIC_Send_Byte(REG_Address);	
    IIC_Wait_Ack();		
	IIC_Send_Byte(REG_data);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;

}

/*
 * ���ܣ� ��MPU6050һ�ֽ�����
 * ���룺 REG_Address ���ݵ�ַ 
 * ����� �ɹ��� ����
          ʧ��
*/
uint8_t read_byte(uint8_t REG_Address)//       ��MPU6050һ�ֽ�����

{

u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MPU_SlaveAddress<<1)|0);
	IIC_Wait_Ack();		
    IIC_Send_Byte(REG_Address);	
    IIC_Wait_Ack();		
    IIC_Start();
	IIC_Send_Byte((MPU_SlaveAddress<<1)|1);
    IIC_Wait_Ack();		
	res=IIC_Read_Byte(0);//�����ֲ��ϵ���0  res=IIC_Read_Byte(0);//
    IIC_Stop();			
	return res;	

}


u8 MPU_Read_Len(u8 reg,u8 len,uint8_t *buf)//������
{
 	IIC_Start(); 
	IIC_Send_Byte((MPU_SlaveAddress<<1)|0);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	
    IIC_Wait_Ack();		
    IIC_Start();
	IIC_Send_Byte((MPU_SlaveAddress<<1)|1);
    IIC_Wait_Ack();
	while(len)	
	{
		if(len==1)*buf=IIC_Read_Byte(0);
		else *buf=IIC_Read_Byte(1);		
		len--;
		buf++; 
	}
    IIC_Stop();	//

	return 0;	
}







/***********************************************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return write_byte(MPU_CFG_REG,data);//
}
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=write_byte(MPU_SAMPLE_RATE_REG,data);	//����Ƶ�ʷ�Ƶ��  0X19     
 	return MPU_Set_LPF(rate/2);	//
}
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return write_byte(MPU_ACCEL_CFG_REG,fsr<<3);////���ٶȼ����üĴ���  0X1C
}
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return write_byte(MPU_GYRO_CFG_REG,fsr<<3);////���������üĴ���  0X1B
	
}


/* ������ԭʼ������� */
void USART_Senser_Data_output(struct MPU6050 *mpu, uint8_t a)
{
  uint8_t q=0x03,p=0xfc,H,L;
	
		if(a)
	{
   printf_1_(&q,1);printf_1_(&p,1);//֡ͷ
	
	 H=(uint8_t)((int16_t)(mpu->Ax*100)>>8); 
   L=(uint8_t)(int16_t)(mpu->Ax*100);
	 printf_1_(&L,1);printf_1_(&H,1);//����1
	
	 H=(uint8_t)((int16_t)(mpu->Ay*100)>>8); 
   L=(uint8_t)(int16_t)(mpu->Ay*100);
	 printf_1_(&L,1);printf_1_(&H,1);//����2
	
	 H=(uint8_t)((int16_t)(mpu->Az*100)>>8); 
   L=(uint8_t)(int16_t)(mpu->Az*100);
	 printf_1_(&L,1);printf_1_(&H,1);//����3
	
	 H=(uint8_t)((int16_t)(mpu->Gx)>>8); 
   L=(uint8_t)(int16_t)(mpu->Gx);
	 printf_1_(&L,1);printf_1_(&H,1);//����4
	
	 H=(uint8_t)((int16_t)(mpu->Gy)>>8); 
   L=(uint8_t)(int16_t)(mpu->Gy);
	 printf_1_(&L,1);printf_1_(&H,1);//����5
	 
	 H=(uint8_t)((int16_t)(mpu->Gz)>>8); 
   L=(uint8_t)(int16_t)(mpu->Gz);
	 printf_1_(&L,1);printf_1_(&H,1);//����6
	 
   printf_1_(&p,1);printf_1_(&q,1);//֡β
  }
	
  else
	{
	  printf("%f  ",mpu->Ax);// printf("\n");
		printf("%f  ",mpu->Ay);// printf("\n");
		printf("%f  ",mpu->Az);// printf("\n\n");
		
		printf("%f  ",mpu->Gx);// printf("\n");
		printf("%f  ",mpu->Gy);// printf("\n");
		printf("%f  ",mpu->Gz); printf("\n\n");
		
	}
}

