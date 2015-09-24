

#ifndef YD_UART6_H
#define YD_UART6_H

#include "Global_Config.h"

#define UART6_AF_SEL    /*1*/ 2   /*重映射功能选择对应不同的管脚,只可选1或2;根据电路图实际连接情况选择*/

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
*定义串口单次接收的最大长度
*/
#define UART6_MAX_RECV_LEN    1000
/*串口底层轮流接收缓冲区个数*/
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
*	函 数 名: USART6_CLK_GPIO_Set
*	功能说明: 设置串口6时钟和GPIO
*	形    参：ENABLE:使能串口6及相应时钟与GPIO配置;DISABLE:失效
*	返 回 值: 无
*********************************************************************************************************
*/
void USART6_CLK_GPIO_Set(FunctionalState NewState);

#endif




