/*
 * pid.c
 *
 *  Created on: 2024年10月20日
 *      Author: zzw
 */
#include "zf_common_headfile.h"

#define PID_i_sum_ON    //位置式PID是否包含积分项。如果仅用PD控制，注释本行

float kp_angle=1;
float ki_angle=0.1;
float kd_angle=0.1;
float i_max=500;        //位置式PID积分项最大值，用于限幅

float i_sum;        //位置式PID积分项
float angle_last_error;      //上一次误差
float angle_output_max=50;       //位置式PID输出最大值，用于限幅

/**********************************************************************************************************
*   函 数 名：PID_Cal
*   功能说明：位置式PID控制方向
*   输    入：
    NowValue:当前值
    AimValue:目标值
*   输    出：PID控制值，直接赋值给执行函数
**********************************************************************************************************/
float angle_pid_calc(int16 NowValue, int16 AimValue)
{

    float  iError;     //当前误差
    float Output;    //控制输出

    iError = AimValue - NowValue;                   //计算当前误差

#ifdef  PID_i_sum_ON
    i_sum += ki_angle * iError;               //位置式PID积分项累加
    i_sum = i_sum > i_max?i_max:i_sum;  //积分项上限幅
    i_sum = i_sum <-i_max?-i_max:i_sum; //积分项下限幅
    if(i_sum*iError<0)
        i_sum=0;
#endif

    Output = kp_angle * iError                        //比例P
           + kd_angle * (iError - angle_last_error);   //微分D

#ifdef  PID_i_sum_ON
    Output += i_sum;                        //积分I
#endif

    Output = Output > angle_output_max?angle_output_max:Output;  //控制输出上限幅
    Output = Output <-angle_output_max?-angle_output_max:Output; //控制输出下限幅

    angle_last_error = iError;                                //更新上次误差，用于下次计算
    return Output;  //返回控制输出值
}
float kp_speed;
float ki_speed;
float kd_speed;
float speed_output_max;      //输出最大值，用于限幅
float speed_last_error1;      //上一次误差
float speed_last_error2;      //上二次误差

/**********************************************************************************************************
*   函 数 名：IncPIDCal
*   功能说明：增量式PID控制速度
*   形    参：
*   返 回 值：
**********************************************************************************************************/
float speed_pid_calc(int16 NowValue, int16 AimValue)
{
    int32_t iError;                          //当前误差值
    float Output;                           //控制输出增量值

    iError = AimValue - NowValue;            //目标值与当前值之差

    Output = (kp_speed * iError)               //E[k]项
            -(ki_speed * speed_last_error1)       //E[k-1]项
            +(kd_speed * speed_last_error2);      //E[k-2]项

    speed_last_error2 = speed_last_error1;         //存储误差，用于下次计算
    speed_last_error1 = iError;

    Output = Output > speed_output_max?speed_output_max:Output;  //控制输出上限幅
    Output = Output <-speed_output_max?-speed_output_max:Output; //控制输出下限幅

    return(Output);                         //返回增量值
}

