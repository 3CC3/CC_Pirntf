#include "MFRC522.h"

/*****
 * 输入16进制字符串(最多不超过（65535+1）/3)，自动输出字符串中所有十六进制数的异或值(计算校验位)
 * @param HexString 输入的十六进制字符串
 * @param result    用于存储结果的十六进制异或值
 * @return
 */
void Calculate_XOR(char *HexString,char *result)
{
    int i = 0,j = 0;
    int Result = 0;
    int temp = 0;
    int num = 0;
    while(*(HexString+i)!='\0')
    {
        while(*(HexString+i) != ' ' && *(HexString+i) != '\0')
        {
            if(*(HexString+i)>='A' && *(HexString+i)<='Z')
            {
                num = *(HexString+i) - 'A' + 10;
            }
            else num = *(HexString+i) - '0';
            temp*=16;
            temp = temp + num;
            i++;
        }
        if(*(HexString+i) == ' ')
            i++;
        Result ^= temp;
        temp = 0;
    }
}
//十进制转十六进制
void ConverteDec2Hex(int decimal,char *result)
{
    int i = 0,j,temp;
    char a[17]="0123456789ABCDEF";
    while(decimal)
    {
        result[i++] = a[decimal%16]; //逆序存储
        decimal/=16;
    }
    //逆回来
    i = i-1;
    for(j = 0;j<i;j++){
        temp = result[j];
        result[j] = result[i-j];
        result[i-j] = temp;
    }
    result[i+1] = '\0';
}
//计算从头到尾的异或值
uint8_t Calculate_XOR2(char *HexString,uint16_t Length)
{
    uint16_t i = 0;
    uint8_t Num = 0x00;
    for(i = 0;i<Length;i++)
    {
        Num^= *(HexString+i);
    }
    return Num;
}