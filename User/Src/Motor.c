//
// Created by 86159 on 2023-09-19.
//
#include "Motor.h"

//Speed取值在0-100之间

void Motor_Init(MOTOR *Motor)
{
    Motor->Direction = 1;
    Motor->Name = "TemperatureMotor";
    Motor->Speed = 0;
    Motor->TIM = htim1;
    Motor->Channel = TIM_CHANNEL_1;
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_GPIO_WritePin(Motor1_A_GPIO_Port,Motor1_A_Pin,GPIO_PIN_SET); //默认电机正转
    HAL_GPIO_WritePin(Motor1_B_GPIO_Port,Motor1_B_Pin,GPIO_PIN_RESET);

}
void Motor_Control(MOTOR Motor,int16_t Speed)
{
    Motor.Speed = Speed;
    if(Speed > 0)
    {
        if(Speed >=100) Speed = 100;
        HAL_GPIO_WritePin(Motor1_A_GPIO_Port,Motor1_A_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(Motor1_B_GPIO_Port,Motor1_B_Pin,GPIO_PIN_RESET);
    }
    else if(Speed < 0)
    {
        if(Speed <= -100) Speed = -100;
        HAL_GPIO_WritePin(Motor1_A_GPIO_Port,Motor1_A_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor1_B_GPIO_Port,Motor1_B_Pin,GPIO_PIN_SET);
        Speed = -Speed;
    }
    __HAL_TIM_SET_COMPARE(&Motor.TIM,Motor.Channel,Speed);
}