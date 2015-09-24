
/*
*PWM 输出管脚 PB1 -->> 左转电机转速调节  TIM3_CH4
*            PE11 -->> 右转电机转速调节  TIM1_CH2
*PWM 输入管脚 PB8 <<-- 左转电机转速指示		TIM4_CH3	
*             PB9 <<-- 右转电机转速指示  TIM4_CH4
*						 
*/

#include "PWM.h"

/*
*********************************************************************************************************
*	函 数 名: TIM1_PWM_CLK_Config
*	功能说明: 设置PWM的频率
*	形    参：Freq:频率
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM1_CLK_Config(u32 Freq,u8 *Duty)
{
  u32 TimerPeriod;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	u32 Prescaler;
	u32 ccr;
//	u8 i,j;
	
	RCC_ClocksTypeDef RCC_Clocks;
	//NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;


	if(Freq == 0)
		return;
	Freq = Freq /2;//定时器频率为输出频率 / 2
	RCC_GetClocksFreq(&RCC_Clocks);
	if(Freq < 1000)//32位定时与16位定时通用,保证计数值不超过65535
	{
		if(RCC_Clocks.HCLK_Frequency < 64000000)
			Prescaler = 10;
		else
			Prescaler = 100;
	}
//	else if(RCC_Clocks.HCLK_Frequency > 64000000 && Freq < 10000)
//		Prescaler = 100;
	else
		Prescaler = 10;
	if(Prescaler)
		TimerPeriod = (RCC_Clocks.PCLK2_Frequency / (Freq * Prescaler)) /*-1*/;
	else
		TimerPeriod = (RCC_Clocks.PCLK2_Frequency / Freq) /*-1*/;
  //TimerPeriod =  (SystemCoreClock / 20000 ) - 1;
	
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  //时基初始化
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
  TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler - 1;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = TimerPeriod - 1;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//输出同相，TIM_OCNPolarity_High时输出反相;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
		
	ccr = Duty[0] * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
//	ccr = Duty[1] * TimerPeriod / 100;
//	TIM_OCInitStructure.TIM_Pulse = ccr;
//	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	
	ccr = Duty[2] * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	
	ccr = Duty[3] * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//输出同相，TIM_OCNPolarity_High时输出反相;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	ccr = Duty[1] * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

/*******************************************************************************
* Function Name  : Tim1_CH2_PWM_OUT
* Description    : 配置TIM1的通道2为PWM输出
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM1_CH1_4_PWM_OUT(u32 PWM_Freq,u8 *Duty)
{
	u8 pwm_flag = 0;
	//PE11对应TIM1的CH2通道正相输出
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
		GPIO_Init(GPIOE,&GPIO_InitStructure);		
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);		
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1);	
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);	
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);	
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);	
		pwm_flag++;
	}
	if(pwm_flag)
	{
		TIM1_CLK_Config(PWM_Freq,Duty);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TIM2_PWM_CLK_Config
*	功能说明: 设置PWM的频率
*	形    参：Freq:频率
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_PWM_CLK_Config(u32 Freq,u8 *Duty)
{
  u32 TimerPeriod,Prescaler,ccr;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	u8 i,j;
	
	RCC_ClocksTypeDef RCC_Clocks;
	//NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;


	if(Freq == 0)
		return;
	Freq = Freq /2;//定时器频率为输出频率 / 2
	RCC_GetClocksFreq(&RCC_Clocks);
	
	
	if(Freq < 1000)//32位定时与16位定时通用,保证计数值不超过65535
	{
		if(RCC_Clocks.HCLK_Frequency < 64000000)
			Prescaler = 10;
		else 
			Prescaler = 100;
	}
	else if(RCC_Clocks.HCLK_Frequency > 64000000 && Freq < 10000)
		Prescaler = 100;
	else
		Prescaler = 10;
	if(Prescaler)
		TimerPeriod = (RCC_Clocks.PCLK1_Frequency / (Freq * Prescaler)) /*-1*/;
	else
		TimerPeriod = (RCC_Clocks.PCLK1_Frequency / Freq) /*-1*/;
  //TimerPeriod =  (SystemCoreClock / 20000 ) - 1;
	
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  //时基初始化
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
  TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler - 1;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = TimerPeriod - 1;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//输出同相，TIM_OCNPolarity_High时输出反相;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
		
	ccr = Duty[0] * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	ccr = Duty[1] * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
  TIM_Cmd(TIM2,ENABLE);

}

/*******************************************************************************
* Function Name  : TIM2_CH4_PWM_OUT
* Description    : 配置TIM2的通道4为PWM输出
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM2_CH1_2_PWM_OUT(u32 PWM_Freq,u8 *Duty)
{
	u8 pwm_flag = 0;
	//PA0,1对应TIM1的CH2通道正相输出
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
		GPIO_Init(GPIOA,&GPIO_InitStructure);		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);		
		pwm_flag++;
	}
	if(pwm_flag)
	{
		TIM2_PWM_CLK_Config(PWM_Freq,Duty);
	}
}
/*******************************************************************************
* Function Name  : TIM1_CH1_4_PWM_IN
* Description    : 捕获TIM3的通道3,4的PWM输入
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM3_CH1_4_PWM_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	RCC_ClocksTypeDef RCC_Clocks;
//	u32 TimerPeriod,Prescaler;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* TIM3 chennel3,4 configuration : PB.08 09 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* TIM3 chennel3,4 configuration : PB.08 09 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
/* --------------------------------------------------------------------------- 
    TIM3 configuration: PWM Input mode

    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2

    External Signal Frequency = TIM3 counter clock / TIM3_CCR2 in Hz. 

    External Signal DutyCycle = (TIM3_CCR1*100)/(TIM3_CCR2) in %.

  --------------------------------------------------------------------------- */
  
	/*定时频率固定在1MHZ*/
	
//	RCC_GetClocksFreq(&RCC_Clocks);
//	Prescaler = RCC_Clocks.PCLK1_Frequency / 1000000;
//	
//	
//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
//  TIM_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;//时钟分频,25MHZ / 1HZ = 25
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

  //TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

  /* Select the TIM3 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1 | TIM_TS_TI2FP2/*TIM_TS_TI2FP2*/);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC2 Interrupt Request */
   TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
	 TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	 TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	 TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
}

 uint16_t lReadVal1[4];
 uint16_t lReadVal2[4];
 uint16_t lCaptureNum[4];
 uint32_t lCapture[4];
 uint32_t lLRMotoFreq[4];//左右电机输出的频率

void TIM3_IRQHandler(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  if(TIM_GetITStatus(TIM3,TIM_IT_CC1))
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		if(lCaptureNum[0] == 0)
    {
      /* Get the Input Capture value */
      lReadVal1[0] = TIM_GetCapture1(TIM3);
      lCaptureNum[0]  = 1;
    }
    else if(lCaptureNum[0]  == 1)
    {
      /* Get the Input Capture value */
      lReadVal2[0] = TIM_GetCapture1(TIM3); 
      
      /* Capture computation */
      if (lReadVal2[0] > lReadVal1[0])
      {
        lCapture[0] = (lReadVal2[0] - lReadVal1[0]); 
      }
      else if (lReadVal2[0] < lReadVal1[0])
      {
        lCapture[0] = ((0xFFFF - lReadVal1[0]) + lReadVal2[0]); 
      }
      else
      {
        lCapture[0] = 0;
      }
      /* Frequency computation */ 
      lLRMotoFreq[0] = (uint32_t) (RCC_Clocks.HCLK_Frequency)/2/ lCapture[0];
      lCaptureNum[0]  = 0;
    }
	}
	else if(TIM_GetITStatus(TIM3,TIM_IT_CC2))
	{
/* Clear TIM1 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    if(lCaptureNum[1] == 0)
    {
      /* Get the Input Capture value */
      lReadVal1[1] = TIM_GetCapture2(TIM3);
      lCaptureNum[1]  = 1;
    }
    else if(lCaptureNum[1]  == 1)
    {
      /* Get the Input Capture value */
      lReadVal2[1] = TIM_GetCapture2(TIM3); 
      
      /* Capture computation */
      if (lReadVal2[1] > lReadVal1[1])
      {
        lCapture[1] = (lReadVal2[1] - lReadVal1[1]); 
      }
      else if (lReadVal2[1] < lReadVal1[1])
      {
        lCapture[1] = ((0xFFFF - lReadVal1[1]) + lReadVal2[1]); 
      }
      else
      {
        lCapture[1] = 0;
      }
      /* Frequency computation */ 
      lLRMotoFreq[1] = (uint32_t) (RCC_Clocks.HCLK_Frequency)/2/ lCapture[1];
      lCaptureNum[1]  = 0;
    }
	}
	
	if(TIM_GetITStatus(TIM3,TIM_IT_CC3))
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		if(lCaptureNum[2] == 0)
    {
      /* Get the Input Capture value */
      lReadVal1[2] = TIM_GetCapture3(TIM3);
      lCaptureNum[2]  = 1;
    }
    else if(lCaptureNum[2]  == 1)
    {
      /* Get the Input Capture value */
      lReadVal2[2] = TIM_GetCapture3(TIM3); 
      
      /* Capture computation */
      if (lReadVal2[2] > lReadVal1[2])
      {
        lCapture[2] = (lReadVal2[2] - lReadVal1[2]); 
      }
      else if (lReadVal2[2] < lReadVal1[2])
      {
        lCapture[2] = ((0xFFFF - lReadVal1[2]) + lReadVal2[2]); 
      }
      else
      {
        lCapture[2] = 0;
      }
      /* Frequency computation */ 
      lLRMotoFreq[2] = (uint32_t) (RCC_Clocks.HCLK_Frequency)/2/ lCapture[2];
      lCaptureNum[2]  = 0;
    }
	}
	else if(TIM_GetITStatus(TIM3,TIM_IT_CC4))
	{
/* Clear TIM1 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    if(lCaptureNum[3] == 0)
    {
      /* Get the Input Capture value */
      lReadVal1[3] = TIM_GetCapture4(TIM3);
      lCaptureNum[3]  = 1;
    }
    else if(lCaptureNum[3]  == 1)
    {
      /* Get the Input Capture value */
      lReadVal2[3] = TIM_GetCapture4(TIM3); 
      
      /* Capture computation */
      if (lReadVal2[3] > lReadVal1[3])
      {
        lCapture[3] = (lReadVal2[3] - lReadVal1[3]); 
      }
      else if (lReadVal2[3] < lReadVal1[3])
      {
        lCapture[3] = ((0xFFFF - lReadVal1[3]) + lReadVal2[3]); 
      }
      else
      {
        lCapture[3] = 0;
      }
      /* Frequency computation */ 
      lLRMotoFreq[3] = (uint32_t) (RCC_Clocks.HCLK_Frequency)/2/ lCapture[3];
      lCaptureNum[3]  = 0;
    }
	}
}


