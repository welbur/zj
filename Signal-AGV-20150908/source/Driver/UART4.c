
#include "Global_Config.h"
#include "UART4.h"

#ifdef UART4_PRINTF_ENABLE
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
  USART_SendData(UART4, (uint8_t) ch);
	/* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
  {}
  return ch;
}	
#endif

char UART4_Recv_Buf[UART4_MAX_ARR][UART4_MAX_RECV_LEN];/*定义N个缓冲区用于轮流接收串口底层协议*/
u8 UART4_User_Buf_No = 0;                            /*当前正在使用的缓冲区*/

_Recv_Str UART4_Str;
DMA_InitTypeDef UART4_DMA_TX,UART4_DMA_RX;


/*
*********************************************************************************************************
*	函 数 名: USART4_Init
*	功能说明: 初始化CPU的USART4串口硬件设备。用于与LCM模块通信,在portserial中设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART4_Init(_UART_BAUD BaudRate)
{
	USART_InitTypeDef USART_InitStructure;   
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	
	RCC_AHB1PeriphClockCmd(USART4_RX_PORT_CLK | USART4_TX_PORT_CLK , ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/* 配置发送管脚*/
  GPIO_PinAFConfig(USART4_RX_PORT, USART4_RX_SOURCE, GPIO_AF_UART4);

  /*配置接收管脚*/
  GPIO_PinAFConfig(USART4_TX_PORT, USART4_TX_SOURCE, GPIO_AF_UART4);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_InitStructure.GPIO_Pin = USART4_RX_PIN;  
  GPIO_Init(USART4_RX_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = USART4_TX_PIN;
  GPIO_Init(USART4_TX_PORT, &GPIO_InitStructure);
	

       
  /* USARTx configured as follow:
        - BaudRate = 19200 baud  
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
	USART_Init(UART4, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	#if UART4_DMA_RX_ENABLE
	/*空闲中断*/
  USART_ITConfig(UART4, USART_IT_IDLE , ENABLE);
  #else
	USART_ITConfig(UART4, USART_IT_RXNE | USART_IT_IDLE , ENABLE);
	#endif
  	
	/* Enable USART */
  USART_Cmd(UART4, ENABLE);
  
  /* 
		CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题：
	 	清发送完成标志，Transmission Complete flag 
	*/
	USART_ClearFlag(UART4, USART_FLAG_TC); 
	
	
	memset((u8*)&UART4_Str,0x00,sizeof(UART4_Str));
	#if UART4_DMA_RX_ENABLE
	  USART4_RX_DMA();
	#endif
	#if UART4_DMA_TX_ENABLE
		USART4_TX_DMA();
		UART4_Str.Send_Finish = 1;
	#endif
   
}
/*
*********************************************************************************************************
*	函 数 名: USART4_CLK_GPIO_Set
*	功能说明: 设置串口3时钟和GPIO
*	形    参：ENABLE:使能串口3及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART4_CLK_GPIO_Set(FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (NewState != DISABLE)
	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//		
//		/* 配置发送管脚*/
//		GPIO_PinAFConfig(USART4_TX_PORT, USART4_TX_SOURCE, GPIO_AF_UART4);

//		/*配置接收管脚*/
//		GPIO_PinAFConfig(USART4_RX_PORT, USART4_RX_SOURCE, GPIO_AF_UART4);

		/* Configure USART Tx as alternate function  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_Pin = USART4_TX_PIN;  
		GPIO_Init(USART4_TX_PORT, &GPIO_InitStructure);

		/* Configure USART Rx as alternate function  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = USART4_RX_PIN;
		GPIO_Init(USART4_RX_PORT, &GPIO_InitStructure);
		
		USART_Cmd(UART4, ENABLE);
	}
	else
	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4, DISABLE);

		USART_Cmd(UART4, DISABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = USART4_TX_PIN;
    GPIO_Init(USART4_TX_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = USART4_RX_PIN;
		GPIO_Init(USART4_RX_PORT, &GPIO_InitStructure);
		
		
	}
}



#if UART4_DMA_RX_ENABLE
/*
*********************************************************************************************************
*	函 数 名: USART4_RX_DMA
*	功能说明: 设置USART4 DMA接收方式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART4_RX_DMA(void)
{
	//DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);                             //开DMA时钟
	  	 
	DMA_StructInit(&UART4_DMA_RX);

	UART4_DMA_RX.DMA_Channel = DMA_Channel_4;
	UART4_DMA_RX.DMA_PeripheralBaseAddr = (u32)&UART4->DR;
	UART4_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART4_Recv_Buf[UART4_User_Buf_No];
	UART4_DMA_RX.DMA_DIR = DMA_DIR_PeripheralToMemory;
	UART4_DMA_RX.DMA_BufferSize = UART4_MAX_RECV_LEN;
	UART4_DMA_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART4_DMA_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART4_DMA_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART4_DMA_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART4_DMA_RX.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	UART4_DMA_RX.DMA_Priority = DMA_Priority_Medium;
	UART4_DMA_RX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	UART4_DMA_RX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	UART4_DMA_RX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	UART4_DMA_RX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream2, &UART4_DMA_RX);
	 
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Stream2, ENABLE);


  DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);                                //DMA5传输完成中断
  DMA_ITConfig(DMA1_Stream2, DMA_IT_TE, ENABLE);


  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif

#if UART4_DMA_TX_ENABLE
/*
*********************************************************************************************************
*	函 数 名: USART4_TX_DMA
*	功能说明: 设置USART4 DMA发送方式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART4_TX_DMA(void)
{

	  NVIC_InitTypeDef   NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);                             //开DMA时钟
	  
	 DMA_DeInit(DMA1_Stream4);
	
	 DMA_StructInit(&UART4_DMA_TX);
	 UART4_DMA_TX.DMA_Channel = DMA_Channel_4;
	 UART4_DMA_TX.DMA_PeripheralBaseAddr = (u32)&UART4->DR;
	 /*需要发送的时候再改变地址和长度*/
	 //UART4_DMA_TX.DMA_Memory0BaseAddr = (u32)&UART4_Recv_Buf[UART4_User_Buf_No];
	 UART4_DMA_TX.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	 //UART4_DMA_TX.DMA_BufferSize = UART4_MAX_RECV_LEN;
	 UART4_DMA_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 UART4_DMA_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 UART4_DMA_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 UART4_DMA_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 UART4_DMA_TX.DMA_Mode = DMA_Mode_Normal;//*/DMA_Mode_Circular;
	 UART4_DMA_TX.DMA_Priority = DMA_Priority_High;
	 UART4_DMA_TX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 UART4_DMA_TX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	 UART4_DMA_TX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 UART4_DMA_TX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 DMA_Init(DMA1_Stream4, &UART4_DMA_TX);
	 
	 USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
	 DMA_ITConfig(DMA1_Stream4, DMA_IT_TC | DMA_IT_TE, ENABLE);               //DMA2传输完成中断

	 DMA_Cmd(DMA1_Stream4,DISABLE);   /*需要发送的时候才打开使能*/
			
	  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
#endif
/*
*********************************************************************************************************
*	函 数 名: USART4_Send_Byte
*	功能说明: USART4发送一个字节
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
u8 USART4_Send_Byte(u8 ch)
{
	USART_SendData(UART4, ch);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET )
	  {
	  	
	  }
	return ch;
}

/*
*********************************************************************************************************
*	函 数 名: USART4_Send_Byte
*	功能说明: USART4发送串数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART4_Send_Str(char *buf,u16 len)
{
	#if UART4_DMA_TX_ENABLE
		//while(!UART4_Str.Send_Finish);
		UART4_Str.Send_Finish = 0;
	  DMA_Cmd(DMA1_Stream4,DISABLE);
    UART4_DMA_TX.DMA_Memory0BaseAddr = (u32)buf;
    UART4_DMA_TX.DMA_BufferSize = len;
 	  DMA_Init(DMA1_Stream4, &UART4_DMA_TX);
    DMA_Cmd(DMA1_Stream4,ENABLE);
	#else	
	u16 i;
	for(i = 0; i < len; i++)
	  {
	  	USART_SendData(UART4, *buf++);
	    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET )
	     {
	  	
	     }
	  }
	#endif	
}
void UART4_IRQHandler(void)
{
	u16 i;
	
 	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)//如果为空闲总线中断
    {
			#if UART4_DMA_RX_ENABLE
	    DMA_Cmd(DMA1_Stream2, DISABLE);//关闭DMA,防止处理其间有数据
  	  UART4_Str.Recv_Len = UART4_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA1_Stream2); 
			/*防止收发切换产生未知数据,一定要收到最少一帧数据长度才置位*/
			if(UART4_Str.Recv_Len )
			 {			
				 UART4_Str.data = UART4_Recv_Buf[UART4_User_Buf_No];
         UART4_Str.Recv_Flag = 1;		
				 UART4_User_Buf_No++;
				 if(UART4_User_Buf_No >= UART4_MAX_ARR)
					 UART4_User_Buf_No = 0;
			 }
			DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2 | DMA_FLAG_TCIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2);//清标志
			DMA_SetCurrDataCounter(DMA1_Stream2,UART4_MAX_RECV_LEN);//重装填
			UART4_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART4_Recv_Buf[UART4_User_Buf_No];
			DMA_Init(DMA1_Stream2, &UART4_DMA_RX); 
			DMA_Cmd(DMA1_Stream2, ENABLE);//处理完,重开DMA
			 
			#else
			UART4_Str.data = UART4_Recv_Buf[UART4_User_Buf_No];
      UART4_Str.Recv_Flag = 1;		
		  UART4_User_Buf_No++;
			if(UART4_User_Buf_No >= UART4_MAX_ARR)
				UART4_User_Buf_No = 0;
		 #endif
			//读SR后读DR清除Idle
			i = UART4->SR;
			i = UART4->DR;
	}
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//如果为接收中断
	{
		UART4_Recv_Buf[UART4_User_Buf_No][UART4_Str.Recv_Len] = USART1->DR;
		UART4_Str.Recv_Len++;
		if(UART4_Str.Recv_Len > UART4_MAX_RECV_LEN)
			UART4_Str.Recv_Len = 0;
	}	
	if(USART_GetITStatus(UART4, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
	{
		USART_ClearITPendingBit(UART4, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
  if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)
  {
   USART_ClearFlag(UART4,USART_FLAG_ORE); //读SR其实就是清除标志
   USART_ReceiveData(UART4);    //读DR
  }
}
/**
 * ISR for USART4 RX  DMA Stream Interrupt
 */
void DMA1_Stream2_IRQHandler()
{
    // Stream2 transfer complete interrupt?
    if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_TCIF2))
      {
        // clear pending interrupt
        DMA_ClearFlag(DMA1_Stream2,DMA_IT_TCIF2);
 
		    UART4_Str.Recv_Len = UART4_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA1_Stream2); 
				if(UART4_Str.Recv_Len > 0)
				 {			
					UART4_Str.data = UART4_Recv_Buf[UART4_User_Buf_No];
					UART4_Str.Recv_Flag = 1;		
					UART4_User_Buf_No++;
					if(UART4_User_Buf_No >= UART4_MAX_ARR)
						UART4_User_Buf_No = 0;	
				 }
				DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2 | DMA_FLAG_TCIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2);//清标志
				DMA_SetCurrDataCounter(DMA1_Stream2,UART4_MAX_RECV_LEN);//重装填

				UART4_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART4_Recv_Buf[UART4_User_Buf_No];
		    DMA_Init(DMA1_Stream2, &UART4_DMA_RX);
      }
   DMA_ClearFlag(DMA1_Stream2,DMA_IT_TEIF2 | DMA_FLAG_HTIF2);
 
}
/**
 * ISR for USART4 TX  DMA Stream Interrupt
 */
void DMA1_Stream4_IRQHandler()
{
    // Stream6 transfer complete interrupt?
    if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4))
      {
        // clear pending interrupt
        DMA_ClearFlag(DMA1_Stream4,DMA_IT_TCIF4);
 
	
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4 | DMA_FLAG_TCIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_HTIF4);//清标志
		DMA_Cmd(DMA1_Stream4,DISABLE);

		UART4_Str.Send_Finish = 1;
      }
   //DMA_ClearFlag(DMA1_Stream4,DMA_IT_TEIF4 | DMA_FLAG_TCIF4);
 
}



