/*
 * speed_control.h
 *
 *  Created on: 2024年10月19日
 *      Author: zzw
 */

#ifndef CODE_SPEED_CONTROL_H_
#define CODE_SPEED_CONTROL_H_

#include "zf_common_headfile.h"

extern int16 lspeed_now;
extern int16 rspeed_now;

extern float kp_angle;
extern float ki_angle;
extern float kd_angle;
extern float i_max;        //位置式PID积分项最大值，用于限幅
extern float angle_output_max;
extern float kp_speed;
extern float ki_speed;
extern float kd_speed;

float angle_pid_calc(int16 NowValue, int16 AimValue);
float speed_pid_calc(int16 NowValue, int16 AimValue);

void motor_speed_output(int16 lspeed,int16 rspeed);
void servo_output(int8 angle);
void get_speed_now();
#endif /* CODE_SPEED_CONTROL_H_ */
