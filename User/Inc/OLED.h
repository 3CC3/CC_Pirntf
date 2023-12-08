//
// Created by 86159 on 2023-10-23.
//

#ifndef MY_FLYBACK_OLED_H
#define MY_FLYBACK_OLED_H

#include "main.h"
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFNum(uint8_t Line, uint8_t Column, float Number, uint8_t InLength, uint8_t DeLength);
uint8_t OLED_ReadStatus(void);

#endif //MY_FLYBACK_OLED_H
