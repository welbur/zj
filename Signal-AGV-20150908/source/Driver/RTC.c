

#include "RTC.h"
#include "stdio.h"
#include "GPIO.h"
//#include "PWR_Mode.h"

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

RTC_DateTypeDef 	RTC_DateStructure;
RTC_TimeTypeDef 	RTC_TimeStructure;
RTC_InitTypeDef 	RTC_InitStructure;

__IO uint32_t   uwLsiFreq = 0;
__IO uint32_t   uwCaptureNumber = 0; 
__IO uint32_t   uwPeriodValue = 0;


u8 table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{

	u16 temp2; 
  u8 yearH,yearL; 
	year = year + 2000;
  yearH=year/100;     
	yearL=year%100;  
  // 如果为21世纪,年份数加100   
  if (yearH>19)yearL+=100; 
  // 所过闰年数只算1900年之后的   
  temp2=yearL+yearL/4; 
  temp2=temp2%7;  
  temp2=temp2+day+table_week[month-1]; 
  if (yearL%4==0&&month<3)
		temp2--; 
   if((temp2%7) == 0)
		{ 
       temp2 = 7; 
       return(temp2); 
		} 
   else 
       return(temp2%7); 
}

void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
	
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error "Please select the RTC Clock source"
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
	
}
/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void YD_SET_RTC(RtcStruct *SetRtc)
{
  u8 WeekDay = 0;

	/* 设置时分秒，以及显示格式 */
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = SetRtc->hour;
	RTC_TimeStructure.RTC_Minutes = SetRtc->min;
	RTC_TimeStructure.RTC_Seconds = SetRtc->sec;
	//RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
	/* Configure the RTC time register */
  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
  {
    printf("\r\n>> !! RTC Set Time failed. !! <<\n");
  } 
  else
  {
    printf("\r\n>> !! RTC Set Time success. !! <<\n");	
	}
	
		WeekDay = RTC_Get_Week(SetRtc->year + 2000,SetRtc->mon,SetRtc->day);
	/* 设置年月日和星期 */
	RTC_DateStructure.RTC_Year    = SetRtc->year;
	RTC_DateStructure.RTC_Month   = SetRtc->mon;
	RTC_DateStructure.RTC_Date    = SetRtc->day;
	RTC_DateStructure.RTC_WeekDay = WeekDay;
	//RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	/* Configure the RTC date register */
  if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
  {
    printf("\r\n>> !! RTC Set Date failed. !! <<\n");
  } 
  else
  {
    printf("\r\n>> !! RTC Set Date success. !! <<\n");
	}

}
/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void YD_GET_RTC(RtcStruct *SetRtc)
{
		/* 获取时分秒，以及显示格式 */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	SetRtc->hour  = RTC_TimeStructure.RTC_Hours;
	SetRtc->min   = RTC_TimeStructure.RTC_Minutes;
	SetRtc->sec   = RTC_TimeStructure.RTC_Seconds;
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	/* 获取年月日和星期 */
	SetRtc->year  = RTC_DateStructure.RTC_Year;
	SetRtc->mon   = RTC_DateStructure.RTC_Month;
	SetRtc->day   = RTC_DateStructure.RTC_Date;
	#if RTC_WEEKDAY_ENABLE
	SetRtc->weekday = RTC_DateStructure.RTC_WeekDay;	
	#endif
	
	printf("Get RTC : %2d-%2d-%2d %2d:%2d:%2d Week:%2d\r\n",SetRtc->year,SetRtc->mon,SetRtc->day,SetRtc->hour,SetRtc->min,SetRtc->sec,SetRtc->weekday);
}
/*******************************************************************************
* Function Name  : RTC_Init
* Description    : 配置RTC
* Input          : 
* Output         : Period
* Return         : None
*******************************************************************************/
u32 bkp_dr0 = 0;
void RTC_Configuration(void)
{
	RtcStruct Rtc_Def;
	if((bkp_dr0 = RTC_ReadBackupRegister(RTC_BKP_DR0)) != 0x32F2)
  {  
		printf("\r Need to configure RTC....\n");
    /* RTC configuration  */
    RTC_Config();

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		#ifdef RTC_CLOCK_SOURCE_LSI
     /* Get the LSI frequency:  TIM5 is used to measure the LSI frequency */
		uwLsiFreq = GetLSIFrequency();

		/* Calendar Configuration */
		/* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
		//RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv	= (uwLsiFreq/128) - 1;
		//RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		#endif
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
      printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n");
    }
		
		Rtc_Def.year = 14;
		Rtc_Def.mon  = 8;
		Rtc_Def.day  = 26;
		Rtc_Def.hour = 20;
		Rtc_Def.min  = 15;
		Rtc_Def.sec  = 00;
    /* Configure the time register */
    YD_SET_RTC(&Rtc_Def); 
		
	  /* 配置备份寄存器，表示已经设置过RTC */
	  RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n Power On Reset occurred....\n");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n External Reset occurred....\n");
    }

    printf("\r No need to configure RTC....\n");
    /* RTC configuration  */
    RTC_Config();

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		#ifdef RTC_CLOCK_SOURCE_LSI
     /* Get the LSI frequency:  TIM5 is used to measure the LSI frequency */
		uwLsiFreq = GetLSIFrequency();

		/* Calendar Configuration */
		/* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
		//RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv	= (uwLsiFreq/128) - 1;
		//RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		#endif
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
      printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n");
    }    
//    /* Enable the PWR clock */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

//    /* Allow access to RTC */
//    PWR_BackupAccessCmd(ENABLE);

//    /* Wait for RTC APB registers synchronisation */
//    RTC_WaitForSynchro();

//    /* Clear the RTC Alarm Flag */
//    RTC_ClearFlag(RTC_FLAG_ALRAF);

//    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
//    EXTI_ClearITPendingBit(EXTI_Line17);

  }
	
	RTC_Wakeup_Init();
//	RTC_Wakeup_Set(1,ENABLE);
	
}

/**
  * @brief  Configures TIM5 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
u32 GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* Enable the LSI oscillator ************************************************/
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* TIM5 configuration *******************************************************/ 
  /* Enable TIM5 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  /* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
  TIM_RemapConfig(TIM5, TIM5_LSI);

  /* Configure TIM5 presclaer */
  TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);
  
  /* TIM5 configuration: Input Capture mode ---------------------
     The LSI oscillator is connected to TIM5 CH4
     The Rising edge is used as active edge,
     The TIM5 CCR4 is used to compute the frequency value 
  ------------------------------------------------------------ */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  
  /* Enable TIM5 Interrupt channel */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM5 counter */
  TIM_Cmd(TIM5, ENABLE);

  /* Reset the flags */
  TIM5->SR = 0;
    
  /* Enable the CC4 Interrupt Request */  
  TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);

  /* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in 
    stm32f4xx_it.c file) ******************************************************/
  while(uwCaptureNumber != 2)
  {
  }
  /* Deinitialize the TIM5 peripheral registers to their default reset values */
  TIM_DeInit(TIM5);

  /* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
  /* Get SYSCLK, HCLK and PCLKx frequency */
  RCC_GetClocksFreq(&RCC_ClockFreq);

  /* Get PCLK1 prescaler */
  if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
  { 
    /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
    return ((RCC_ClockFreq.PCLK1_Frequency / uwPeriodValue) * 8);
  }
  else
  { /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
    return (((2 * RCC_ClockFreq.PCLK1_Frequency) / uwPeriodValue) * 8) ;
  }
}
/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
uint16_t tmpCC4[2] = {0, 0};
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)
  {    
    /* Get the Input Capture value */
    tmpCC4[uwCaptureNumber++] = TIM_GetCapture4(TIM5);
   
    /* Clear CC4 Interrupt pending bit */
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);

    if (uwCaptureNumber >= 2)
    {
      /* Compute the period length */
      uwPeriodValue = (uint16_t)(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
    }
  }
}

/*******************************************************************************
* Function Name  : RTC_Wakeup_Set
* Description    : 使能/失效 RTC 唤醒中断
* Input          : 
* Output         : Period
* Return         : None
*******************************************************************************/
void RTC_Wakeup_Ctrl(u32 Counter ,FunctionalState NewState)
{
	RTC_WakeUpCmd(DISABLE);
	if(Counter)
		Counter = Counter - 1;
	RTC_SetWakeUpCounter(Counter);
	RTC_WakeUpCmd(NewState);
	RTC_ITConfig(RTC_IT_WUT, NewState);
	/* Clear Wakeup flag*/ 
  PWR_ClearFlag(PWR_FLAG_WU);
	/* Clear WakeUp (WUTF) pending flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);
}

/*******************************************************************************
* Function Name  : RTC_Wakeup_Init
* Description    : 配置RTC 唤醒中断
* Input          : 
* Output         : Period
* Return         : None
*******************************************************************************/
void RTC_Wakeup_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef Wakeup_Exit;
	/* EXTI 配置 *******************************************************/
	EXTI_ClearITPendingBit(EXTI_Line22);
	Wakeup_Exit.EXTI_Line = EXTI_Line22;
	Wakeup_Exit.EXTI_Mode = EXTI_Mode_Interrupt;
	Wakeup_Exit.EXTI_Trigger = EXTI_Trigger_Rising;
	Wakeup_Exit.EXTI_LineCmd = ENABLE;
	EXTI_Init(&Wakeup_Exit);
	/* 使能RTC唤醒中断 */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* 配置RTC唤醒时钟源: CK_SPRE (1Hz) */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	RTC_SetWakeUpCounter(0x0);
	
		/* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);	
	/* Disable WKUP pin */
  PWR_WakeUpPinCmd(DISABLE);	
	/* Clear Wakeup flag*/ 
  PWR_ClearFlag(PWR_FLAG_WU);
	
	/* 使能RTC唤醒中断 */
	RTC_ITConfig(RTC_IT_WUT, DISABLE);
	/* 使能唤醒计数 */
	RTC_WakeUpCmd(DISABLE);
	
	/* Clear WakeUp (WUTF) pending flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);

}

/*
********************************************************************************
* 函 数 名: RTC_WKUP_IRQHandler
* 功能说明: RTC唤醒中断
* 形 参：无
* 返 回 值: 无
********************************************************************************
*/
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		/* Clear WakeUp (WUTF) pending flag */
    RTC_ClearFlag(RTC_FLAG_WUTF);
		/* Clear Wakeup flag*/ 
    PWR_ClearFlag(PWR_FLAG_WU);
//		GPIO_ToggleBits(RUNLED_PORT,RUNLED_PIN);
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		
		
	}
}


