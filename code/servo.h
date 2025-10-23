#ifndef SERVO_H
#define SERVO_H

#include "zf_driver_pwm.h"
#define PWM_SERVO                (ATOM1_CH1_P33_9)
#define PWM_SERVO_FREQ            50

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)PWM_SERVO_FREQ)*(0.5+(float)(x)/90.0))

extern float SERVO_MOTOR_L_MAX;                                                // ���������϶�����Χ �Ƕ�
extern float SERVO_MOTOR_R_MAX;                                                // ���������϶�����Χ �Ƕ�
extern float SERVO_MOTOR_MID;                                                  // ���������϶���м�λ�� �Ƕ�

void servo_init();

void servo_set_angle(double angle);

#endif //SERVO_H