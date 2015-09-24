/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "stm32f4xx.h"
#include "UART4.h"
//#include "stm32f10x_conf.h"
/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );
//extern void USART2_Configuration(void);

GPIO_InitTypeDef   GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
  	if(xRxEnable)
	{
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);	
	}

	if(xTxEnable)
	{
		USART_ITConfig(UART4, USART_IT_TC, ENABLE);
	}
	else
	{
	   USART_ITConfig(UART4, USART_IT_TC, DISABLE);
	}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure; 
    
    
    (void)ucPORT; //不修改串口
    (void)ucDataBits; //不修改数据位长度
    (void)eParity; //不修改校验格式
   
  
  

  RCC_AHB1PeriphClockCmd(USART4_RX_PORT_CLK,ENABLE);
  RCC_AHB1PeriphClockCmd(USART4_TX_PORT_CLK,ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);


  GPIO_PinAFConfig(USART4_TX_PORT, USART4_TX_SOURCE, GPIO_AF_UART4);
  GPIO_PinAFConfig(USART4_RX_PORT, USART4_RX_SOURCE, GPIO_AF_UART4);


  GPIO_InitStructure.GPIO_Pin = USART4_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(USART4_TX_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = USART4_RX_PIN;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(USART4_RX_PORT, &GPIO_InitStructure);

/*
   		 USARTx configured as follow:
         - BaudRate = 115200 baud  
		 - Word Length = 8 Bits
         - One Stop Bit
         - No parity
         - Hardware flow control disabled (RTS and CTS signals)
         - Receive and transmit    
 */

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStructure);
  /* Enable the Open_USART Transmit interrupt: this interrupt is generated when the 
     Open_USART transmit data register is empty */
  //USART_ITConfig(Open_USART,USART_IT_RXNE,ENABLE);

  USART_Cmd(UART4, ENABLE);

//  DMA_InitTypeDef DMA_InitStructure;
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
//  DMA_DeInit(DMA2_Stream5);
//  DMA_StructInit(&DMA_InitStructure);
//  DMA_InitStructure.DMA_Channel = DMA_Channel_4; //选择Channel_4
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR)); //数据传输的外设首地址，详解见上
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)VoltageBuffer; //自己定义待发送数组的首地址，要强制转换为32位
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //数据传输方向选择为内存->外设
//  DMA_InitStructure.DMA_BufferSize = 2; //传输数据大小为8，单位由以下确定，大小要配合定义的数组类型和外设数据类型
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR 数据寄存器
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增允许，因为要读取一个数组
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR 数据寄存器为8为，故选Byte
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //这里也选Byte
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMA 传输模式为Normal，如果为Circular,将会循环传输
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级为High
//  /*双缓冲模式，在DMA_Init 之前调用在Circular 模式有效，会强制Circular，
//  *不支持Memory toMemory，(uint32_t)Buffer2为DMA_Memory_1，DMA 先将Buffer
//  *中的数据发送完毕后在发送Buffer2的数据，当然顺序可以改变
//  DMA_DoubleBufferModeConfig(DMA2_Stream6, (uint32_t)Buffer2, DMA_Memory_0);
//  DMA_DoubleBufferModeCmd(DMA2_Stream6, ENABLE);
//  */
//  //DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE); 
//  DMA_Init(DMA2_Stream5, &DMA_InitStructure);
//  DMA_Cmd(DMA2_Stream5, ENABLE); //DMA2_Stream5
//  /*DMA 中断开*/
//  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

  

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    USART_SendData(UART4, ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    *pucByte = USART_ReceiveData(UART4);
    return TRUE;
}
void UART4_IRQHandler(void)																										//
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		prvvUARTRxISR();
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		prvvUARTTxReadyISR();
		USART_ClearITPendingBit(UART4, USART_IT_TC);
	}
}
/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
