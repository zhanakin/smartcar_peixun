/*
 * pid.c
 *
 *  Created on: 2024��10��20��
 *      Author: zzw
 */
#include "zf_common_headfile.h"

#define PID_i_sum_ON    //λ��ʽPID�Ƿ����������������PD���ƣ�ע�ͱ���

float kp_angle=1;
float ki_angle=0.1;
float kd_angle=0.1;
float i_max=500;        //λ��ʽPID���������ֵ�������޷�

float i_sum;        //λ��ʽPID������
float angle_last_error;      //��һ�����
float angle_output_max=50;       //λ��ʽPID������ֵ�������޷�

/**********************************************************************************************************
*   �� �� ����PID_Cal
*   ����˵����λ��ʽPID���Ʒ���
*   ��    �룺
    NowValue:��ǰֵ
    AimValue:Ŀ��ֵ
*   ��    ����PID����ֵ��ֱ�Ӹ�ֵ��ִ�к���
**********************************************************************************************************/
float angle_pid_calc(int16 NowValue, int16 AimValue)
{

    float  iError;     //��ǰ���
    float Output;    //�������

    iError = AimValue - NowValue;                   //���㵱ǰ���

#ifdef  PID_i_sum_ON
    i_sum += ki_angle * iError;               //λ��ʽPID�������ۼ�
    i_sum = i_sum > i_max?i_max:i_sum;  //���������޷�
    i_sum = i_sum <-i_max?-i_max:i_sum; //���������޷�
    if(i_sum*iError<0)
        i_sum=0;
#endif

    Output = kp_angle * iError                        //����P
           + kd_angle * (iError - angle_last_error);   //΢��D

#ifdef  PID_i_sum_ON
    Output += i_sum;                        //����I
#endif

    Output = Output > angle_output_max?angle_output_max:Output;  //����������޷�
    Output = Output <-angle_output_max?-angle_output_max:Output; //����������޷�

    angle_last_error = iError;                                //�����ϴ��������´μ���
    return Output;  //���ؿ������ֵ
}
float kp_speed;
float ki_speed;
float kd_speed;
float speed_output_max;      //������ֵ�������޷�
float speed_last_error1;      //��һ�����
float speed_last_error2;      //�϶������

/**********************************************************************************************************
*   �� �� ����IncPIDCal
*   ����˵��������ʽPID�����ٶ�
*   ��    �Σ�
*   �� �� ֵ��
**********************************************************************************************************/
float speed_pid_calc(int16 NowValue, int16 AimValue)
{
    int32_t iError;                          //��ǰ���ֵ
    float Output;                           //�����������ֵ

    iError = AimValue - NowValue;            //Ŀ��ֵ�뵱ǰֵ֮��

    Output = (kp_speed * iError)               //E[k]��
            -(ki_speed * speed_last_error1)       //E[k-1]��
            +(kd_speed * speed_last_error2);      //E[k-2]��

    speed_last_error2 = speed_last_error1;         //�洢�������´μ���
    speed_last_error1 = iError;

    Output = Output > speed_output_max?speed_output_max:Output;  //����������޷�
    Output = Output <-speed_output_max?-speed_output_max:Output; //����������޷�

    return(Output);                         //��������ֵ
}

