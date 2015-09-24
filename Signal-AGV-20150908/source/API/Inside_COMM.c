
#include "Inside_COMM.h"
#include "crc.h"

_Comm_Send_Str Inside_Send_Str;
static int8_t Inside_Resend_Timer_Index = -1;

/****************************************************************************
* ��    �ƣ�InsideCommInit
* ��    �ܣ�������ذ�ӿڣ���ʼ��ͨ��
* ��    ������
* ��    �أ���
* ˵    ����
****************************************************************************/
void InsideCommInit(void)
{
	USART2_Init(BAUD_115200);
	memset((char*)&Inside_Send_Str,0x00,sizeof(Inside_Send_Str));
	Inside_Send_Str.CommPwd = 0;
}
/****************************************************************************
* ��    �ƣ�InsideSendData
* ��    �ܣ��������ݸ����ذ�
* ��    ������
* ��    �أ���
* ˵    ����
****************************************************************************/
void InsideSendData(char *data,uint16_t len)
{
	USART2_Send_Str(data,len);
}
/****************************************************************************
* ��    �ƣ�InsideSendTimeout
* ��    �ܣ��������ݸ��Ŵ�������ʱ����
* ��    ������
* ��    �أ���
* ˵    ����
****************************************************************************/
int8_t InsideSendTimeout(void *cbptr)
{
	cbptr = cbptr;
	if(Inside_Send_Str.ReSendCount < INSIDE_RESEND_MAX_NUM)
	{
		Inside_Send_Str.ReSendCount++;
		InsideSendData(Inside_Send_Str.SendBuf,Inside_Send_Str.SendLen);
	}
	else
	{
		if(Inside_Resend_Timer_Index != -1)
			Free_Timer(&Inside_Resend_Timer_Index);
	}
	return 0;
}
/****************************************************************************
* ��    �ƣ�InsideSendCommData
* ��    �ܣ��ⲿ�ӿڣ���������
* ��    ����*data ���������׵�ַ��len���ݳ���
* ��    �أ���
* ˵    ����
****************************************************************************/
void InsideSendCommData(unsigned char *data,unsigned short len)
{
    unsigned short crcValue;
		uint32_t tmp=0;

    Inside_Send_Str.SendBuf[0]=FRAME_HADE;
    memcpy(&Inside_Send_Str.SendBuf[1],data,len);
    crcValue=CalcCRC((uint8_t*)&Inside_Send_Str.SendBuf[1],len,Inside_Send_Str.CommPwd);
    Inside_Send_Str.SendBuf[len+1]=crcValue>>8;
    Inside_Send_Str.SendBuf[len+2]=crcValue&0x00FF;
    Inside_Send_Str.SendBuf[len+3]=FRAME_END;
    Inside_Send_Str.SendLen = len + 4;
    InsideSendData(&Inside_Send_Str.SendBuf[0],Inside_Send_Str.SendLen);
    if(Inside_Resend_Timer_Index != -1)
			Free_Timer(&Inside_Resend_Timer_Index);
		Malloc_Timer(500,Auto_Mode,InsideSendTimeout,NULL,&Inside_Resend_Timer_Index);
}
/****************************************************************************
* ��    �ƣ�void SendACK(unsigned char cmd)
* ��    �ܣ��յ����ݺ�������Ӧ��
* ��    ����flag Ӧ���Ƿ������ı�־
* ��    �أ���
* ˵    ����
****************************************************************************/
void InsideSendACK(unsigned char flag)
{
	uint8_t AckBuf[2];
	AckBuf[0]=CMD_DRIVER_ACK;
  AckBuf[1]=flag;
	InsideSendCommData(&AckBuf[0],2);
}
/****************************************************************************
* ��    �ƣ�InsideRecvProcess(void)
* ��    �ܣ������յ����ݴ������
* ��    ����*data ���������׵�ַ��len���ݳ���
* ��    �أ���
* ˵    ����
****************************************************************************/
void InsideRecvProcess(unsigned char *data,unsigned short len,InsideCB_FuncPtr_TypeDef InsideCB)
{
    unsigned short crcValue,checkValue;
    if((data[0]==FRAME_HADE) && (data[len-1]==FRAME_END))
    {
        crcValue = CalcCRC(&data[1],len-4,Inside_Send_Str.CommPwd);
        checkValue = data[len-3]<<8 | data[len-2];
        if(crcValue == checkValue)
        {
          if(data[1] != CMD_DRIVER_ACK)
						{
							InsideSendACK(ACK_PASS);
						}
					else
						{
							if(Inside_Resend_Timer_Index != -1)
								Free_Timer(&Inside_Resend_Timer_Index);
							InsideCB((char*)data,len);
						}
                
//          switch(data[1])
//            {
//						case CMD_DRIVER_INIT_PARAMTER:
//							{
//							InitCarProcess(&data[DATA_START_POSITION]);
//							break;
//							}
//						case CMD_DRIVER_RUN_ACTION:
//							{
//							GoActionProcess((CAR_ACTION)data[DATA_START_POSITION]);
//							break;
//							}		
//            }

        } 
				else
        {
            InsideSendACK(ACK_ERROR);
        }
    } 
		else
    {
        InsideSendACK(ACK_ERROR);
    }

}
/****************************************************************************
* ��    �ƣ�InsideRecvCheck
* ��    �ܣ���鴮���Ƿ��յ�����
* ��    ����������Ч����õĴ�����
* ��    �أ���
* ˵    ����
****************************************************************************/
void InsideRecvCheck(InsideCB_FuncPtr_TypeDef InsideCB)
{
	if(UART2_Str.Recv_Flag)
	{
		UART2_Str.Recv_Flag = 0;
		InsideRecvProcess((uint8_t*)UART2_Str.data,UART2_Str.Recv_Len,InsideCB);
	}
}




