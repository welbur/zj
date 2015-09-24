

#ifndef YD_UART1_H
#define YD_UART1_H

#include "Global_Config.h"

#define UART1_AF_SEL     1 /*2*/  /*��ӳ�书��ѡ���Ӧ��ͬ�Ĺܽ�,ֻ��ѡ1��2;���ݵ�·ͼʵ���������ѡ��*/

#if UART1_AF_SEL == 1

#define USART1_TX_PIN   GPIO_Pin_9
#define USART1_RX_PIN   GPIO_Pin_10

#define USART1_TX_PORT   GPIOA
#define USART1_RX_PORT   GPIOA

#define USART1_TX_PORT_CLK   RCC_AHB1Periph_GPIOA
#define USART1_RX_PORT_CLK   RCC_AHB1Periph_GPIOA

#define USART1_TX_SOURCE GPIO_PinSource9
#define USART1_RX_SOURCE GPIO_PinSource10


#elif UART1_AF_SEL == 2

#define USART1_TX_PIN   GPIO_Pin_6
#define USART1_RX_PIN   GPIO_Pin_7

#define USART1_TX_PORT   GPIOB
#define USART1_RX_PORT   GPIOB

#define USART1_TX_PORT_CLK   RCC_AHB1Periph_GPIOB
#define USART1_RX_PORT_CLK   RCC_AHB1Periph_GPIOB

#define USART1_TX_SOURCE GPIO_PinSource6
#define USART1_RX_SOURCE GPIO_PinSource7

#endif


/*
*���崮�ڵ��ν��յ���󳤶�
*/
#define UART1_MAX_RECV_LEN    100
/*���ڵײ��������ջ���������*/
#define UART1_MAX_ARR          3

extern _Recv_Str UART1_Str;

extern DMA_InitTypeDef UART1_DMA_TX,UART1_DMA_RX;


/*
*********************************************************************************************************
*	�� �� ��: USART1_Init
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸�����ں��ϲ����ͨ��
*	��    �Σ�_YD_UART_BAUD:������;cb:���յ����ݺ�Ļص�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_Init(_UART_BAUD BaudRate);
#if UART1_DMA_RX_ENABLE
void USART1_RX_DMA(void);
#endif
#if UART1_DMA_TX_ENABLE
void USART1_TX_DMA(void);
#endif
unsigned char USART1_Send_Byte(unsigned char ch);
unsigned char USART1_Send_Str(char *buf,uint16_t len);
/*
*********************************************************************************************************
*	�� �� ��: USART1_CLK_GPIO_Set
*	����˵��: ���ô���1ʱ�Ӻ�GPIO
*	��    �Σ�ENABLE:ʹ�ܴ���1����Ӧʱ����GPIO����;DISABLE:ʧЧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_CLK_GPIO_Set(FunctionalState NewState);


#endif











