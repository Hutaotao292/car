#include "servo.h"

float SERVO_MOTOR_L_MAX =           (85 );                                       // ���������϶�����Χ �Ƕ�
float SERVO_MOTOR_R_MAX =           (110);                                       // ���������϶�����Χ �Ƕ�
float SERVO_MOTOR_MID =             (97 );                                       // ���������϶���м�λ�� �Ƕ�


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