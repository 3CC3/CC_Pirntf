#include "OLED.h"
#include "OLED_Font.h"
#include "stm32f103xb.h"

/*引脚配置*/
/*
//#define OLED_W_SCL(x)        OLED_SCL_GPIO_Port->BSRR = ((uint32_t)OLED_SCL_Pin)<<(16*!x)
// 注意这里不能用这个宏定义，因为引脚翻转速度太快了，从机不一定能跟上，或者达到了引脚翻转的速度极限
*/

#define OLED_W_SDA(x)		HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, (x))

void OLED_W_SCL(uint8_t x)
{
    OLED_SCL_GPIO_Port->BSRR = ((uint32_t)OLED_SCL_Pin)<<(16*!x);
}

//读取一个字节 高位在前
uint8_t I2C_R_Byte(void)
{
    uint8_t i;
    uint8_t Data = 0x00;
    OLED_W_SDA(1); //主机释放数据线
    for(i =0;i<8;i++){
        OLED_W_SCL(1); //主机读数据
        if(HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port,OLED_SDA_Pin)) Data |= (0x80>>i);
        OLED_W_SCL(0); //从机写数据
    }
    return Data;
}
/*引脚初始化*/
void OLED_I2C_Init(void)
{
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(Byte & (0x80 >> i));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
    OLED_W_SCL(0);
}
//接收应答位
uint8_t I2C_ReceiveACK(void)
{
//    uint8_t MYACK = 0;
//    OLED_W_SDA(1); //释放SDA
//    OLED_W_SCL(1);//拉高SCL让从机放数据
//    MYACK = HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port,OLED_SDA_Pin);
//    OLED_W_SCL(0);
//    return MYACK;
    
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);		//从机地址
    OLED_I2C_SendByte(0x00);		//写命令
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);		//从机地址
    OLED_I2C_SendByte(0x40);		//写数据
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}
/**
  * @brief  OLED读状态
  * @param  无
  * @retval Status:1代表显示关闭    0代表显示开启
  */
uint8_t OLED_ReadStatus(void)
{
    uint8_t Status = 0x00;
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x79); //以读的形式
    Status = I2C_R_Byte();
    return Status;
}
/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);					//设置Y位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for(i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
    }
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}
/**
  * @brief  判定传入的整数的长度
  * @param  Number 传入的正整数，范围：0~4294967295
  * @retval 返回正整数的长度
  */
uint8_t OLED_JudgeNum(uint32_t Number)
{
    uint8_t i = 10,Length = 1;
    while(Number/i){
        Length++;
        i*=10;
    }
    return Length;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}
/**
  * @brief  OLED显示小数,最大5位小数（十进制，正负数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：3.4E-38 ~3.4E+38
  * @param  InLength 要显示数字的整数的长度，范围：1~10
  * @param  DeLength 要显示数字的小数的长度，范围：1~5
  * @retval 无
  */
void OLED_ShowFNum(uint8_t Line, uint8_t Column, float Number, uint8_t InLength, uint8_t DeLength)
{
    float decimal;  //小数的位数
    //uint8_t integral; //整数的位数
    uint8_t i;
    int Num[5] = {0};
    if(Number>=0){
        OLED_ShowChar(Line, Column, '+');
    }
    else {
        Number = -Number;
        OLED_ShowChar(Line, Column, '-');
    }
    //integral = OLED_JudgeNum((int)Number);  //返回整数的位数
    decimal = Number - (int)Number; //取出小数
    Num[0] = ((int)(decimal*10))%10;
    Num[1] = ((int)(decimal*100))%10;
    Num[2] = ((int)(decimal*1000))%10;
    Num[3] = ((int)(decimal*10000))%10;
    Num[4] = ((int)(decimal*100000))%10;

    OLED_ShowNum(Line, Column+1, (int)Number, InLength); //显示整数部分
    OLED_ShowChar(Line, Column + 1 + InLength, '.');
    if(DeLength>5)DeLength = 5;
    for(i = 1; i <= DeLength; i++){
        OLED_ShowChar(Line, Column + 1 + InLength + i, Num[i - 1] + '0');
    }
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0; i < 1000; i++)			//上电延时
    {
        for (j = 0; j < 1000; j++);
    }

    OLED_I2C_Init();			//端口初始化

    OLED_WriteCommand(0xAE);	//关闭显示

    OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);	//设置多路复用率
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);	//设置显示偏移
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);	//设置显示开始行

    OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);	//设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9);	//设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

    OLED_WriteCommand(0xA6);	//设置正常/倒转显示

    OLED_WriteCommand(0x8D);	//设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF);	//开启显示

    OLED_Clear();				//OLED清屏
}
