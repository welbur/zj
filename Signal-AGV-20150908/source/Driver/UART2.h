

#ifndef YD_UART2_H
#define YD_UART2_H

#include "Global_Config.h"

#define UART2_AF_SEL    2 /*1*/   /*��ӳ�书��ѡ���Ӧ��ͬ�Ĺܽ�,ֻ��ѡ1��2;���ݵ�·ͼʵ���������ѡ��*/

#if UART2_AF_SEL == 1

#define USART2_TX_PIN   GPIO_Pin_2
#define USART2_RX_PIN   GPIO_Pin_3

#define USART2_TX_PORT   GPIOA
#define USART2_RX_PORT   GPIOA

#define USART2_TX_PORT_CLK   RCC_AHB1Periph_GPIOA
#define USART2_RX_PORT_CLK   RCC_AHB1Periph_GPIOA

#define USART2_TX_SOURCE GPIO_PinSource2
#define USART2_RX_SOURCE GPIO_PinSource3


#elif UART2_AF_SEL == 2

#define USART2_TX_PIN   GPIO_Pin_5
#define USART2_RX_PIN   GPIO_Pin_6

#define USART2_TX_PORT   GPIOD
#define USART2_RX_PORT   GPIOD

#define USART2_TX_PORT_CLK   RCC_AHB1Periph_GPIOD
#define USART2_RX_PORT_CLK   RCC_AHB1Periph_GPIOD

#define USART2_TX_SOURCE GPIO_PinSource5
#define USART2_RX_SOURCE GPIO_PinSource6

#endif


/*
*���崮�ڵ��ν��յ���󳤶�
*/
#define UART2_MAX_RECV_LEN    2000
/*���ڵײ��������ջ���������*/
#define UART2_MAX_ARR          2


extern _Recv_Str UART2_Str;

extern DMA_InitTypeDef UART2_DMA_TX,UART2_DMA_RX;

void USART2_Init(_UART_BAUD BaudRate);
void USART2_RX_DMA(void);
void USART2_TX_DMA(void);
u8 USART2_Send_Byte(u8 ch);
void USART2_Send_Str(char *buf,u16 len);
/*
*********************************************************************************************************
*	�� �� ��: USART2_CLK_GPIO_Set
*	����˵��: ���ô���2ʱ�Ӻ�GPIO
*	��    �Σ�ENABLE:ʹ�ܴ���2����Ӧʱ����GPIO����;DISABLE:ʧЧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART2_CLK_GPIO_Set(FunctionalState NewState);

#endif






