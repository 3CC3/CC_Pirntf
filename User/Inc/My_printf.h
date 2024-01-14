//
// Created by 86159 on 2023-12-05.
//

#ifndef MY_FLYBACK_MY_PRINTF_H
#define MY_FLYBACK_MY_PRINTF_H

#include "main.h"
#include "stdarg.h"

/***********************************************************************************************************************************
 * 实现了自己的printf
 * 注意事项:
 * 1.函数开的用来存储可变参数的栈区默认是以sizeof(int)4个字节来存储参数的，所以无论是char、short当然还有int都是以四字节的形式存储在栈区中的
 * 2.当有double（8字节）类型的参数时，需要进行内存对齐（存放8字节变量的内存地址必须为8的整数倍）
 * 3.为了使编译器能够为我们进行类似标准prinf的格式字符waring检测，需要用到attribute指令来命令编译器
 *
 ***********************************************************************************************************************************/
typedef char* MY_va_list; //定义列表的类型
typedef char* STARTTYPE;  //可变参数前一个参数的的类型

typedef int native_int;   //定义一个本地存储的int，用于确定本编译器开辟函数栈区的单位长度（以int为单位长度）
#define FIXEDLENGTH sizeof(native_int)  //函数栈区单元的最小长度,默认就是sizeof(int),如果有更长的：如double共有8个字节，那么就要分配两份4字节的单元来存储



#define MEMALIGN(v,l)   ((sizeof(l) == 8 && ((int)v)%8!=0)?FIXEDLENGTH:0)//用于内存对齐，因为函数的栈区本来是默认4字节的，但是double类型是8字节的，所以有时候可能不能够直接接在4字节变量的后面存储8字节变量
#define MY_va_start(v,l) (v = (char*)&l+sizeof(STARTTYPE))  //指向可变参数第一个参数的地址
#define MY_va_arg(v,l)  ( *(l*)( (v += (sizeof(l) + MEMALIGN(v,l)) ) - sizeof(l)) ) //取出一个l类型的数据，并且让指针v指向下一个参数的地址，(注意：这里要先把v转为l*型的指针再取值，告诉指针每次要移动多少个字节，以防取错数据)
#define MY_va_end(v)    (v = (void*)NULL)       //结束后使得该指针指向NULL，防止出现野指针

int My_Sum(int count,...);
int Test(char a,int num,const char* string,...);
void MY_Retarget(UART_HandleTypeDef *huart);
void MY_printf(const char* fmt,...)
__attribute__((__format__(__printf__,1,2))); //1表示格式化字符串的位置是第一个，2表示可变参数的位置是第二个（这是个固定格式，不能变！）

#endif //MY_FLYBACK_MY_PRINTF_H
