

#ifndef YD_UART1_H
#define YD_UART1_H

#include "Global_Config.h"

#define UART1_AF_SEL     1 /*2*/  /*重映射功能选择对应不同的管脚,只可选1或2;根据电路图实际连接情况选择*/

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
*定义串口单次接收的最大长度
*/
#define UART1_MAX_RECV_LEN    100
/*串口底层轮流接收缓冲区个数*/
#define UART1_MAX_ARR          3

extern _Recv_Str UART1_Str;

extern DMA_InitTypeDef UART1_DMA_TX,UART1_DMA_RX;


/*
*********************************************************************************************************
*	函 数 名: USART1_Init
*	功能说明: 初始化CPU的USART1串口硬件设备。用于和上层软件通信
*	形    参：_YD_UART_BAUD:波特率;cb:接收到数据后的回调函数
*	返 回 值: 无
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
*	函 数 名: USART1_CLK_GPIO_Set
*	功能说明: 设置串口1时钟和GPIO
*	形    参：ENABLE:使能串口1及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_CLK_GPIO_Set(FunctionalState NewState);


#endif











