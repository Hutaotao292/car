#include "motor.h"


void motor_init()
{
    pwm_init(PWM_LEFT_1,PWM_MOTOR_FREQ,0);
    pwm_init(PWM_LEFT_2,PWM_MOTOR_FREQ,0);
    pwm_init(PWM_RIGHT_1,PWM_MOTOR_FREQ,0);
    pwm_init(PWM_RIGHT_2,PWM_MOTOR_FREQ,0);
}

