

#include "UART1.h"

Uart_FuncPtr_TypeDef Uart1Callback; /*���յ����ݺ���Ӧ�Ĵ�����*/



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

char UART1_Recv_Buf[UART1_MAX_ARR][UART1_MAX_RECV_LEN];/*����N�������������������մ��ڵײ�Э��*/
u8 UART1_User_Buf_No = 0;
_Recv_Str UART1_Str;

DMA_InitTypeDef UART1_DMA_TX,UART1_DMA_RX;

/*
*********************************************************************************************************
*	�� �� ��: USART1_Init
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸�����ںʹŴ�����ͨ��
*	��    �Σ�_YD_UART_BAUD:������;
*	�� �� ֵ: ��
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
	
	/* ���÷��͹ܽ�*/
  GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_SOURCE, GPIO_AF_USART1);

  /*���ý��չܽ�*/
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
	/*�����ж�*/
  USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);
  #else
	USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_IDLE , ENABLE);
	#endif
	/* Enable USART */
  USART_Cmd(USART1, ENABLE);
  
  /* 
		CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
	 	�巢����ɱ�־��Transmission Complete flag 
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
*	�� �� ��: USART1_CLK_GPIO_Set
*	����˵��: ���ô���1ʱ�Ӻ�GPIO
*	��    �Σ�ENABLE:ʹ�ܴ���1����Ӧʱ����GPIO����;DISABLE:ʧЧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_CLK_GPIO_Set(FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (NewState != DISABLE)
	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//		
//		/* ���÷��͹ܽ�*/
//		GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_SOURCE, GPIO_AF_USART1);

//		/*���ý��չܽ�*/
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
*	�� �� ��: USART1_RX_DMA
*	����˵��: ����USART1 DMA���շ�ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_RX_DMA(void)
{
	 //DMA_InitTypeDef UART1_DMA_RX;
	 NVIC_InitTypeDef   NVIC_InitStructure;

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                             //��DMAʱ��
	  	 
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


   DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);                                //DMA5��������ж�
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
*	�� �� ��: USART1_TX_DMA
*	����˵��: ����USART1 DMA���ͷ�ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_TX_DMA(void)
{
	  //DMA_InitTypeDef DMA_InitStructure;
	  NVIC_InitTypeDef   NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                             //��DMAʱ��
	  
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
	 DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);               //DMA2��������ж�
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
*	�� �� ��: USART1_Send_Byte
*	����˵��: ����USART1 ����һ���ֽ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char USART1_Send_Byte(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t)ch); /*����һ���ַ�����*/ 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)/*�ȴ��������*/
  {
  
  }
  return ch;	
}
/*
*********************************************************************************************************
*	�� �� ��: USART1_Send_Str
*	����˵��: ����USART1 ����һ���ַ�
*	��    �Σ���
*	�� �� ֵ: ��
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
 	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//���Ϊ���������ж�
    {
			#if UART1_DMA_RX_ENABLE
	    DMA_Cmd(DMA2_Stream5, DISABLE);//�ر�DMA,��ֹ�������������
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
			DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���־
			DMA_SetCurrDataCounter(DMA2_Stream5,UART1_MAX_RECV_LEN);//��װ��
			UART1_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART1_Recv_Buf[UART1_User_Buf_No];
			DMA_Init(DMA2_Stream5, &UART1_DMA_RX);	
			//DMA_SetCurrDataCounter(DMA2_Stream2,MAX_RCV_LEN);//��װ��
			DMA_Cmd(DMA2_Stream5, ENABLE);//������,�ؿ�DMA
			 
			#else
			 UART1_Str.data = UART1_Recv_Buf[UART1_Str.User_Buf_No];
			 UART1_Str.Recv_Flag = 1;
			 UART1_Str.User_Buf_No++;
			 if(UART1_Str.User_Buf_No >= UART_MAX_ARR)
				 UART1_Str.User_Buf_No = 0;
			#endif
			//��SR���DR���Idle
			i = USART1->SR;
			i = USART1->DR;
			 
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���Ϊ�����ж�
	{
		UART1_Recv_Buf[UART1_User_Buf_No][UART1_Str.Recv_Len] = USART1->DR;
		UART1_Str.Recv_Len++;
		if(UART1_Str.Recv_Len > UART1_MAX_RECV_LEN)
			UART1_Str.Recv_Len = 0;
	}
	if(USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//����
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
				DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���־
				DMA_SetCurrDataCounter(DMA2_Stream5,UART1_MAX_RECV_LEN);//��װ��
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
 	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7 | DMA_FLAG_TCIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7);//���־		
    DMA_ClearFlag(DMA2_Stream7,DMA_IT_TEIF7);
  
 }



 