#include "stm32f4xx.h"
#include "mb.h" 
#include "mbutils.h"



void ENTER_CRITICAL_SECTION( )//进入超临界 关总中断
{
	__set_PRIMASK(1);
}

void EXIT_CRITICAL_SECTION( )//退出超临界 开总中断
{
	__set_PRIMASK(0);
}
//实时电压值


unsigned short usRegHoldingBuf[REG_INPUT_NREGS] = {
	0x0000,	//AGV编号                                    
	0x0000,	//速度设定
	0x0000,	//AGV工作模式
	0x0000,	//目的站点停靠时间
	0x0000,	//P值
	0x0000,	//D值
	0x0000,	//电压校准值
	0x0000,	//备用2
	0x0000,	//当前速度
	0x0000,	//当前电量
	0x0000,	//错误信息码
	0x0000,	//AGV工作状态
	0x0000,	//上一站点卡号
	0x0000,	//离开上一站点的距离
	0x0000,	//下一站点卡号
	0x0000	//目的站点卡号
};
u16 usRegInputBuf[REG_HOLDING_NREGS]={0x5555,0x7002,0x6003,0x5004};
u8 ucRegCoilsBuf[REG_COILS_SIZE]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
u8 ucRegDiscreteBuf[REG_DISCRETE_SIZE]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

//u16 usRegInputBuf[8]={0x0000,0xfe02,0x1314,0x1516,0x1405,0x1506,0x1607,0x1708};
//u16 *usRegHoldingBuf=usRegInputBuf;	//一个测试用的 寄存器数组 地址0-7
//
//u8 REG_INPUT_START=0,REG_HOLDING_START=0;
//u8 REG_INPUT_NREGS=8,REG_HOLDING_NREGS=8;
//u8 usRegInputStart=0,usRegHoldingStart=0;
//
//u8 ucRegCoilsBuf[8]={0x00,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
//u8 REG_COILS_START=0,REG_DISCRETE_START=0;
//
//u8 ucRegDiscreteBuf[8]={0x00,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
//u8 REG_COILS_SIZE=0,REG_DISCRETE_SIZE=0;

//读数字寄存器 功能码0x04

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

    if( ( (int16_t)usAddress >= REG_INPUT_START )&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - REG_INPUT_START );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;

}
// 寄存器的读写函数 支持的命令为读 0x03 和写0x06

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
        eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
//	u16 *PRT=(u16*)pucRegBuffer;

    if( ( (int16_t)usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - REG_HOLDING_START );
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
//                *PRT++ = __REV16(usRegHoldingBuf[iRegIndex++]); //数据序转 REV16.W

                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
//                usRegHoldingBuf[iRegIndex++] = __REV16(*PRT++); //数据序转 REV16.W

                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


//读/写开关寄存器  0x01  x05

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
//    ( void )pucRegBuffer;
//    ( void )usAddress;
//    ( void )usNCoils;
//    ( void )eMode;
//    return MB_ENOREG;
    
    //错误状态
    eMBErrorCode eStatus = MB_ENOERR;
    //寄存器个数
    int16_t iNCoils = ( int16_t )usNCoils;
    //寄存器偏移量
    int16_t usBitOffset;
    
    //检查寄存器是否在指定范围内
    if( ( (int16_t)usAddress >= REG_COILS_START ) &&
    ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
    {
    //计算寄存器偏移量
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
    //读操作
    case MB_REG_READ:
    while( iNCoils > 0 )
    {
    *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
    ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
    iNCoils -= 8;
    usBitOffset += 8;
    }
    break;
    
    //写操作
    case MB_REG_WRITE:
    while( iNCoils > 0 )
    {
    xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
    ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
    *pucRegBuffer++ );
    iNCoils -= 8;
    }
    break;
    }
    
    }
    else
    {
    eStatus = MB_ENOREG;
    }
    return eStatus;
    
}

//读开关寄存器 0x02
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
//    ( void )pucRegBuffer;
//    ( void )usAddress;
//    ( void )usNDiscrete;
//    return MB_ENOREG;
  
    //错误状态
    eMBErrorCode eStatus = MB_ENOERR;
    //操作寄存器个数
    int16_t iNDiscrete = ( int16_t )usNDiscrete;
    //偏移量
    uint16_t usBitOffset;
    
    //判断寄存器时候再制定范围内
    if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
    ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
    {
    //获得偏移量
    usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );
    
    while( iNDiscrete > 0 )
    {
    *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
    ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
    iNDiscrete -= 8;
    usBitOffset += 8;
    }
    
    }
    else
    {
    eStatus = MB_ENOREG;
    }
    return eStatus;  
  
}
