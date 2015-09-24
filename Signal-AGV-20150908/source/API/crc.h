#ifndef _CRC_H
#define _CRC_H

#include "stm32f4xx.h"


unsigned short CalcCRC(unsigned char *data, unsigned short size, unsigned short pwd);

#endif

