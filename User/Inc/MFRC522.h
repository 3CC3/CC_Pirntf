#ifndef MFRC522_H_
#define MFRC522_H_

#include "main.h"
#include <stdint.h>


void Calculate_XOR(char *HexString,char *result);
void ConverteDec2Hex(int decimal,char *result);
uint8_t Calculate_XOR2(char *HexString,uint16_t Length);

#endif /* MFRC522_H_ */
