
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
*���嶨ʱ����ʱ��ص�������ԭ��
*/
typedef int8_t (*Time_FuncPtr_TypeDef)(void *cbptr);

/*
*
*���崮�ڽ��յ����ݺ�ص�������ԭ��
*/
typedef s16 (*Uart_FuncPtr_TypeDef)(void *cbptr);

/*
*���峣�õĲ�����,�û������ɶ�����Ҫ�Ĳ�����,����߲����ʲ�Ҫ����115200
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
*����ͨ�ô������ݽṹ��
*
*/
typedef struct
{
  char *data;                  /*���ݻ�����*/
	u16 Recv_Len;               /*һ֡����*/	
	u8 Recv_Flag;		           /*�յ�һ֡��־*/
	u8 Send_Finish;            /*һ֡�������*/
}_Recv_Str;
/*
*  ����ϵͳÿTICK�������ʱ����
*/
typedef enum
{
	Period_1MS   = 1000,   //ÿTICK��ʱ1MS,100TICKΪ1S
	Period_10MS  = 100,
}_TIMER_PERIOD;

/*
* ������ʹ�õ����ڸ�ʽ
*/
typedef struct 
{
	u8	sec;    /* ��- [0,59]  */
	u8	min;    /* ��- [0,59]  */
	u8	hour;   /* ʱ- [0,23]  */
	u8	day;    /* ��- [1,31]  */
	u8	mon;    /* ��- [1,12] */
	u8	year;   /* ��- [0,127] */
	#if RTC_WEEKDAY_ENABLE
	u8  weekday; /*���� [1 - 7]*/
	#endif
}RtcStruct;

/*
*�����Ƿ񿪻����Ź�
*/
#define WATCHDOG_ENABLE 1     //�Ƿ��������Ź�
#define WATCHDOG_Time   20    //���Ź�ι��ʱ��

/*
*����GPIO������
*��ѡGPIO_Speed_25MHz
*		 GPIO_Speed_25MHz
*/
#define GPIO_SPEED  GPIO_Speed_25MHz

#define DISABLE_INT __disable_irq()     //�ر����ж�
#define ENABLE_INT  __enable_irq()			//�������ж�

//#define _STRING(x) #x
//#define STRING(x) _STRING(#x)
#define STRING(x) #x

/*
*���崮��1,2,6�Ƿ�ʹ��DMA�շ�����
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
*����printf������ĸ�����,���漸��ѡ��ֻ��ѡ������һ��,����ͬʱѡ����
*
*/
//#define UART1_PRINTF_ENABLE  
//#define UART2_PRINTF_ENABLE
//#define UART3_PRINTF_ENABLE
//#define UART4_PRINTF_ENABLE
//#define UART5_PRINTF_ENABLE
#define UART6_PRINTF_ENABLE

/*
*�Ƿ�ʹ��MODBUS
*/
#define MODBUS_ENABLE 1


/*
*���弸·ADC�ڲ��������е�λ��
*/
enum
{
	BAT_VAR_MONITOR = 0,//��ص�ѹ���
};

#endif




