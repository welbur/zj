

#include "UART1.h"

Uart_FuncPtr_TypeDef Uart1Callback; /*接收到数据后相应的处理函数*/



#ifdef UART1_PRINTF_ENABLE
#include <stdio.h>
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
	/* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}
  return ch;
}	
#endif

char UART1_Recv_Buf[UART1_MAX_ARR][UART1_MAX_RECV_LEN];/*定义N个缓冲区用于轮流接收串口底层协议*/
u8 UART1_User_Buf_No = 0;
_Recv_Str UART1_Str;

DMA_InitTypeDef UART1_DMA_TX,UART1_DMA_RX;

/*
*********************************************************************************************************
*	函 数 名: USART1_Init
*	功能说明: 初始化CPU的USART1串口硬件设备。用于和磁传感器通信
*	形    参：_YD_UART_BAUD:波特率;
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_Init(_UART_BAUD BaudRate)
{
	USART_InitTypeDef USART_InitStructure;   
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
//	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	RCC_AHB1PeriphClockCmd(USART1_TX_PORT_CLK | USART1_RX_PORT_CLK , ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* 配置发送管脚*/
  GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_SOURCE, GPIO_AF_USART1);

  /*配置接收管脚*/
  GPIO_PinAFConfig(USART1_RX_PORT, USART1_RX_SOURCE, GPIO_AF_USART1);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;  
  GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
  GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);
	

       
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	#if UART1_DMA_RX_ENABLE
	/*空闲中断*/
  USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);
  #else
	USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_IDLE , ENABLE);
	#endif
	/* Enable USART */
  USART_Cmd(USART1, ENABLE);
  
  /* 
		CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题：
	 	清发送完成标志，Transmission Complete flag 
	*/
	USART_ClearFlag(USART1, USART_FLAG_TC);  
	
	memset((u8*)&UART1_Str,0x00,sizeof(UART1_Str));
	#if UART1_DMA_RX_ENABLE
	  USART1_RX_DMA();
	#endif
	#if UART1_DMA_TX_ENABLE
		USART1_TX_DMA();
		UART1_Str.Send_Finish = 1;
	#endif
	
}	

/*
*********************************************************************************************************
*	函 数 名: USART1_CLK_GPIO_Set
*	功能说明: 设置串口1时钟和GPIO
*	形    参：ENABLE:使能串口1及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_CLK_GPIO_Set(FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (NewState != DISABLE)
	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//		
//		/* 配置发送管脚*/
//		GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_SOURCE, GPIO_AF_USART1);

//		/*配置接收管脚*/
//		GPIO_PinAFConfig(USART1_RX_PORT, USART1_RX_SOURCE, GPIO_AF_USART1);

		/* Configure USART Tx as alternate function  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;  
		GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

		/* Configure USART Rx as alternate function  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
		GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);
		
		USART_Cmd(USART1, ENABLE);
	}
	else
	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);

		USART_Cmd(USART1, DISABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
		GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);
		
		
	}
}



#if UART1_DMA_RX_ENABLE
/*
*********************************************************************************************************
*	函 数 名: USART1_RX_DMA
*	功能说明: 设置USART1 DMA接收方式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_RX_DMA(void)
{
	 //DMA_InitTypeDef UART1_DMA_RX;
	 NVIC_InitTypeDef   NVIC_InitStructure;

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                             //开DMA时钟
	  	 
	 DMA_StructInit(&UART1_DMA_RX);

	 UART1_DMA_RX.DMA_Channel = DMA_Channel_4;
	 UART1_DMA_RX.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	 UART1_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART1_Recv_Buf[UART1_User_Buf_No];
	 UART1_DMA_RX.DMA_DIR = DMA_DIR_PeripheralToMemory;
	 UART1_DMA_RX.DMA_BufferSize = UART1_MAX_RECV_LEN;
	 UART1_DMA_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 UART1_DMA_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 UART1_DMA_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 UART1_DMA_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 UART1_DMA_RX.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	 UART1_DMA_RX.DMA_Priority = DMA_Priority_Medium;
	 UART1_DMA_RX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 UART1_DMA_RX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	 UART1_DMA_RX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 UART1_DMA_RX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 DMA_Init(DMA2_Stream5, &UART1_DMA_RX);
	 
	 USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	 DMA_Cmd(DMA2_Stream5, ENABLE);


   DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);                                //DMA5传输完成中断
   DMA_ITConfig(DMA2_Stream5, DMA_IT_TE, ENABLE);


   NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_Init(&NVIC_InitStructure);
			

}
#endif
#if UART1_DMA_TX_ENABLE
/*
*********************************************************************************************************
*	函 数 名: USART1_TX_DMA
*	功能说明: 设置USART1 DMA发送方式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_TX_DMA(void)
{
	  //DMA_InitTypeDef DMA_InitStructure;
	  NVIC_InitTypeDef   NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                             //开DMA时钟
	  
	  DMA_StructInit(&UART1_DMA_TX);

	 UART1_DMA_TX.DMA_Channel = DMA_Channel_4;
	 UART1_DMA_TX.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
//	 UART1_DMA_TX.DMA_Memory0BaseAddr = (u32)&USART1_Rcv_Str.Buf[0];
	 UART1_DMA_TX.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	 UART1_DMA_TX.DMA_BufferSize = MAX_RCV_LEN;
	 UART1_DMA_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 UART1_DMA_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 UART1_DMA_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 UART1_DMA_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 UART1_DMA_TX.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	 UART1_DMA_TX.DMA_Priority = DMA_Priority_Medium;
	 UART1_DMA_TX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 UART1_DMA_TX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	 UART1_DMA_TX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 UART1_DMA_TX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 //DMA_DoubleBufferModeConfig ( DMA2_Stream7, (uint32_t)&USART1_Rcv_Str.Buf[1], DMA_Memory_0 );
   //DMA_DoubleBufferModeCmd ( DMA2_Stream7, ENABLE );  
	 DMA_Init(DMA2_Stream7, &UART1_DMA_TX);
	 
	 USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	 DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);               //DMA2传输完成中断
   DMA_ITConfig(DMA2_Stream7, DMA_IT_TE, ENABLE);
			
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
#endif

/*
*********************************************************************************************************
*	函 数 名: USART1_Send_Byte
*	功能说明: 设置USART1 发送一个字节
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
unsigned char USART1_Send_Byte(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t)ch); /*发送一个字符函数*/ 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)/*等待发送完成*/
  {
  
  }
  return ch;	
}
/*
*********************************************************************************************************
*	函 数 名: USART1_Send_Str
*	功能说明: 设置USART1 发送一串字符
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
unsigned char USART1_Send_Str(char *buf,uint16_t len)
{
	
	#if UART1_DMA_TX_ENABLE
	//while(!UART1_Str.Send_Finish);
	UART1_Str.Send_Finish = 0;
	
	DMA_Cmd(DMA2_Stream7, DISABLE);
	UART1_DMA_TX.DMA_Memory0BaseAddr = (u32)buf;
	UART1_DMA_TX.DMA_BufferSize = len;
	DMA_Init(DMA2_Stream7, &UART1_DMA_TX);
	DMA_Cmd(DMA2_Stream7, ENABLE);
	#else	
	 uint16_t i;

	 for(i =0; i < len; i++)
		{
		 USART_SendData(USART1, *buf++);
		 while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET )
			{
				
			}
		}
	#endif
return 0;	
}

void USART1_IRQHandler(void)
{
	u16 i;
 	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//如果为空闲总线中断
    {
			#if UART1_DMA_RX_ENABLE
	    DMA_Cmd(DMA2_Stream5, DISABLE);//关闭DMA,防止处理其间有数据
  	  UART1_Str.Recv_Len = UART1_MAX_RECV_LEN - DMA_GetCurrDataCounter(DMA2_Stream5); 
	    if(UART1_Str.Recv_Len > 0)
       {			
				 UART1_Str.data = UART1_Recv_Buf[UART1_User_Buf_No];
         UART1_Str.Recv_Flag = 1;		
				 UART1_User_Buf_No++;
				 if(UART1_User_Buf_No >= UART1_MAX_ARR)
					 UART1_User_Buf_No = 0;
				 //Uart1Callback((void*)&UART1_Str);
		   }
			DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清标志
			DMA_SetCurrDataCounter(DMA2_Stream5,UART1_MAX_RECV_LEN);//重装填
			UART1_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART1_Recv_Buf[UART1_User_Buf_No];
			DMA_Init(DMA2_Stream5, &UART1_DMA_RX);	
			//DMA_SetCurrDataCounter(DMA2_Stream2,MAX_RCV_LEN);//重装填
			DMA_Cmd(DMA2_Stream5, ENABLE);//处理完,重开DMA
			 
			#else
			 UART1_Str.data = UART1_Recv_Buf[UART1_Str.User_Buf_No];
			 UART1_Str.Recv_Flag = 1;
			 UART1_Str.User_Buf_No++;
			 if(UART1_Str.User_Buf_No >= UART_MAX_ARR)
				 UART1_Str.User_Buf_No = 0;
			#endif
			//读SR后读DR清除Idle
			i = USART1->SR;
			i = USART1->DR;
			 
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//如果为接收中断
	{
		UART1_Recv_Buf[UART1_User_Buf_No][UART1_Str.Recv_Len] = USART1->DR;
		UART1_Str.Recv_Len++;
		if(UART1_Str.Recv_Len > UART1_MAX_RECV_LEN)
			UART1_Str.Recv_Len = 0;
	}
	if(USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
	{
		USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
  	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
}

/**
 * ISR for USART1 RX  DMA Stream Interrupt
 */
void DMA2_Stream5_IRQHandler()
{
    // Stream2 transfer complete interrupt?
    if(DMA_GetITStatus(DMA2_Stream5,DMA_IT_TCIF5))
    {
        // clear pending interrupt
        DMA_ClearFlag(DMA2_Stream5,DMA_IT_TCIF5);
 
		    UART1_Str.Recv_Len = UART1_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA2_Stream5); 
				if(UART1_Str.Recv_Len > 0)
				 {			
					UART1_Str.data = UART1_Recv_Buf[UART1_User_Buf_No];
					UART1_Str.Recv_Flag = 1;		
					UART1_User_Buf_No++;
					if(UART1_User_Buf_No >= UART1_MAX_ARR)
						UART1_User_Buf_No = 0;
					//Uart1Callback((void*)&UART1_Str);
				 }
				DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清标志
				DMA_SetCurrDataCounter(DMA2_Stream5,UART1_MAX_RECV_LEN);//重装填
				//USART1_Rcv_Str.Free_Buf_No = DMA_GetCurrentMemoryTarget(DMA2_Stream2);

				UART1_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART1_Recv_Buf[UART1_User_Buf_No];
		    DMA_Init(DMA2_Stream5, &UART1_DMA_RX);	
		 }
     DMA_ClearFlag(DMA2_Stream5,DMA_IT_TEIF5 | DMA_FLAG_HTIF5);
 
}
/**
 * ISR for USART1 TX  DMA Stream Interrupt
 */
 void DMA2_Stream7_IRQHandler()
 {
     // Stream2 transfer complete interrupt?
     if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7))
       {
         // clear pending interrupt
         DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);
				 UART1_Str.Send_Finish = 1;
				 DMA_Cmd(DMA2_Stream7, DISABLE);
  
       }
 	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7 | DMA_FLAG_TCIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7);//清标志		
    DMA_ClearFlag(DMA2_Stream7,DMA_IT_TEIF7);
  
 }



 