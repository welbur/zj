

#ifndef RTC_H
#define RTC_H

#include "Global_Config.h"

//#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
#define RTC_CLOCK_SOURCE_LSI   /* LSI used as RTC source clock */

#if WATCHDOG_ENABLE
typedef struct
{
	u8 NumofInt;
	u8 NumofRem;
	u8 Count;
}_WATCHDOG_Wakeup_Time;
#endif


extern __IO uint32_t   uwLsiFreq;


void RTC_Config(void);
void YD_SET_RTC(RtcStruct *SetRtc);
void YD_GET_RTC(RtcStruct *SetRtc);
void RTC_Configuration(void);
u32 GetLSIFrequency(void);
void RTC_Wakeup_Init(void);
void RTC_Wakeup_Ctrl(u32 Counter ,FunctionalState NewState);


/*******************************************************************************
* Function Name  : RTC_AlarmA_Set
* Description    : ≈‰÷√RTCƒ÷÷” ±º‰
* Input          : 
* Output         : Period
* Return         : None
*******************************************************************************/
void RTC_AlarmA_Set(void);
/*******************************************************************************
* Function Name  : RTC_AlarmA_Ctrl
* Description    :  πƒ‹/ ß–ß RTCƒ÷÷”÷–∂œ
* Input          : 
* Output         : Period
* Return         : None
*******************************************************************************/
void RTC_AlarmA_Ctrl(FunctionalState NewState);
/*******************************************************************************
* Function Name  : RTC_AlarmA_Init
* Description    : ≈‰÷√RTCƒ÷÷”÷–∂œ
* Input          : 
* Output         : Period
* Return         : None
*******************************************************************************/
void RTC_AlarmA_Init(void);







#endif

