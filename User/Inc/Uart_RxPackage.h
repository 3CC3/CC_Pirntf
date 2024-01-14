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
typedef struct
{
    UART_HandleTypeDef *huart; //使用的串口
    TIM_HandleTypeDef *htim; //串口接收数据所用到的定时器
    uint16_t MAX_RxLength;//�???大字节数
    uint16_t Cur_RxLength; //当前串口接收数组中的字节�???
    uint8_t *Rx_Package; //字符接收数组
    uint8_t *Tx_Package; //字符发�?�数�???
    uint8_t TempData; //用于暂时存储接收到的�???个字�???
    volatile uint8_t RxStartFalg; //串口�???始接收字符标志位   因为标志位会在中断里面随时变化，所以是’易变的‘需要加上‘volatile’防止被编译器优化
    volatile uint8_t RxEndFalg; //串口结束接收字符标志�???
}__attribute__((packed))UartRx_Package;


void Uart_RxPackage_Init(UartRx_Package *Uart_PackageHandle,TIM_HandleTypeDef *htim,uint16_t MAX_RxLength,UART_HandleTypeDef *huart);
uint8_t Uart_DoTask(UartRx_Package *Uart_PackageHandle);
void Uart_PackageStart(UartRx_Package *Uart_PackageHandle);


Satus Uart_UpPack(char* InputString,uint16_t StringLength,char *OutputString);
#endif //MY_FLYBACK_UART_RXPACKAGE_H
