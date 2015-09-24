
#ifndef __INSIDE_COMM_H
#define __INSIDE_COMM_H

#include "Driver_Include.h"
#include "Magnet_COMM.h"

#define INSIDE_RESEND_MAX_NUM    2  //最大重发次数

//与主控板的通信指令
#define CMD_DRIVER_ACK						0xA0
#define CMD_DRIVER_INIT_PARAMTER  0xC1
#define CMD_DRIVER_RUN_ACTION			0xC2
#define CMD_DRIVER_RUN_STATUS     0xC3
#define CMD_DRIVER_READ_RFID      0xC4
#define CMD_DRICER_ERROR					0xC5

/*
*
*定义收到主控板的有效数据后的回调函数的原型
*/
typedef void (*InsideCB_FuncPtr_TypeDef)(char *cbptr,uint16_t len);

extern _Comm_Send_Str Inside_Send_Str;


void InsideCommInit(void);
void InsideSendData(char *data,uint16_t len);
int8_t InsideSendTimeout(void *cbptr);
void InsideSendCommData(unsigned char *data,unsigned short len);
void InsideSendACK(unsigned char flag);
void InsideRecvProcess(unsigned char *data,unsigned short len,InsideCB_FuncPtr_TypeDef InsideCB);
void InsideRecvCheck(InsideCB_FuncPtr_TypeDef InsideCB);

#endif


