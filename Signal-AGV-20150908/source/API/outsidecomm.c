

#include "OutSidecomm.h"
#include "crc.h"


uint8_t outSideSendBuf[OUT_DATA_SEND_LEN];
uint16_t lastOutSienSendLen;

CAR_ATTRIBUTE *agvRunStatus;
/****************************************************************************
* 名    称：OutSideCommInit
* 功    能：初始化通信
* 参    数：agv_data  AGV的运行数据地址
* 反    回：无
* 说    明：
****************************************************************************/ 	
void OutsideCommInit(CAR_ATTRIBUTE *agv_data)
{

	USART2_Init(BAUD_115200);
	
	agvRunStatus=agv_data;
	
	outSideSendBuf[0]=OUT_FRAME_HADE;
	outSideSendBuf[1]=OUT_FRAME_HADE;
	outSideSendBuf[2]='A';//0x41
	outSideSendBuf[3]=0x00;
	outSideSendBuf[4]=agvRunStatus->AGVID>>8;
	outSideSendBuf[5]=agvRunStatus->AGVID&0x00FF;
	outSideSendBuf[6]=0x00;
	outSideSendBuf[7]=0x00;
	outSideSendBuf[8]=0x51;
	outSideSendBuf[9]=0x01;
}

/****************************************************************************
* 名    称：OutsideSendData
* 功    能：通地串口发送数据
* 参    数：data 要发送数据的首地址  len 要发送的长度
* 反    回：无
* 说    明：
****************************************************************************/ 
void OutsideSendData(char *data,uint16_t len)
{
	USART2_Send_Str(data,len);
}

/****************************************************************************
* 名    称：void OutsideSendCommData
* 功    能：外部接口，发送数据
* 参    数：*data 传入数据首地址，len数据长度
* 反    回：无
* 说    明：
****************************************************************************/ 	
void OutsideSendCommData(unsigned char *data,unsigned short len)
{
	unsigned short crcValue;
	
	outSideSendBuf[10]=len>>8;
	outSideSendBuf[11]=len&0x00FF;
  memcpy(&outSideSendBuf[12],data,len);
	crcValue=CalcCRC(&outSideSendBuf[2],len+10,0x0000);
  outSideSendBuf[len+12]=crcValue>>8;
  outSideSendBuf[len+13]=crcValue&0x00FF;
  outSideSendBuf[len+14]=OUT_FRAME_END;
	lastOutSienSendLen=len+15;
	
  OutsideSendData((char*)outSideSendBuf,lastOutSienSendLen);
}

/****************************************************************************
* 名    称：SendOutSideACK
* 功    能：收到数据后，做出的应答
* 参    数：flag 应答是否正常的标志,rGroup 应答的数据编组，rCMD 应答的数据指令
* 反    回：无
* 说    明：
****************************************************************************/ 
void SendOutSideACK(uint8_t flag,uint8_t rGroup,uint8_t rCMD)
{
  unsigned char buf[5];
	
	buf[0]=CMD_SEND_ACK_MSB;
  buf[1]=CMD_SEND_ACK_LSB;
	buf[2]=flag;
	buf[3]=rGroup;
	buf[4]=rCMD;
  OutsideSendCommData(buf,5);
}

/****************************************************************************
* 名    称：void SendCarStatusToOutSide(void)
* 功    能：发送AGV状态数据
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/ 
void SendCarStatusToOutSide(void)
{
	uint8_t buf[16];
	
	buf[0]=CMD_SEND_NOW_STATUS_MSB;
	buf[1]=CMD_SEND_NOW_STATUS_LSB;
	buf[2]=0x02;
	buf[3]=agvRunStatus->Destination>>8;
	buf[4]=agvRunStatus->Destination&0x00FF;
	buf[5]=agvRunStatus->Voltage>>8;
	buf[6]=agvRunStatus->Voltage&0x00FF;
	buf[7]=agvRunStatus->NowStatus;
	buf[8]=agvRunStatus->NowSpeed;
	buf[9]=agvRunStatus->LastStation>>8;
	buf[10]=agvRunStatus->LastStation&0x00FF;
	buf[11]=agvRunStatus->Distance>>8;
	buf[12]=agvRunStatus->Distance&0x00FF;
	buf[13]=agvRunStatus->NextStation>>8;
	buf[14]=agvRunStatus->NextStation&0x00FF;
	buf[15]=0x00;
	OutsideSendCommData(&buf[0],16);
}



/****************************************************************************
* 名    称：void OutSideRecvProcess
* 功    能：串口收到数据处理过程
* 参    数：*data 传入数据首地址，len数据长度
* 反    回：无
* 说    明：
****************************************************************************/ 
void OutsideRecvProcess(unsigned char *data,unsigned short len,OutsideCB_FuncPtr_TypeDef OutsideCB)
{		
	unsigned short crcValue,checkValue;
	
	if((data[0]==OUT_FRAME_HADE)&&(data[1]==OUT_FRAME_HADE))
	{
		crcValue = CalcCRC(&data[2],len-5,0x0000);
	  checkValue = data[len-3]<<8 | data[len-2];
	  if(crcValue == checkValue)
		{
			switch(data[10])
			{
				case CMD_RECV_ROUTE_SERAIL:
				{
					if(data[11]==CMD_RECV_START_ROUTE)
					{
						SendOutSideACK(0,CMD_RECV_ROUTE_SERAIL,CMD_RECV_START_ROUTE);
					}else
					if(data[11]==CMD_RECV_ROUTE_SERAIL)
					{
						SendOutSideACK(0,CMD_RECV_ROUTE_SERAIL,CMD_RECV_ROUTE_SERAIL);
						OutsideCB((char*)&data[12],len - 5);
					}
					break;
				}
			}
		}
	}
	else
	{
		
	}	
}
/****************************************************************************
* 名    称：OnsideRecvCheck
* 功    能：检查串口是否收到数据
* 参    数：数据有效后调用的处理函数
* 反    回：无
* 说    明：
****************************************************************************/
void OutsideRecvCheck(OutsideCB_FuncPtr_TypeDef OutsideCB)
{
	if(UART2_Str.Recv_Flag)
	{
		UART2_Str.Recv_Flag = 0;
		OutsideRecvProcess((uint8_t*)UART2_Str.data,UART2_Str.Recv_Len,OutsideCB);
	}
}


