//
// Created by 86159 on 2023-09-30.
//

#ifndef MY_FLYBACK_UART_RXPACKAGE_H
#define MY_FLYBACK_UART_RXPACKAGE_H

#include "main.h"
#include "stdlib.h"
#include "string.h"

typedef enum
{
    My_Error = 0,
    My_Success,
}Satus;
void Uart_RxPackage_Init(UartRx_Package *Uart_PackageHandle,TIM_HandleTypeDef *htim,uint16_t MAX_RxLength,UART_HandleTypeDef *huart);
uint8_t Uart_DoTask(UartRx_Package *Uart_PackageHandle);
void Uart_PackageStart(UartRx_Package *Uart_PackageHandle);


Satus Uart_UpPack(char* InputString,uint16_t StringLength,char *OutputString);
#endif //MY_FLYBACK_UART_RXPACKAGE_H
