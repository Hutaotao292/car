#include "servo.h"

float SERVO_MOTOR_L_MAX =           (85 );                                       // 定义主板上舵机活动范围 角度
float SERVO_MOTOR_R_MAX =           (110);                                       // 定义主板上舵机活动范围 角度
float SERVO_MOTOR_MID =             (97 );                                       // 定义主板上舵机中间位置 角度


void servo_init()
{
    pwm_init(PWM_SERVO,PWM_SERVO_FREQ,0);
}

void servo_set_angle(double angle){
    if(angle > SERVO_MOTOR_R_MAX){
        angle = SERVO_MOTOR_R_MAX;
    } else if(angle < SERVO_MOTOR_L_MAX){
        angle = SERVO_MOTOR_L_MAX;
    }
    pwm_set_duty(PWM_SERVO, (uint32_t)SERVO_MOTOR_DUTY(angle));
}