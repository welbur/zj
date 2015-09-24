
#include "Global_Config.h"
#include "Timer.h"

_Timer_Str TimeList[MAX_TIMER_NUM];
static u8 Open_Sel = 0;
u32 RealPeriod;
RCC_ClocksTypeDef RCC_Clocks;

/*******************************************************************************
* Function Name  : Init_SYSTICK
* Description    : 配置SYSTICK工作方式
* Input          : 产生每秒 Period次中断
* Output         : Period
* Return         : None
*******************************************************************************/
void Init_SYSTICK(_TIMER_PERIOD Period)
{
	
	/* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
		
  SysTick_Config(RCC_Clocks.HCLK_Frequency / Period);
}
/*******************************************************************************
* Function Name  : Close_SYSTICK
* Description    : 关闭SYSTICK中断
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void Close_SYSTICK(void)
{
	  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk;
	/* Function successful */
}
/*******************************************************************************
* Function Name  : Open_SYSTICK
* Description    : 开户SYSTICK中断
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void Open_SYSTICK(void)
{
	  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
	/* Function successful */
}
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimeDec();
}
/*******************************************************************************
* Function Name  : Init_SYS_Timer
* Description    : 初始化延时所使用硬件定时器
* Input          : 产生每秒 Period次中断
* Output         : Period
* Return         : None
*******************************************************************************/
void SYS_Timer_Init(_TIMER_PERIOD Period)
{
	
	memset((u8*)&TimeList,0x00,sizeof(TimeList));
	RealPeriod = Period;
	
	Init_SYSTICK(Period);
}
/*******************************************************************************
* Function Name  : SYS_Timer_Close
* Description    : 关闭硬件定时器
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void SYS_Timer_Close(void)
{
	Close_SYSTICK();
}
/*******************************************************************************
* Function Name  : SYS_Timer_Close
* Description    : 关闭硬件定时器
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void SYS_Timer_Open(void)
{
	Open_SYSTICK();
}
/*******************************************************************************
* Function Name  : Malloc_Timer
* Description    : 申请使用定时器
* Input          : timeout:定时时间;TimeNum:定时循环次数cb:定时超时后的回调函数,cbptr:回调函数参数,IndexAddr 索引值存放位置的地址(该地址只能是静态地址，不能是栈地址)
* Output         : None
* Return         : 成功返回定时器索引;失败返回 -1
*******************************************************************************/
s8 Malloc_Timer(u32 timeout,u32 TimeNum,Time_FuncPtr_TypeDef cb,void *cbptr, s8 *IndexAddr )
{
	u8 i;
	u32 Period = 0;
	
	for(i = 0; i < MAX_TIMER_NUM;i++)
	{
		if((TimeList[i].Preload == 0) && (TimeList[i].Flag == 0))
			break;
	}
	if( i == MAX_TIMER_NUM)
		return -1;
	switch(RealPeriod)
	{
		case Period_1MS:
			Period = timeout;
		break;
		case Period_10MS:
		  if(timeout % 10)
				Period = 1;
			if(timeout / 10)
				Period += timeout / 10;
		break;
//		case Period_100MS:
//			if(timeout % 100)
//				Period = 1;
//			if(timeout / 100)
//				Period += timeout / 100;
//		break;
		default:return -1;
	}
	
	DISABLE_INT;
	memset((u8*)&TimeList[i],0x00,sizeof(TimeList[i]));
	TimeList[i].Count = Period;
	//if(mode == Auto_Mode)
		TimeList[i].Preload = Period;
	TimeList[i].TimeNum   = TimeNum;
	TimeList[i].Callback = cb;
	TimeList[i].ptr = cbptr;
	if( IndexAddr != NULL )
	{
		TimeList[i].Address = (s32)IndexAddr;
	}
	
	
	ENABLE_INT;
	SYS_Timer_Open();
	Open_Sel = 0x01;
	if( IndexAddr != NULL )
	{
		(*IndexAddr) = i;
	}
	
	
	return i;
}

/*******************************************************************************
* Function Name  : Free_Timer
* Description    : 释放使用定时器
* Input          : timerindex,在定时列表中的位置
* Output         : None
* Return         : 成功返回定时器索引;失败返回 -1

* NOTE: 定时器只定时一次,到达后系统会自动释放该定时器,
        定时多次时,想提早结束定时才需要调用此函数释放定时器

*******************************************************************************/
s8 Free_Timer(s8 *timerindex)
{
	s8 index = (*timerindex);
	if( (*timerindex) == -1)
		return -1;
	if( (*timerindex) >= MAX_TIMER_NUM)
		return -1;
	//DISABLE_INT();
	//printf("\r\n timerindex = %d\r\n", (*timerindex) );
	memset((u8*)&TimeList[(*timerindex)],0x00,sizeof(TimeList[0]));
	(*timerindex) = -1;
	//ENABLE_INT();
	return index;
}
/*******************************************************************************
* Function Name  : YD_Timer_Check
* Description    : 定时器超时检查
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
s8 Timer_Check(void)
{
	u8 i,j;
	for(i = 0,j = 0; i < MAX_TIMER_NUM;i++)
	{
		if(TimeList[i].Flag)/*定时到达后,执行定时回调函数*/
		{
			TimeList[i].Flag = 0;
			if(TimeList[i].Callback != NULL)
				TimeList[i].Callback(TimeList[i].ptr);
		}
		if(TimeList[i].Preload == 0)
		{
			j++;
		}
	}
	if((j == MAX_TIMER_NUM) && (Open_Sel))
	{
		Open_Sel = 0;
		SYS_Timer_Close();
	}
	return i;
}
/*******************************************************************************
* Function Name  : TimeDec
* Description    : 递减定时值
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TimeDec(void)
{
	u8 i;
	for( i = 0; i < MAX_TIMER_NUM;i++)
	{
		if(TimeList[i].Count)
		{
			if(--TimeList[i].Count == 0)
			{
				TimeList[i].Flag = 1;
				if(TimeList[i].TimeNum == 0x00000001)
				{
					TimeList[i].Preload = 0;
					
					if( TimeList[i].Address != 0 )
					{
						*((s32 *)(TimeList[i].Address)) = -1;
					}
					
				}
				else
				{
					TimeList[i].Count = TimeList[i].Preload;
				}
				TimeList[i].TimeNum--;
			}
		}//end if(TimeList[i].Count)
	}//end for( i = 0; i < MAX_TIMER_NUM;i++)
}

/*******************************************************************************
* Function Name  : Delay
* Description    : 延时函数,程序将等待超时到达后才跳出此函数
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
s8 Delay(u32 timeout)
{
	s8 index = 0;
	index = Malloc_Timer(timeout,1,NULL,NULL, NULL);
	if(index == -1)
		return -1;
	while(!TimeList[index].Flag)
	{
		#if WATCHDOG_ENABLE
		IWDG_ReloadCounter();	//喂狗
		#endif
	}
	TimeList[index].Flag = 0;
	return 0;
}

/****************************************************************************
* 名    称：void Delay_100ns(__IO uint32_t nCount)
* 功    能：纳秒级延时函数
* 入口参数：nCount 延迟的时间
* 出口参数：无
****************************************************************************/
void Delay_100ns(__IO uint32_t nCount)
{
	nCount-=1;
	while(nCount--)
	{
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
	}
}

/****************************************************************************
* 名    称：void Delay_us(__IO uint32_t nCount)
* 功    能：微秒级延时函数
* 入口参数：nCount 延迟的时间
* 出口参数：无
****************************************************************************/
void Delay_us(__IO uint32_t nCount)
{
	while(nCount--)
	{
		Delay_100ns(10);
	}
}

/****************************************************************************
* 名    称：void Delay_us(__IO uint32_t nCount)
* 功    能：微秒级延时函数
* 入口参数：nCount 延迟的时间
* 出口参数：无
****************************************************************************/
void Delay_ms(__IO uint32_t nCount)
{
	while(nCount--)
	{
		Delay_us(1000);
	}
}




