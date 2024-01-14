//
// Created by 86159 on 2023-09-30.
//
#include "Uart_RxPackage.h"

/***********************************************************************************************************************
 * 该串口接收不定长数据只能一次接收MAX_RxLength个字节以内的数据，若超过MAX_RxLength个字节，则会溢出
 * @param Uart_PackageHandle 串口接收数据包的句柄，在主函数中定义，于该函数赋初始值
 * @param htim               用于判断串口接收结束的定时器
 * @param MAX_RxLength       串口单次最大接收的字节数
 * @param huart              选择用哪个串口
 ***********************************************************************************************************************/

void Uart_RxPackage_Init(UartRx_Package *Uart_PackageHandle,TIM_HandleTypeDef *htim,uint16_t MAX_RxLength,UART_HandleTypeDef *huart)
{
    Uart_PackageHandle->huart = huart;
    Uart_PackageHandle->htim = htim;
    Uart_PackageHandle->Cur_RxLength = 0;
    Uart_PackageHandle->MAX_RxLength = MAX_RxLength;
    Uart_PackageHandle->Tx_Package = NULL;
    Uart_PackageHandle->Rx_Package = (uint8_t *)malloc(sizeof(uint8_t)*MAX_RxLength);
    Uart_PackageHandle->TempData = 0;
    Uart_PackageHandle->RxStartFalg = 0;
    Uart_PackageHandle->RxEndFalg = 0;
    Uart_PackageHandle->PackageStart = Uart_PackageStart;  //函数指针
}
/**
 * 开始串口接收数据函数
 * @param Uart_PackageHandle 传入串口数据包的句柄的地址，注意：这里只能是传入地址，否则HAL_UART_Receive_DMA()函数不能改变TempData的值，会出错
 */
void Uart_PackageStart(UartRx_Package *Uart_PackageHandle)
{
    HAL_UART_Receive_DMA(Uart_PackageHandle->huart,&Uart_PackageHandle->TempData,1); //�?启DMA接收串口数据
    HAL_TIM_Base_Start_IT(Uart_PackageHandle->htim);
}

/***********************************************************************************************************************
 * 该函数用于执行串口接收数据后所要干的事
 * @param Uart_PackageHandle    串口接收数据包的句柄，在主函数中定义
 * @return          SUCCESS:代表串口接收数据结束，该函数运行在正确的位置上
 *                  ERROR:代表该函数被放在了不对的位置上（串口接收未结束）
 **********************************************************************************************************************/
uint8_t Uart_DoTask(UartRx_Package *Uart_PackageHandle)
{
    if(Uart_PackageHandle->RxEndFalg == 1)
    {
        HAL_UART_Transmit(Uart_PackageHandle->huart,Uart_PackageHandle->Rx_Package,Uart_PackageHandle->Cur_RxLength,999);
        Uart_PackageHandle->RxEndFalg = 0;
        Uart_PackageHandle->Cur_RxLength = 0;
        memset(Uart_PackageHandle->Rx_Package,0,sizeof(uint8_t)*Uart_PackageHandle->Cur_RxLength); //初始化接收数组
    }
    else return ERROR;
    return SUCCESS;
}









/************************************************************************************************************************
 * 串口解包程序
 * @param InputString  输入的字符串
 * @param StringLength 输入字符串的长度
 * @param OutputString 解包后的字符串
 * @retval 1:My_Success 0:My_Error(说明校验码没通过)
 **********************************************************************************************************************/
Satus Uart_UpPack(char* InputString,uint16_t StringLength,char *OutputString)
{
    uint16_t i = 0,j = 0;
//    if(*InputString != 0x60 || (*(InputString+StringLength-1) != Calculate_XOR2(InputString,StringLength-1)) )
//        return My_Error;//说明帧头和校验码不正确

    for(i = 11;i<StringLength;i++,j++) //这里从i=6开始是为了去掉帧头
    {
        *(OutputString+j) = *(InputString+i);
    }
}


