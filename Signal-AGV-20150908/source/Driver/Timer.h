

#ifndef YD_TIMER_H
#define YD_TIMER_H

#include "Global_Config.h"

#define MAX_TIMER_NUM  30


#define	Only_Mode  1              /*一次工作模式*/
#define	Auto_Mode  0xffffffff     /*循环工作模式*/

#define SEC 1000
#define MS   1


typedef struct
{
	volatile u32 TimeNum;  /*定时次数*/
	volatile u8 Flag;     /*定时到达标志*/
	volatile u32 Count;   /*定时时间*/
	volatile u32 Preload; /*循环定时预装值*/
	volatile s32 Address;
	Time_FuncPtr_TypeDef Callback; /*定时超时后相应的处理函数*/
	void *ptr;
}_Timer_Str;




extern _Timer_Str TimeList[];

/*******************************************************************************
* Function Name  : Init_SYSTICK
* Description    : 配置SYSTICK工作方式
* Input          : 产生每秒 Period次中断
* Output         : Period
* Return         : None
*******************************************************************************/
void Init_SYSTICK(_TIMER_PERIOD Period);

/*******************************************************************************
* Function Name  : Init_SYS_Timer
* Description    : 初始化延时所使用硬件定时器
* Input          : 产生每秒 Period次中断
* Output         : Period
* Return         : None
*******************************************************************************/
void SYS_Timer_Init(_TIMER_PERIOD Period);
/*******************************************************************************
* Function Name  : SYS_Timer_Close
* Description    : 关闭硬件定时器
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void SYS_Timer_Close(void);
/*******************************************************************************
* Function Name  : SYS_Timer_Close
* Description    : 关闭硬件定时器
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void SYS_Timer_Open(void);
/*******************************************************************************
* Function Name  : Malloc_Timer
* Description    : 申请使用定时器
* Input          : timeout:定时时间;TimeNum:定时循环次数cb:定时超时后的回调函数,cbptr:回调函数参数
* Output         : None
* Return         : 成功返回定时器索引;失败返回 -1
*******************************************************************************/
s8 Malloc_Timer(u32 timeout,u32 TimeNum,Time_FuncPtr_TypeDef cb,void *cbptr, s8 *IndexAddr);
/*******************************************************************************
* Function Name  : Free_Timer
* Description    : 释放使用定时器
* Input          : timerindex,在定时列表中的位置
* Output         : None
* Return         : 成功返回定时器索引;失败返回 -1

* NOTE: 定时器只定时一次,到达后系统会自动释放该定时器,
        定时多次时,想提早结束定时才需要调用此函数释放定时器

*******************************************************************************/
s8 Free_Timer(s8 *timerindex);
/*******************************************************************************
* Function Name  : TimeDec
* Description    : 递减定时值
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TimeDec(void);
/*******************************************************************************
* Function Name  : YD_Timer_Check
* Description    : 定时器超时检查
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
s8 Timer_Check(void);
/*******************************************************************************
* Function Name  : Delay
* Description    : 延时函数,程序将等待超时到达后才跳出此函数
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
s8 Delay(u32 timeout);

void Delay_100ns(__IO uint32_t nCount);
void Delay_us(__IO uint32_t nCount);
void Delay_ms(__IO uint32_t nCount);














#endif


