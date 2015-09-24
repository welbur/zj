

#include "UART2.h"

#ifdef UART2_PRINTF_ENABLE
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
  USART_SendData(USART2, (uint8_t) ch);
	/* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {}
  return ch;
}	
#endif

char UART2_Recv_Buf[UART2_MAX_ARR][UART2_MAX_RECV_LEN];/*����N�������������������մ��ڵײ�Э��*/
u8 UART2_User_Buf_No = 0;                            /*��ǰ����ʹ�õĻ�����*/

_Recv_Str UART2_Str;
DMA_InitTypeDef UART2_DMA_TX,UART2_DMA_RX;

/*
*********************************************************************************************************
*	�� �� ��: USART2_Init
*	����˵��: ��ʼ��CPU��USART2����Ӳ���豸������ͨ�Ű�ͨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_Init(_UART_BAUD BaudRate)
{
	USART_InitTypeDef USART_InitStructure;   
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	
	RCC_AHB1PeriphClockCmd(USART2_RX_PORT_CLK | USART2_TX_PORT_CLK , ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* ���÷��͹ܽ�*/
  GPIO_PinAFConfig(USART2_RX_PORT, USART2_RX_SOURCE, GPIO_AF_USART2);

  /*���ý��չܽ�*/
  GPIO_PinAFConfig(USART2_TX_PORT, USART2_TX_SOURCE, GPIO_AF_USART2);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;  
  GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
  GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
	

       
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
	USART_Init(USART2, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	#if UART2_DMA_RX_ENABLE
	/*�����ж�*/
  USART_ITConfig(USART2, USART_IT_IDLE , ENABLE);
	#else
	/*�����ж�*/
  USART_ITConfig(USART2, USART_IT_RXNE | USART_IT_IDLE , ENABLE);
	#endif
  	
	/* Enable USART */
  USART_Cmd(USART2, ENABLE);
  
  /* 
		CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
	 	�巢����ɱ�־��Transmission Complete flag 
	*/
	USART_ClearFlag(USART2, USART_FLAG_TC); 
	
	memset((u8*)&UART2_Str,0x00,sizeof(UART2_Str));
	#if UART2_DMA_RX_ENABLE
	  USART2_RX_DMA();
	#endif
	#if UART1_DMA_TX_ENABLE
		USART2_TX_DMA();
		UART2_Str.Send_Finish = 1;
	#endif
}

/*
*********************************************************************************************************
*	�� �� ��: USART2_CLK_GPIO_Set
*	����˵��: ���ô���2ʱ�Ӻ�GPIO
*	��    �Σ�ENABLE:ʹ�ܴ���2����Ӧʱ����GPIO����;DISABLE:ʧЧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_CLK_GPIO_Set(FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (NewState != DISABLE)
	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	
//		/* ���÷��͹ܽ�*/
//		GPIO_PinAFConfig(USART2_RX_PORT, USART2_RX_SOURCE, GPIO_AF_USART2);

//		/*���ý��չܽ�*/
//		GPIO_PinAFConfig(USART2_TX_PORT, USART2_TX_SOURCE, GPIO_AF_USART2);

		/* Configure USART Tx as alternate function  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;  
		GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

		/* Configure USART Rx as alternate function  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
		GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
		
		USART_Cmd(USART2, ENABLE);
	}
	else
	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);

		USART_Cmd(USART2, DISABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
    GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
		GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);
		
	}
}

#if UART2_DMA_RX_ENABLE
/*
*********************************************************************************************************
*	�� �� ��: USART1_RX_DMA
*	����˵��: ����USART1 DMA���շ�ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_RX_DMA(void)
{
	//DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);                             //��DMAʱ��
	  	 
	DMA_StructInit(&UART2_DMA_RX);

	UART2_DMA_RX.DMA_Channel = DMA_Channel_4;
	UART2_DMA_RX.DMA_PeripheralBaseAddr = (u32)&USART2->DR;
	UART2_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART2_Recv_Buf[UART2_User_Buf_No];
	UART2_DMA_RX.DMA_DIR = DMA_DIR_PeripheralToMemory;
	UART2_DMA_RX.DMA_BufferSize = UART2_MAX_RECV_LEN;
	UART2_DMA_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART2_DMA_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART2_DMA_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART2_DMA_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART2_DMA_RX.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	UART2_DMA_RX.DMA_Priority = DMA_Priority_Medium;
	UART2_DMA_RX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	UART2_DMA_RX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	UART2_DMA_RX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	UART2_DMA_RX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &UART2_DMA_RX);
	 
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Stream5, ENABLE);


  DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);                                //DMA5��������ж�
  DMA_ITConfig(DMA1_Stream5, DMA_IT_TE, ENABLE);


  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif
#if UART2_DMA_TX_ENABLE
/*
*********************************************************************************************************
*	�� �� ��: USART2_TX_DMA
*	����˵��: ����USART2 DMA���ͷ�ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_TX_DMA(void)
{
	  //DMA_InitTypeDef DMA_InitStructure;
	  NVIC_InitTypeDef   NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);                             //��DMAʱ��
	  
	 DMA_DeInit(DMA1_Stream6);
	
	 DMA_StructInit(&UART2_DMA_TX);
	 UART2_DMA_TX.DMA_Channel = DMA_Channel_4;
	 UART2_DMA_TX.DMA_PeripheralBaseAddr = (u32)&USART2->DR;
	 /*��Ҫ���͵�ʱ���ٸı��ַ�ͳ���*/
	 //UART2_DMA_TX.DMA_Memory0BaseAddr = (u32)&UART2_Recv_Buf[UART2_User_Buf_No];
	 UART2_DMA_TX.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	 //UART2_DMA_TX.DMA_BufferSize = UART2_MAX_RECV_LEN;
	 UART2_DMA_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 UART2_DMA_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 UART2_DMA_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 UART2_DMA_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 UART2_DMA_TX.DMA_Mode = DMA_Mode_Normal;//*/DMA_Mode_Circular;
	 UART2_DMA_TX.DMA_Priority = DMA_Priority_High;
	 UART2_DMA_TX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 UART2_DMA_TX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	 UART2_DMA_TX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 UART2_DMA_TX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 DMA_Init(DMA1_Stream6, &UART2_DMA_TX);
	 
	 USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	 DMA_ITConfig(DMA1_Stream6, DMA_IT_TC | DMA_IT_TE, ENABLE);               //DMA2��������ж�

	 DMA_Cmd(DMA1_Stream6,DISABLE);   /*��Ҫ���͵�ʱ��Ŵ�ʹ��*/
			
	  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/*
*********************************************************************************************************
*	�� �� ��: USART2_Send_Byte
*	����˵��: USART2����һ���ֽ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 USART2_Send_Byte(u8 ch)
{
	USART_SendData(USART2, ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET )
	  {
	  	
	  }
	return ch;
}

/*
*********************************************************************************************************
*	�� �� ��: USART2_Send_Byte
*	����˵��: USART2���ʹ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_Send_Str(char *buf,u16 len)
{
	#if UART2_DMA_TX_ENABLE
		//while(!UART2_Str.Send_Finish);
		UART2_Str.Send_Finish = 0;
	  DMA_Cmd(DMA1_Stream6,DISABLE);
    UART2_DMA_TX.DMA_Memory0BaseAddr = (u32)buf;
    UART2_DMA_TX.DMA_BufferSize = len;
 	  DMA_Init(DMA1_Stream6, &UART2_DMA_TX);
    DMA_Cmd(DMA1_Stream6,ENABLE);
	#else	
	u16 i;
	for(i = 0; i < len; i++)
	  {
	  	USART_SendData(USART2, *buf++);
	    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET )
	     {
	  	
	     }
	  }
	#endif	
}



void USART2_IRQHandler(void)
{
	u16 i;
	
 	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//���Ϊ���������ж�
    {
			#if UART2_DMA_RX_ENABLE
	    DMA_Cmd(DMA1_Stream5, DISABLE);//�ر�DMA,��ֹ�������������
  	  UART2_Str.Recv_Len = UART2_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA1_Stream5); 
			/*��ֹ�շ��л�����δ֪����,һ��Ҫ�յ�����һ֡���ݳ��Ȳ���λ*/
			if(UART2_Str.Recv_Len )
			 {			
				 UART2_Str.data = UART2_Recv_Buf[UART2_User_Buf_No];
         UART2_Str.Recv_Flag = 1;		
				 UART2_User_Buf_No++;
				 if(UART2_User_Buf_No >= UART2_MAX_ARR)
					 UART2_User_Buf_No = 0;
			 }
			DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���־
			DMA_SetCurrDataCounter(DMA1_Stream5,UART2_MAX_RECV_LEN);//��װ��
			UART2_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART2_Recv_Buf[UART2_User_Buf_No];
			DMA_Init(DMA1_Stream5, &UART2_DMA_RX); 
			DMA_Cmd(DMA1_Stream5, ENABLE);//������,�ؿ�DMA
			 
			#else
			UART2_Str.data = UART2_Recv_Buf[UART2_User_Buf_No];
      UART2_Str.Recv_Flag = 1;		
		  UART2_User_Buf_No++;
			if(UART2_User_Buf_No >= UART2_MAX_ARR)
				UART2_User_Buf_No = 0;
		 #endif
			//��SR���DR���Idle
			i = USART2->SR;
			i = USART2->DR;
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���Ϊ�����ж�
	{
		UART2_Recv_Buf[UART2_User_Buf_No][UART2_Str.Recv_Len] = USART1->DR;
		UART2_Str.Recv_Len++;
		if(UART2_Str.Recv_Len > UART2_MAX_RECV_LEN)
			UART2_Str.Recv_Len = 0;
	}	
	if(USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//����
	{
		USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
  {
   USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR��ʵ���������־
   USART_ReceiveData(USART2);    //��DR
  }
}
/**
 * ISR for USART2 RX  DMA Stream Interrupt
 */
void DMA1_Stream5_IRQHandler()
{
    // Stream2 transfer complete interrupt?
    if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TCIF5))
      {
        // clear pending interrupt
        DMA_ClearFlag(DMA1_Stream5,DMA_IT_TCIF5);
 
		    UART2_Str.Recv_Len = UART2_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA1_Stream5); 
				if(UART2_Str.Recv_Len > 0)
				 {			
					UART2_Str.data = UART2_Recv_Buf[UART2_User_Buf_No];
					UART2_Str.Recv_Flag = 1;		
					UART2_User_Buf_No++;
					if(UART2_User_Buf_No >= UART2_MAX_ARR)
						UART2_User_Buf_No = 0;	
				 }
				DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���־
				DMA_SetCurrDataCounter(DMA1_Stream5,UART2_MAX_RECV_LEN);//��װ��

				UART2_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART2_Recv_Buf[UART2_User_Buf_No];
		    DMA_Init(DMA1_Stream5, &UART2_DMA_RX);
      }
   DMA_ClearFlag(DMA1_Stream5,DMA_IT_TEIF5 | DMA_FLAG_HTIF5);
 
}
/**
 * ISR for USART2 TX  DMA Stream Interrupt
 */
void DMA1_Stream6_IRQHandler()
{
    // Stream6 transfer complete interrupt?
    if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6))
      {
        // clear pending interrupt
        DMA_ClearFlag(DMA1_Stream6,DMA_IT_TCIF6);
 
	
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_DMEIF6 | DMA_FLAG_FEIF6 | DMA_FLAG_TCIF6 | DMA_FLAG_TEIF6 | DMA_FLAG_HTIF6);//���־
		DMA_Cmd(DMA1_Stream6,DISABLE);

		UART2_Str.Send_Finish = 1;
      }
   //DMA_ClearFlag(DMA1_Stream6,DMA_IT_TEIF6 | DMA_FLAG_TCIF6);
 
}

#endif
















