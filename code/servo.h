#ifndef SERVO_H
#define SERVO_H

#include "zf_driver_pwm.h"
#define PWM_SERVO                (ATOM1_CH1_P33_9)
#define PWM_SERVO_FREQ            50

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)PWM_SERVO_FREQ)*(0.5+(float)(x)/90.0))

extern float SERVO_MOTOR_L_MAX;                                                // 定义主板上舵机活动范围 角度
extern float SERVO_MOTOR_R_MAX;                                                // 定义主板上舵机活动范围 角度
extern float SERVO_MOTOR_MID;                                                  // 定义主板上舵机中间位置 角度

void servo_init();

void servo_set_angle(double angle);

#endif //SERVO_H