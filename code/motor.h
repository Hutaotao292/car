#ifndef MOTOR_H
#define MOTOR_H


#include "zf_driver_pwm.h"
#include "servo.h"

#define PWM_RIGHT_1                (ATOM0_CH1_P21_3)
#define PWM_RIGHT_2                (ATOM0_CH3_P21_5)
#define PWM_LEFT_1                 (ATOM0_CH0_P21_2)
#define PWM_LEFT_2                 (ATOM0_CH2_P21_4)
#define PWM_MOTOR_FREQ                15000

void motor_init();

#endif //MOTOR_H