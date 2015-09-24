
#ifndef __GLOBAL_CONFIG_H
#define __GLOBAL_CONFIG_H

#include "stm32f4xx.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
//#include "math.h"
#include "Global_Struct.h"


/*
*
*定义定时器超时后回调函数的原型
*/
typedef int8_t (*Time_FuncPtr_TypeDef)(void *cbptr);

/*
*
*定义串口接收到数据后回调函数的原型
*/
typedef s16 (*Uart_FuncPtr_TypeDef)(void *cbptr);

/*
*定义常用的波特率,用户可自由定义想要的波特率,但最高波特率不要超过115200
*
*/
typedef enum
{
	BAUD_1200 = 1200,
	BAUD_2400 = 2400,
	BAUD_4800 = 4800,
	BAUD_9600 = 9600,
	BAUD_19200 = 19200,
	BAUD_38400 = 38400,
	BAUD_56000 = 56000,
	BAUD_57600 = 57600,
	BAUD_115200 = 115200,
}_UART_BAUD;

/*
*定义通用串口数据结构体
*
*/
typedef struct
{
  char *data;                  /*数据缓冲区*/
	u16 Recv_Len;               /*一帧长度*/	
	u8 Recv_Flag;		           /*收到一帧标志*/
	u8 Send_Finish;            /*一帧发送完毕*/
}_Recv_Str;
/*
*  定义系统每TICK代表的延时周期
*/
typedef enum
{
	Period_1MS   = 1000,   //每TICK延时1MS,100TICK为1S
	Period_10MS  = 100,
}_TIMER_PERIOD;

/*
* 定义所使用的日期格式
*/
typedef struct 
{
	u8	sec;    /* 秒- [0,59]  */
	u8	min;    /* 分- [0,59]  */
	u8	hour;   /* 时- [0,23]  */
	u8	day;    /* 日- [1,31]  */
	u8	mon;    /* 月- [1,12] */
	u8	year;   /* 年- [0,127] */
	#if RTC_WEEKDAY_ENABLE
	u8  weekday; /*星期 [1 - 7]*/
	#endif
}RtcStruct;

/*
*定义是否开户看门狗
*/
#define WATCHDOG_ENABLE 1     //是否启动看门狗
#define WATCHDOG_Time   20    //看门狗喂狗时间

/*
*定义GPIO口速率
*可选GPIO_Speed_25MHz
*		 GPIO_Speed_25MHz
*/
#define GPIO_SPEED  GPIO_Speed_25MHz

#define DISABLE_INT __disable_irq()     //关闭总中断
#define ENABLE_INT  __enable_irq()			//开启总中断

//#define _STRING(x) #x
//#define STRING(x) _STRING(#x)
#define STRING(x) #x

/*
*定义串口1,2,6是否使用DMA收发数据
*
*/
#define UART1_DMA_RX_ENABLE   1
#define UART1_DMA_TX_ENABLE   1
#define UART2_DMA_RX_ENABLE   1
#define UART2_DMA_TX_ENABLE   1
#define UART3_DMA_RX_ENABLE   1
#define UART3_DMA_TX_ENABLE   1
#define UART4_DMA_RX_ENABLE   1
#define UART4_DMA_TX_ENABLE   1
#define UART5_DMA_RX_ENABLE   1
#define UART5_DMA_TX_ENABLE   1
#define UART6_DMA_RX_ENABLE   1
#define UART6_DMA_TX_ENABLE   1


/*
*定义printf输出到哪个串口,下面几个选项只可选择其中一个,不可同时选择多个
*
*/
//#define UART1_PRINTF_ENABLE  
//#define UART2_PRINTF_ENABLE
//#define UART3_PRINTF_ENABLE
//#define UART4_PRINTF_ENABLE
//#define UART5_PRINTF_ENABLE
#define UART6_PRINTF_ENABLE

/*
*是否使用MODBUS
*/
#define MODBUS_ENABLE 1


/*
*定义几路ADC在采样数组中的位置
*/
enum
{
	BAT_VAR_MONITOR = 0,//电池电压监测
};

#endif




