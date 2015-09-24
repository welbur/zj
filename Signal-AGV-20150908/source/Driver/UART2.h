

#ifndef YD_UART2_H
#define YD_UART2_H

#include "Global_Config.h"

#define UART2_AF_SEL    2 /*1*/   /*重映射功能选择对应不同的管脚,只可选1或2;根据电路图实际连接情况选择*/

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
*定义串口单次接收的最大长度
*/
#define UART2_MAX_RECV_LEN    2000
/*串口底层轮流接收缓冲区个数*/
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
*	函 数 名: USART2_CLK_GPIO_Set
*	功能说明: 设置串口2时钟和GPIO
*	形    参：ENABLE:使能串口2及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART2_CLK_GPIO_Set(FunctionalState NewState);

#endif






