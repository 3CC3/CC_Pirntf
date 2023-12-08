//
// Created by 86159 on 2023-09-19.
//

#ifndef MY_FLYBACK_MOTOR_H
#define MY_FLYBACK_MOTOR_H

#include "main.h"
#include "tim.h"
void Motor_Init(MOTOR *Motor);
void Motor_Control(MOTOR Motor,int16_t Speed);

#endif //MY_FLYBACK_MOTOR_H
