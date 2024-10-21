/*
 * speed_control.c
 *
 *  Created on: 2024��10��19��
 *      Author: zzw
 */
#include "zf_common_headfile.h"

int16 lspeed_now,rspeed_now;
/*
 * lspeed,rspeed:-10000~10000
 */
void motor_speed_output(int16 lspeed,int16 rspeed)
{
    pwm_set_duty(MOTORL_PWM_CH1,5000+lspeed/2);
    pwm_set_duty(MOTORL_PWM_CH2,5000-lspeed/2);
    pwm_set_duty(MOTORR_PWM_CH1,5000+rspeed/2);
    pwm_set_duty(MOTORR_PWM_CH2,5000-rspeed/2);
}

void get_speed_now()
{
    lspeed_now=encoder_get_count(TIM4_ENCODER);
    rspeed_now=encoder_get_count(TIM6_ENCODER);
    encoder_clear_count(TIM4_ENCODER);
    encoder_clear_count(TIM6_ENCODER);
}

/*
 * ��ֵ737
 * ռ�ձ�:640~820������640-820=180�� 640���ң�820����
 */
void servo_output(int8 angle)
{
    pwm_set_duty(ATOM1_CH1_P33_9,737-angle);
}
