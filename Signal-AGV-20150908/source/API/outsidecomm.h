#ifndef __OUTSIDECOMM_H
#define __OUTSIDECOMM_H

#include "Driver_Include.h"
#include "Global_Struct.h"

#define OUT_FRAME_HADE						0xA5
#define OUT_FRAME_END						0x5A

#define CMD_SEND_ACK_MSB					0xA0
#define CMD_SEND_ACK_LSB					0x00
#define CMD_SEND_NOW_STATUS_MSB				0xA2
#define CMD_SEND_NOW_STATUS_LSB				0x00
#define CMD_RECV_SCAN_MSB					0xA3
#define CMD_RECV_SCAN_LSB					0x00
#define CMD_RECV_START_ROUTE_SERAIL			0xA1
#define CMD_RECV_START_ROUTE				0x05
#define CMD_RECV_ROUTE_SERAIL				0x06

#define		OUT_DATA_SEND_LEN						64

/*
*
*定义收到主控板的有效数据后的回调函数的原型
*/
typedef void (*OutsideCB_FuncPtr_TypeDef)(unsigned char *cbptr,uint16_t len);

void OutsideCommInit(CAR_ATTRIBUTE *agv_data);
void OutsideSendData(char *data,uint16_t len);
void OutsideSendCommData(unsigned char *data,unsigned short len);
void SendOutSideACK(uint8_t flag,uint8_t rGroup,uint8_t rCMD);
void SendCarStatusToOutSide(void);
void OutsideRecvProcess(unsigned char *data,unsigned short len,OutsideCB_FuncPtr_TypeDef OutsideCB);
void OutsideRecvCheck(OutsideCB_FuncPtr_TypeDef OutsideCB);




#endif

