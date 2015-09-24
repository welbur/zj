

#ifndef __MAGNET_COMM_H
#define __MAGNET_COMM_H

#include "Driver_Include.h"


//磁传感器通信指令
#define CMD_MAGNET_ACK					    0xA0
#define CMD_MAGNET_CHECK_DATA				0xD2
#define CMD_MAGNET_RFID_CARD				0xD3

//帧头帧尾
#define FRAME_HADE                          0x20
#define FRAME_END                           0xFE

#define CMD_POSTTION                1
#define DATA_START_POSITION					2

//应答
#define ACK_PASS							0x01
#define ACK_ERROR							0x02


#define MAGNET_RESEND_MAX_NUM    2  //最大重发次数

/*
*
*定义收到主控板的有效数据后的回调函数的原型
*/
typedef void (*MagnetCB_FuncPtr_TypeDef)(char *cbptr,uint16_t len);

typedef struct
{
	uint16_t 	CommPwd;  //CRC检验种子
	char 		  SendBuf[64];
	uint16_t  SendLen;
	uint8_t   ReSendCount;//重发次数
}_Comm_Send_Str;

extern _Comm_Send_Str Magnet_Send_Str;

extern uint16_t MagneticValue;   //磁传感器值

void MagnetCommInit(void);
void MagnetSendData(char *data,uint16_t len);
int8_t MagnetSendTimeout(void *cbptr);
void MagnetSendCommData(unsigned char *data,unsigned short len);
void MagnetSendACK(unsigned char flag);
void MagnetReport(uint8_t flag);
void MagnetRecvProcess(unsigned char *data,unsigned short len,MagnetCB_FuncPtr_TypeDef MagnetCB);
void MagnetRecvCheck(MagnetCB_FuncPtr_TypeDef MagnetCB);




#endif



