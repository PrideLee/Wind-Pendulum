#include "Attitude.h"
#include "math.h"


float Kp=2.0f;                        //  10.0f   �����Kp��Ki�����ڵ������ٶȼ����������ǻ�����̬���ٶ�   Խ������Խ�����ܼ��ٶ�Ӱ��ԽС��Ӧ�ðɣ�

float Ki=0.008f;                       //  0.008f  ò��Сһ���

#define halfT 0.005f             // �������ڵ�һ�룬�˴�������̬�ٶ�Ϊ100HZ�����������Ԫ��΢�ַ���ʱ���������


float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // ��ʼ��̬��Ԫ�����ɱ任��Ԫ����ʽ����

float exInt = 0, eyInt = 0, ezInt = 0;    //��ǰ���ٶȼƲ�õ��������ٶ��������ϵķ���

                                //���õ�ǰ��̬��������������������ϵķ��������Ļ���
/*����Ϊ��̬���㷨*/
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, struct Attitude *AT)//g�������ǣ�a����ٶȼƲ�����gx��gy��gz�ֱ��Ӧ������Ľ��ٶȣ���λ�Ƕ�/��;����ax��ay��az�ֱ��Ӧ������ļ��ٶ�ԭʼ����
{
  float q0temp,q1temp,q2temp,q3temp;//��Ԫ���ݴ���������΢�ַ���ʱҪ��

  float norm; //ʸ����ģ����Ԫ���ķ���

  float vx, vy, vz;//��ǰ��̬��������������������ϵķ���

  float ex, ey, ez;//��ǰ�ӼƲ�õ��������ٶ��������ϵķ���

              //���õ�ǰ��̬��������������������ϵķ��������

 

  // 

  float q0q0;// = q0*q0;

  float q0q1;// = q0*q1;

  float q0q2;// = q0*q2;

  float q1q1;// = q1*q1;

  float q1q3;// = q1*q3;

  float q2q2;// = q2*q2;

  float q2q3;// = q2*q3;

  float q3q3;// = q3*q3;

  q0q0=q0*q0;
	q0q1=q0*q1;
	q0q2=q0*q2;
	q1q1=q1*q1;
	q1q3=q1*q3;
	q2q2=q2*q2;
	q2q3=q2*q3;
	q3q3=q3*q3;
	
  //תΪ ����/��
	gx/=57.296f;
	gy/=57.296f;
	gz/=57.296f;
	

  if(ax*ay*az==0)//�Ӽƴ�����������״̬ʱ��������̬���㣬��Ϊ�������ĸ���������

        return;

                  

  norm = My_Sqrt(ax*ax + ay*ay + az*az);//�����ٶȵ�ԭʼ���ݣ���һ�����õ���λ���ٶ�,�������������Ҳ����Ҫ�޸�KP������

  ax = ax /norm;

  ay = ay / norm;

  az = az / norm;

 

  //�õ�ǰ��̬������������������ϵķ�����

  //�ο�����nϵת������������bϵ������Ԫ����ʾ�ķ������Ҿ�������м���

  vx = 2*(q1q3 - q0q2);                                                                                                             

  vy = 2*(q0q1 + q2q3);

  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

 

  //�����õ������������������������������Ա�ʾ��һ����Ϊ���������ǵ�λ������sin0����0


  ex = (ay*vz - az*vy) ;                                                                  

  ey = (az*vx - ax*vz) ;

  ez = (ax*vy - ay*vx) ;

 //����������л���

  exInt = exInt + ex * Ki;                                           //�������л���

  eyInt = eyInt + ey * Ki;

  ezInt = ezInt + ez * Ki;

 

  //�ò���������PI����������ƫ��ͨ������Kp��Ki��������,���Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶ�

  gx = gx + Kp*ex + exInt;  //�����PI�󲹳��������ǣ����������Ư��

  gy = gy + Kp*ey + eyInt;

  gz = gz + Kp*ez + ezInt;    //�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

 

  //���������̬�ĸ��£�Ҳ������Ԫ��΢�ַ��̵����

  q0temp=q0;//�ݴ浱ǰֵ���ڼ���

  q1temp=q1;
  q2temp=q2;

  q3temp=q3;

  //����һ�ױϿ��ⷨ�����֪ʶ�ɲμ���������������Ե���ϵͳ��P212

  q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;

  q1 = q1temp + (q0temp*gx + q2temp*gz -q3temp*gy)*halfT;

  q2 = q2temp + (q0temp*gy - q1temp*gz +q3temp*gx)*halfT;

  q3 = q3temp + (q0temp*gz + q1temp*gy -q2temp*gx)*halfT;

 

  //��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕȣ����������Դ�����������任

  norm = My_Sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

  q0 = q0 / norm;

  q1 = q1 / norm;

  q2 = q2 / norm;

  q3 = q3 / norm;

 

  //��Ԫ����ŷ���ǵ�ת��

  //����YAW��������ڼ��ٶȼƶ���û���������ã���˴˴�ֱ���������ǻ��ִ���

//  Q_ANGLE.Z = GYRO_I.Z; // yaw

  AT->X = asin(-2 * q1 * q3 + 2 * q0* q2)*57.3; // pitch

  AT->Y = atan2(2 * q2 * q3 + 2 * q0 * q1,-2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll

}



float My_Sqrt(float x)  
{  
        float xhalf = 0.5f*x;  
        int i = *(int*)&x; // get bits for floating VALUE  
        i= 0x5f375a86- (i>>1); // gives initial guess y0  
        x= *(float*)&i; // convert bits BACK to float  
        x= x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy  
        return 1/x;  
}


