
#ifndef __UART3_H
#define __UART3_H

#include "Global_Config.h"

#define UART3_AF_SEL    3 /*1 2*/   /*重映射功能选择对应不同的管脚,只可选1或2或3;根据电路图实际连接情况选择*/

#if UART3_AF_SEL == 1

#define USART3_TX_PIN   GPIO_Pin_10
#define USART3_RX_PIN   GPIO_Pin_11

#define USART3_TX_PORT   GPIOB
#define USART3_RX_PORT   GPIOB

#define USART3_TX_PORT_CLK   RCC_AHB1Periph_GPIOB
#define USART3_RX_PORT_CLK   RCC_AHB1Periph_GPIOB

#define USART3_TX_SOURCE GPIO_PinSource10
#define USART3_RX_SOURCE GPIO_PinSource11

#elif UART3_AF_SEL == 2

#define USART3_TX_PIN   GPIO_Pin_10
#define USART3_RX_PIN   GPIO_Pin_11

#define USART3_TX_PORT   GPIOC
#define USART3_RX_PORT   GPIOC

#define USART3_TX_PORT_CLK   RCC_AHB1Periph_GPIOC
#define USART3_RX_PORT_CLK   RCC_AHB1Periph_GPIOC

#define USART3_TX_SOURCE GPIO_PinSource10
#define USART3_RX_SOURCE GPIO_PinSource11

#elif UART3_AF_SEL == 3

#define USART3_TX_PIN   GPIO_Pin_8
#define USART3_RX_PIN   GPIO_Pin_9

#define USART3_TX_PORT   GPIOD
#define USART3_RX_PORT   GPIOD

#define USART3_TX_PORT_CLK   RCC_AHB1Periph_GPIOD
#define USART3_RX_PORT_CLK   RCC_AHB1Periph_GPIOD

#define USART3_TX_SOURCE GPIO_PinSource8
#define USART3_RX_SOURCE GPIO_PinSource9

#endif

/*
*定义串口单次接收的最大长度
*/
#define UART3_MAX_RECV_LEN    2000
/*串口底层轮流接收缓冲区个数*/
#define UART3_MAX_ARR          2


extern _Recv_Str UART3_Str;

extern DMA_InitTypeDef UART3_DMA_TX,UART3_DMA_RX;

void USART3_Init(_UART_BAUD BaudRate);
void USART3_RX_DMA(void);
void USART3_TX_DMA(void);
u8 USART3_Send_Byte(u8 ch);
void USART3_Send_Str(char *buf,u16 len);
/*
*********************************************************************************************************
*	函 数 名: USART2_CLK_GPIO_Set
*	功能说明: 设置串口2时钟和GPIO
*	形    参：ENABLE:使能串口2及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART3_CLK_GPIO_Set(FunctionalState NewState);


#endif


