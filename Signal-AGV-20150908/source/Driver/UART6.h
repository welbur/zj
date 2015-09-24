

#ifndef YD_UART6_H
#define YD_UART6_H

#include "Global_Config.h"

#define UART6_AF_SEL    /*1*/ 2   /*��ӳ�书��ѡ���Ӧ��ͬ�Ĺܽ�,ֻ��ѡ1��2;���ݵ�·ͼʵ���������ѡ��*/

#if UART6_AF_SEL == 1

#define USART6_TX_PIN   GPIO_Pin_14
#define USART6_RX_PIN   GPIO_Pin_11

#define USART6_TX_PORT   GPIOG
#define USART6_RX_PORT   GPIOG

#define USART6_TX_PORT_CLK   RCC_AHB1Periph_GPIOG
#define USART6_RX_PORT_CLK   RCC_AHB1Periph_GPIOG

#define USART6_TX_SOURCE GPIO_PinSource14
#define USART6_RX_SOURCE GPIO_PinSource11


#elif UART6_AF_SEL == 2

#define USART6_TX_PIN   GPIO_Pin_6
#define USART6_RX_PIN   GPIO_Pin_7

#define USART6_TX_PORT   GPIOC
#define USART6_RX_PORT   GPIOC

#define USART6_TX_PORT_CLK   RCC_AHB1Periph_GPIOC
#define USART6_RX_PORT_CLK   RCC_AHB1Periph_GPIOC

#define USART6_TX_SOURCE GPIO_PinSource6
#define USART6_RX_SOURCE GPIO_PinSource7

#endif


/*
*���崮�ڵ��ν��յ���󳤶�
*/
#define UART6_MAX_RECV_LEN    1000
/*���ڵײ��������ջ���������*/
#define UART6_MAX_ARR          2

extern _Recv_Str UART6_Str;

extern DMA_InitTypeDef UART6_DMA_TX,UART6_DMA_RX;

void USART6_Init(_UART_BAUD BaudRate);
void USART6_RX_DMA(void);
void USART6_TX_DMA(void);
unsigned char USART6_Send_Byte(unsigned char ch);
unsigned char USART6_Send_Str(char *buf,uint16_t len);
/*
*********************************************************************************************************
*	�� �� ��: USART6_CLK_GPIO_Set
*	����˵��: ���ô���6ʱ�Ӻ�GPIO
*	��    �Σ�ENABLE:ʹ�ܴ���6����Ӧʱ����GPIO����;DISABLE:ʧЧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART6_CLK_GPIO_Set(FunctionalState NewState);

#endif




