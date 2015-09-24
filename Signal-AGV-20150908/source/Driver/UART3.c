
#include "Global_Config.h"
#include "UART3.h"

#ifdef UART3_PRINTF_ENABLE
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
  USART_SendData(USART3, (uint8_t) ch);
	/* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
  {}
  return ch;
}	
#endif

char UART3_Recv_Buf[UART3_MAX_ARR][UART3_MAX_RECV_LEN];/*����N�������������������մ��ڵײ�Э��*/
u8 UART3_User_Buf_No = 0;                            /*��ǰ����ʹ�õĻ�����*/

_Recv_Str UART3_Str;
DMA_InitTypeDef UART3_DMA_TX,UART3_DMA_RX;


/*
*********************************************************************************************************
*	�� �� ��: USART3_Init
*	����˵��: ��ʼ��CPU��USART3����Ӳ���豸������������ģ��ͨ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_Init(_UART_BAUD BaudRate)
{
	USART_InitTypeDef USART_InitStructure;   
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	
	RCC_AHB1PeriphClockCmd(USART3_RX_PORT_CLK | USART3_TX_PORT_CLK , ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* ���÷��͹ܽ�*/
  GPIO_PinAFConfig(USART3_RX_PORT, USART3_RX_SOURCE, GPIO_AF_USART3);

  /*���ý��չܽ�*/
  GPIO_PinAFConfig(USART3_TX_PORT, USART3_TX_SOURCE, GPIO_AF_USART3);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_InitStructure.GPIO_Pin = USART3_RX_PIN;  
  GPIO_Init(USART3_RX_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN;
  GPIO_Init(USART3_TX_PORT, &GPIO_InitStructure);
	

       
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
	USART_Init(USART3, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	#if UART3_DMA_RX_ENABLE
	/*�����ж�*/
  USART_ITConfig(USART3, USART_IT_IDLE , ENABLE);
  #else
	USART_ITConfig(USART3, USART_IT_RXNE | USART_IT_IDLE , ENABLE);
	#endif
  	
	/* Enable USART */
  USART_Cmd(USART3, ENABLE);
  
  /* 
		CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
	 	�巢����ɱ�־��Transmission Complete flag 
	*/
	USART_ClearFlag(USART3, USART_FLAG_TC); 
	
	
	memset((u8*)&UART3_Str,0x00,sizeof(UART3_Str));
	#if UART3_DMA_RX_ENABLE
	  USART3_RX_DMA();
	#endif
	#if UART3_DMA_TX_ENABLE
		USART3_TX_DMA();
		UART3_Str.Send_Finish = 1;
	#endif
   
}
/*
*********************************************************************************************************
*	�� �� ��: USART3_CLK_GPIO_Set
*	����˵��: ���ô���3ʱ�Ӻ�GPIO
*	��    �Σ�ENABLE:ʹ�ܴ���3����Ӧʱ����GPIO����;DISABLE:ʧЧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_CLK_GPIO_Set(FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (NewState != DISABLE)
	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//		
//		/* ���÷��͹ܽ�*/
//		GPIO_PinAFConfig(USART3_TX_PORT, USART3_TX_SOURCE, GPIO_AF_USART3);

//		/*���ý��չܽ�*/
//		GPIO_PinAFConfig(USART3_RX_PORT, USART3_RX_SOURCE, GPIO_AF_USART3);

		/* Configure USART Tx as alternate function  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN;  
		GPIO_Init(USART3_TX_PORT, &GPIO_InitStructure);

		/* Configure USART Rx as alternate function  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = USART3_RX_PIN;
		GPIO_Init(USART3_RX_PORT, &GPIO_InitStructure);
		
		USART_Cmd(USART3, ENABLE);
	}
	else
	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);

		USART_Cmd(USART3, DISABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN;
    GPIO_Init(USART3_TX_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = USART3_RX_PIN;
		GPIO_Init(USART3_RX_PORT, &GPIO_InitStructure);
		
		
	}
}



#if UART3_DMA_RX_ENABLE
/*
*********************************************************************************************************
*	�� �� ��: USART3_RX_DMA
*	����˵��: ����USART3 DMA���շ�ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_RX_DMA(void)
{
	//DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);                             //��DMAʱ��
	  	 
	DMA_StructInit(&UART3_DMA_RX);

	UART3_DMA_RX.DMA_Channel = DMA_Channel_4;
	UART3_DMA_RX.DMA_PeripheralBaseAddr = (u32)&USART3->DR;
	UART3_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART3_Recv_Buf[UART3_User_Buf_No];
	UART3_DMA_RX.DMA_DIR = DMA_DIR_PeripheralToMemory;
	UART3_DMA_RX.DMA_BufferSize = UART3_MAX_RECV_LEN;
	UART3_DMA_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	UART3_DMA_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	UART3_DMA_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	UART3_DMA_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	UART3_DMA_RX.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	UART3_DMA_RX.DMA_Priority = DMA_Priority_Medium;
	UART3_DMA_RX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	UART3_DMA_RX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	UART3_DMA_RX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	UART3_DMA_RX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &UART3_DMA_RX);
	 
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Stream1, ENABLE);


  DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);                                //DMA5��������ж�
  DMA_ITConfig(DMA1_Stream1, DMA_IT_TE, ENABLE);


  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif

#if UART3_DMA_TX_ENABLE
/*
*********************************************************************************************************
*	�� �� ��: USART3_TX_DMA
*	����˵��: ����USART3 DMA���ͷ�ʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_TX_DMA(void)
{

	  NVIC_InitTypeDef   NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);                             //��DMAʱ��
	  
	 DMA_DeInit(DMA1_Stream3);
	
	 DMA_StructInit(&UART3_DMA_TX);
	 UART3_DMA_TX.DMA_Channel = DMA_Channel_4;
	 UART3_DMA_TX.DMA_PeripheralBaseAddr = (u32)&USART3->DR;
	 /*��Ҫ���͵�ʱ���ٸı��ַ�ͳ���*/
	 //UART3_DMA_TX.DMA_Memory0BaseAddr = (u32)&UART3_Recv_Buf[UART3_User_Buf_No];
	 UART3_DMA_TX.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	 //UART3_DMA_TX.DMA_BufferSize = UART3_MAX_RECV_LEN;
	 UART3_DMA_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 UART3_DMA_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 UART3_DMA_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 UART3_DMA_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 UART3_DMA_TX.DMA_Mode = DMA_Mode_Normal;//*/DMA_Mode_Circular;
	 UART3_DMA_TX.DMA_Priority = DMA_Priority_High;
	 UART3_DMA_TX.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 UART3_DMA_TX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	 UART3_DMA_TX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 UART3_DMA_TX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 DMA_Init(DMA1_Stream3, &UART3_DMA_TX);
	 
	 USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	 DMA_ITConfig(DMA1_Stream3, DMA_IT_TC | DMA_IT_TE, ENABLE);               //DMA2��������ж�

	 DMA_Cmd(DMA1_Stream3,DISABLE);   /*��Ҫ���͵�ʱ��Ŵ�ʹ��*/
			
	  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
#endif
/*
*********************************************************************************************************
*	�� �� ��: USART3_Send_Byte
*	����˵��: USART3����һ���ֽ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 USART3_Send_Byte(u8 ch)
{
	USART_SendData(USART3, ch);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET )
	  {
	  	
	  }
	return ch;
}

/*
*********************************************************************************************************
*	�� �� ��: USART3_Send_Byte
*	����˵��: USART3���ʹ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_Send_Str(char *buf,u16 len)
{
	#if UART3_DMA_TX_ENABLE
		//while(!UART3_Str.Send_Finish);
		UART3_Str.Send_Finish = 0;
	  DMA_Cmd(DMA1_Stream3,DISABLE);
    UART3_DMA_TX.DMA_Memory0BaseAddr = (u32)buf;
    UART3_DMA_TX.DMA_BufferSize = len;
 	  DMA_Init(DMA1_Stream3, &UART3_DMA_TX);
    DMA_Cmd(DMA1_Stream3,ENABLE);
	#else	
	u16 i;
	for(i = 0; i < len; i++)
	  {
	  	USART_SendData(USART3, *buf++);
	    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET )
	     {
	  	
	     }
	  }
	#endif	
}
void USART3_IRQHandler(void)
{
	u16 i;
	
 	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//���Ϊ���������ж�
    {
			#if UART3_DMA_RX_ENABLE
	    DMA_Cmd(DMA1_Stream1, DISABLE);//�ر�DMA,��ֹ�������������
  	  UART3_Str.Recv_Len = UART3_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA1_Stream1); 
			/*��ֹ�շ��л�����δ֪����,һ��Ҫ�յ�����һ֡���ݳ��Ȳ���λ*/
			if(UART3_Str.Recv_Len )
			 {			
				 UART3_Str.data = UART3_Recv_Buf[UART3_User_Buf_No];
         UART3_Str.Recv_Flag = 1;		
				 UART3_User_Buf_No++;
				 if(UART3_User_Buf_No >= UART3_MAX_ARR)
					 UART3_User_Buf_No = 0;
			 }
			DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_TCIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//���־
			DMA_SetCurrDataCounter(DMA1_Stream1,UART3_MAX_RECV_LEN);//��װ��
			UART3_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART3_Recv_Buf[UART3_User_Buf_No];
			DMA_Init(DMA1_Stream1, &UART3_DMA_RX); 
			DMA_Cmd(DMA1_Stream1, ENABLE);//������,�ؿ�DMA
			 
			#else
			UART3_Str.data = UART3_Recv_Buf[UART3_User_Buf_No];
      UART3_Str.Recv_Flag = 1;		
		  UART3_User_Buf_No++;
			if(UART3_User_Buf_No >= UART3_MAX_ARR)
				UART3_User_Buf_No = 0;
		 #endif
			//��SR���DR���Idle
			i = USART3->SR;
			i = USART3->DR;
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���Ϊ�����ж�
	{
		UART3_Recv_Buf[UART3_User_Buf_No][UART3_Str.Recv_Len] = USART1->DR;
		UART3_Str.Recv_Len++;
		if(UART3_Str.Recv_Len > UART3_MAX_RECV_LEN)
			UART3_Str.Recv_Len = 0;
	}	
	if(USART_GetITStatus(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//����
	{
		USART_ClearITPendingBit(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
  if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
  {
   USART_ClearFlag(USART3,USART_FLAG_ORE); //��SR��ʵ���������־
   USART_ReceiveData(USART3);    //��DR
  }
}
/**
 * ISR for USART3 RX  DMA Stream Interrupt
 */
void DMA1_Stream1_IRQHandler()
{
    // Stream2 transfer complete interrupt?
    if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_TCIF1))
      {
        // clear pending interrupt
        DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1);
 
		    UART3_Str.Recv_Len = UART3_MAX_RECV_LEN -DMA_GetCurrDataCounter(DMA1_Stream1); 
				if(UART3_Str.Recv_Len > 0)
				 {			
					UART3_Str.data = UART3_Recv_Buf[UART3_User_Buf_No];
					UART3_Str.Recv_Flag = 1;		
					UART3_User_Buf_No++;
					if(UART3_User_Buf_No >= UART3_MAX_ARR)
						UART3_User_Buf_No = 0;	
				 }
				DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_TCIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//���־
				DMA_SetCurrDataCounter(DMA1_Stream1,UART3_MAX_RECV_LEN);//��װ��

				UART3_DMA_RX.DMA_Memory0BaseAddr = (u32)&UART3_Recv_Buf[UART3_User_Buf_No];
		    DMA_Init(DMA1_Stream1, &UART3_DMA_RX);
      }
   DMA_ClearFlag(DMA1_Stream1,DMA_IT_TEIF1 | DMA_FLAG_HTIF1);
 
}
/**
 * ISR for USART3 TX  DMA Stream Interrupt
 */
void DMA1_Stream3_IRQHandler()
{
    // Stream6 transfer complete interrupt?
    if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3))
      {
        // clear pending interrupt
        DMA_ClearFlag(DMA1_Stream3,DMA_IT_TCIF3);
 
	
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_DMEIF3 | DMA_FLAG_FEIF3 | DMA_FLAG_TCIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_HTIF3);//���־
		DMA_Cmd(DMA1_Stream3,DISABLE);

		UART3_Str.Send_Finish = 1;
      }
   //DMA_ClearFlag(DMA1_Stream3,DMA_IT_TEIF3 | DMA_FLAG_TCIF3);
 
}



