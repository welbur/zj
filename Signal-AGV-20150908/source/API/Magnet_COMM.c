

#include "Magnet_COMM.h"
#include "crc.h"

_Comm_Send_Str Magnet_Send_Str;

static int8_t Magnet_Resend_Timer_Index = -1;

/****************************************************************************
* 名    称：MagneticCommInit
* 功    能：磁传感器，初始化通信
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetCommInit(void)
{
	
	USART1_Init(BAUD_115200);
	memset((char*)&Magnet_Send_Str,0x00,sizeof(Magnet_Send_Str));
	Magnet_Send_Str.CommPwd = 0;
	
	MagnetReport(1);//磁传感器处于正常模式
	
}
/****************************************************************************
* 名    称：void MagnetSendData(char *data,uint16_t len)
* 功    能：发送数据给磁传感器
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetSendData(char *data,uint16_t len)
{
	USART1_Send_Str(data,len);
}
/****************************************************************************
* 名    称：MagnetSendTimeout
* 功    能：发送数据给磁传感器超时处理
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
int8_t MagnetSendTimeout(void *cbptr)
{
	cbptr = cbptr;
	if(Magnet_Send_Str.ReSendCount < MAGNET_RESEND_MAX_NUM)
	{
		Magnet_Send_Str.ReSendCount++;
		MagnetSendData(Magnet_Send_Str.SendBuf,Magnet_Send_Str.SendLen);
	}
	else
	{
		if(Magnet_Resend_Timer_Index != -1)
			Free_Timer(&Magnet_Resend_Timer_Index);
	}
	return 0;
}

/****************************************************************************
* 名    称：MagneticSendCommData
* 功    能：外部接口，发送数据
* 参    数：*data 传入数据首地址，len数据长度
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetSendCommData(unsigned char *data,unsigned short len)
{
	unsigned short crcValue;
	uint32_t tmp=0;

  memset(&Magnet_Send_Str, 0x0, sizeof(Magnet_Send_Str));

  Magnet_Send_Str.SendBuf[0] = FRAME_HADE;
  memcpy(&Magnet_Send_Str.SendBuf[1],data,len);
  crcValue=CalcCRC((uint8_t*)&Magnet_Send_Str.SendBuf[1],len,Magnet_Send_Str.CommPwd);
  Magnet_Send_Str.SendBuf[len+1]=crcValue>>8;
  Magnet_Send_Str.SendBuf[len+2]=crcValue&0x00FF;
  Magnet_Send_Str.SendBuf[len+3]=FRAME_END;
  Magnet_Send_Str.SendLen =len + 4;
  MagnetSendData(Magnet_Send_Str.SendBuf,Magnet_Send_Str.SendLen);
  Magnet_Send_Str.ReSendCount =0;
	
	if(Magnet_Resend_Timer_Index != -1)
		Free_Timer(&Magnet_Resend_Timer_Index);
	Malloc_Timer(500,Auto_Mode,MagnetSendTimeout,NULL,&Magnet_Resend_Timer_Index);
}

/****************************************************************************
* 名    称：MagnetSendACK
* 功    能：收到数据后，做出的应答
* 参    数：flag 应答是否正常的标志
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetSendACK(unsigned char flag)
{
	uint8_t MagneticAckBuf[2];
	
	MagneticAckBuf[0]=CMD_MAGNET_ACK;
  MagneticAckBuf[1]=flag;
	MagnetSendCommData(&MagneticAckBuf[0],2);
}
/****************************************************************************
* 名    称：MagnetReport
* 功    能：初始化导航器,主动上报数据
* 参    数：flag:1:正常模式2:调试模式
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetReport(uint8_t flag)
{
	uint8_t buf[2];
	buf[0]=0xD1;
	buf[1] = flag;
	MagnetSendCommData(&buf[0],2);
}
/****************************************************************************
* 名    称：MagnetRecvProcess
* 功    能：串口收到数据处理过程
* 参    数：*data 传入数据首地址，len数据长度
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetRecvProcess(unsigned char *data,unsigned short len,MagnetCB_FuncPtr_TypeDef MagnetCB)
{
    unsigned short crcValue,checkValue;
    if((data[0]==FRAME_HADE) && (data[len-1]==FRAME_END))
    {
        crcValue=CalcCRC(&data[1],len-4,Magnet_Send_Str.CommPwd);
        checkValue=data[len-3]<<8 | data[len-2];
        if(crcValue == checkValue)
        {
					if(data[CMD_POSTTION] == CMD_MAGNET_ACK)
					{
						//MagneticSendACK(ACK_PASS);
					}else
					{
						if(Magnet_Resend_Timer_Index != -1)
							Free_Timer(&Magnet_Resend_Timer_Index);
						MagnetCB((char*)data,len);
					}
						
//          switch(data[CMD_POSTTION])
//            {
//						case CMD_MAGNET_CHECK_DATA:
//						{
//							MagneticCheckValueProcess((data[DATA_START_POSITION]<<8) | data[DATA_START_POSITION+1]);
//							break;
//						}
//						case CMD_MAGNET_RFID_CARD:
//						{
//							MagnetSendACK(ACK_PASS);
//							ReadRFIDProcess(&data[DATA_START_POSITION]);
//							break;
//						}
//            }

        } 
				else
        {
//            MagneticSendACK(ACK_ERROR);
        }
    }
		else
    {
//        MagneticSendACK(ACK_ERROR);
    }

}
/****************************************************************************
* 名    称：MagnetRecvCheck
* 功    能：检查串口是否收到数据
* 参    数：数据有效后调用的处理函数
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetRecvCheck(MagnetCB_FuncPtr_TypeDef MagnetCB)
{
	if(UART1_Str.Recv_Flag)
	{
		UART1_Str.Recv_Flag = 0;
		USART2_Send_Str(UART1_Str.data,UART1_Str.Recv_Len);     //调试用，打印串口1数据到PC
		MagnetRecvProcess((uint8_t*)UART1_Str.data,UART1_Str.Recv_Len,MagnetCB);
		memset(UART1_Str.data,0x00,sizeof(UART1_Str.Recv_Len));
	}
}

