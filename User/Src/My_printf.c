//
// Created by 86159 on 2023-12-05.

#include "My_printf.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "stdlib.h"

#define ghuart huart1
int My_Sum(int count,...)
{
    int sum = 0;
    va_list p; //定义一个va_list类型的指针(相当于改建一个列表)
    va_start(p,count); //通过count来使得p指向第一个参数的地址
    for(int i = 0;i < count;i++)
    {
        sum += va_arg(p,int); //取出一个int的p指针指向的地址的数据，并使得p指向下一个int地址
    }
    printf("第一个参数大小为：%d\r\n",sizeof(count));
    va_end(p); //使得p指针指向一个NULL，防止出现野指针
    return sum;
}

int Test(char a,int num,const char* string,...)
{
    MY_va_list p;
    MY_va_start(p,string);
    double *arg1 = &MY_va_arg(p, double);
    int *arg2 = &MY_va_arg(p,int);
    int *arg3 = &MY_va_arg(p,int);
    printf("第一个参数为：%f地址为：%p\r\n",*arg1,arg1);
    printf("第二个参数为：%d地址为：%p\r\n",*arg2,arg2);
    printf("第三个参数为：%d地址为：%p\r\n",*arg3,arg3);
    printf("前地址为：%p\r\n",&string);
    MY_va_end(p);
    return 0;  //本当に素晴らしい  实在是太美妙了
}
/**
 * 用于计算字符串的长度（必须要有‘\0’结束符）
 * @param string 要计算长度的字符串
 * @return 返回字符串的长度
 */
uint16_t StringLength(const char* string)
{
    uint16_t i = 0;
    while(*(string+i) != '\0')i++;
    return i;
}
/**
 * 自己的printf
 * @param fmt 格式字符
 * @param ... 输入的参数
 */
void MY_printf(const char* fmt,...)
{
    int i = 0,k = 0;
    char tempstring[10]; //用于暂时存储要输出的字符串
    MY_va_list p;
    MY_va_start(p,fmt);
    while(*(fmt+i) != '\0')
    {
        if(*(fmt+i) == '%')
        {
            HAL_UART_Transmit(&ghuart,(uint8_t*)(fmt+k),i-k,999); //仅发送字符到'%'之间的纯字符
            i++;
            switch(*(fmt+i))
            {
                case 'd':  //输出整数
                {
                    itoa(MY_va_arg(p,int),tempstring,10);
                    HAL_UART_Transmit(&ghuart,(uint8_t*)tempstring,StringLength(tempstring),999); //以字符串的形式发数字
                    memset(tempstring,0,10);
                }break;
                case 'c': //输出字符
                {
                    HAL_UART_Transmit(&ghuart,(uint8_t*)&MY_va_arg(p,int),1,999); //以字符串的形式发数字
                }break;
                case 'p': //输出地址
                {
                    tempstring[0] = '0';  //加上十六进制的头
                    tempstring[1] = 'x';
                    itoa((int)MY_va_arg(p,int),tempstring+2,16); //把十进制数转为十六进制数后的字符串
                    HAL_UART_Transmit(&ghuart,(uint8_t*)tempstring,StringLength(tempstring),999); //以字符串的形式发数字
                    memset(tempstring,0,10);  //清除数组防止下次使用的时候有前一次的字符未被覆盖
                }break;
                case 's':  //输出字符串
                {
                    uint8_t *string = (uint8_t*)MY_va_arg(p,int); //取得字符串的首地址
                    HAL_UART_Transmit(&ghuart,string,StringLength((char*)string),999); //以字符串的形式发数字
                }break;
                case 'f': //输出浮点数
                {
                    sprintf(tempstring,"%f",(double)MY_va_arg(p,double)); //注意，取出浮点数的时候要用double类型的（8个字节），前四个字节为低位，后四个字节为高位(不完全是整数部分)
                    HAL_UART_Transmit(&ghuart,(uint8_t*)tempstring,StringLength(tempstring),999); //以字符串的形式发数字
                    memset(tempstring,0,10);  //清除数组防止下次使用的时候有前一次的字符未被覆盖
                }break;
            }
            i++;
            k = i; //记录纯字符的起始位置
        }
        i++;
    }
    MY_va_end(p);
   HAL_UART_Transmit(&ghuart,(uint8_t*)(fmt+k),i-k,999); //发送最后一个'%'到'\n'之间的字符
}