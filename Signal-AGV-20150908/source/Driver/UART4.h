
#ifndef __UART4_H
#define __UART4_H

#include "Global_Config.h"

#define UART4_AF_SEL    2 /*1*/   /*重映射功能选择对应不同的管脚,只可选1或2;根据电路图实际连接情况选择*/

#if UART4_AF_SEL == 1

#define USART4_TX_PIN   GPIO_Pin_0
#define USART4_RX_PIN   GPIO_Pin_1

#define USART4_TX_PORT   GPIOA
#define USART4_RX_PORT   GPIOA

#define USART4_TX_PORT_CLK   RCC_AHB1Periph_GPIOA
#define USART4_RX_PORT_CLK   RCC_AHB1Periph_GPIOA

#define USART4_TX_SOURCE GPIO_PinSource0
#define USART4_RX_SOURCE GPIO_PinSource1

#elif UART4_AF_SEL == 2

#define USART4_TX_PIN   GPIO_Pin_10
#define USART4_RX_PIN   GPIO_Pin_11

#define USART4_TX_PORT   GPIOC
#define USART4_RX_PORT   GPIOC

#define USART4_TX_PORT_CLK   RCC_AHB1Periph_GPIOC
#define USART4_RX_PORT_CLK   RCC_AHB1Periph_GPIOC

#define USART4_TX_SOURCE GPIO_PinSource10
#define USART4_RX_SOURCE GPIO_PinSource11

#endif

/*
*定义串口单次接收的最大长度
*/
#define UART4_MAX_RECV_LEN    200
/*串口底层轮流接收缓冲区个数*/
#define UART4_MAX_ARR          2


extern _Recv_Str UART4_Str;

extern DMA_InitTypeDef UART4_DMA_TX,UART4_DMA_RX;

void USART4_Init(_UART_BAUD BaudRate);
void USART4_RX_DMA(void);
void USART4_TX_DMA(void);
u8 USART4_Send_Byte(u8 ch);
void USART4_Send_Str(char *buf,u16 len);
/*
*********************************************************************************************************
*	函 数 名: USART2_CLK_GPIO_Set
*	功能说明: 设置串口2时钟和GPIO
*	形    参：ENABLE:使能串口2及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART4_CLK_GPIO_Set(FunctionalState NewState);


#endif


