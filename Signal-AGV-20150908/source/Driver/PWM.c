
/*
*PWM 输出管脚 PB1 -->> 左转电机转速调节  TIM3_CH4
*            PE11 -->> 右转电机转速调节  TIM1_CH2
*PWM 输入管脚 PB8 <<-- 左转电机转速指示		TIM4_CH3	
*             PB9 <<-- 右转电机转速指示  TIM4_CH4
*						 
*/

#include "PWM.h"



/*********************************************************************************************************
*	函 数 名: TIM1_ENC_Init
*	功能说明: 
*	形    参：
*	返 回 值: 
*********************************************************************************************************/

void TIM1_ENC_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

 

  
  //GPIO_StructInit(&GPIO_InitStructure);
  /* Configure PC.06,07 as encoder input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_TIM1);
//     GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM1);
  
  /* Enable the TIM3 Update Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */ 
  //TIM_DeInit(TIM3);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 60000; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM1, &TIM_ICInitStructure);
  
  // Clear all pending interrupts
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  //Reset counter
  //TIM2->CNT = 100;
  
  // ENC_Clear_Speed_Buffer();
  
  TIM_Cmd(TIM1, ENABLE); 
}

#if 0
/*
*********************************************************************************************************
*	函 数 名: TIM1_PWM_CLK_Config
*	功能说明: 设置PWM的频率
*	形    参：Freq:频率
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM1_PWM_CLK_Config(u32 Freq,s8 Duty)
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
	//Freq = Freq /2;//定时器频率为输出频率 / 2
	RCC_GetClocksFreq(&RCC_Clocks);
	if(Freq < 1000)//32位定时与16位定时通用,保证计数值不超过65535
	{
		if(RCC_Clocks.HCLK_Frequency < 64000000)
			Prescaler = 100;
		else
			Prescaler = 1000;
	}
//	else if(RCC_Clocks.HCLK_Frequency > 64000000 && Freq < 10000)
//		Prescaler = 100;
	else
		Prescaler = 1000;
  Prescaler = 100;
	TimerPeriod = (RCC_Clocks.PCLK2_Frequency*2 / (Freq * Prescaler)) /*-1*/;
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
	
		
	ccr = Duty * TimerPeriod / 100;
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
void TIM1_CH2_PWM_OUT(u32 PWM_Freq,s8 Duty)
{
	u8 pwm_flag = 0;
	//PE11对应TIM1的CH2通道正相输出
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	if(Duty < 5 || Duty > 99)
	{
		TIM_Cmd(TIM1,DISABLE);
		TIM_CtrlPWMOutputs(TIM1,DISABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		if(Duty < 5)
			GPIO_ResetBits(GPIOE,GPIO_Pin_11);
		else
			GPIO_SetBits(GPIOE,GPIO_Pin_11);
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOE,&GPIO_InitStructure);		
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);		
		pwm_flag++;
	}
	if(pwm_flag)
	{
		TIM1_PWM_CLK_Config(PWM_Freq,Duty);
	}
}
#endif

/*********************************************************************************************************
*	函 数 名: TIM3_ENC_Init
*	功能说明: 
*	形    参：
*	返 回 值: 
*********************************************************************************************************/

void TIM3_ENC_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

 

  
  //GPIO_StructInit(&GPIO_InitStructure);
  /* Configure PC.06,07 as encoder input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
     GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
  
  /* Enable the TIM3 Update Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */ 
  //TIM_DeInit(TIM3);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 60000; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  
  // Clear all pending interrupts
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  //Reset counter
  //TIM2->CNT = 100;
  
  // ENC_Clear_Speed_Buffer();
  
  TIM_Cmd(TIM3, ENABLE); 
}


#if 0
/*
*********************************************************************************************************
*	函 数 名: TIM3_PWM_CLK_Config
*	功能说明: 设置PWM的频率
*	形    参：Freq:频率
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM3_PWM_CLK_Config(u32 Freq,s8 Duty)
{
  u32 TimerPeriod,Prescaler,ccr;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	u8 i,j;
	
	RCC_ClocksTypeDef RCC_Clocks;
	//NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;


	if(Freq == 0)
		return;
	//Freq = Freq /2;//定时器频率为输出频率 / 2
	RCC_GetClocksFreq(&RCC_Clocks);
	
	
	if(Freq < 1000)//32位定时与16位定时通用,保证计数值不超过65535
	{
		if(RCC_Clocks.HCLK_Frequency < 64000000)
			Prescaler = 100;
		else 
			Prescaler = 1000;
	}
	else if(RCC_Clocks.HCLK_Frequency > 64000000 && Freq < 10000)
		Prescaler = 1000;
	else
		Prescaler = 100;
	Prescaler = 100;

	TimerPeriod = (RCC_Clocks.PCLK1_Frequency*2 / (Freq * Prescaler)) /*-1*/;
	
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  //时基初始化
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
  TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler - 1;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = TimerPeriod - 1;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//输出同相，TIM_OCNPolarity_High时输出反相;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
		
	ccr = Duty * TimerPeriod / 100;
	TIM_OCInitStructure.TIM_Pulse = ccr;

	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	
  TIM_Cmd(TIM3,ENABLE);

}

/*******************************************************************************
* Function Name  : TIM3_CH4_PWM_OUT
* Description    : 配置TIM3的通道4为PWM输出
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM3_CH4_PWM_OUT(u32 PWM_Freq,s8 Duty)
{
	u8 pwm_flag = 0;
	//PB1对应TIM1的CH2通道正相输出
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	if(Duty < 5 || Duty > 99)
	{
		TIM_Cmd(TIM3,DISABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		if(Duty < 5)
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		else
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOB,&GPIO_InitStructure);		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);		
		pwm_flag++;
	}
	if(pwm_flag)
	{
		TIM3_PWM_CLK_Config(PWM_Freq,Duty);
	}
}

#endif


/*******************************************************************************
* Function Name  : TIM4_CH3_CH4_PWM_IN
* Description    : 捕获TIM4的通道3,4的PWM输入
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/

#if 0
void TIM4_CH3_CH4_PWM_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_ClocksTypeDef RCC_Clocks;
	u32 TimerPeriod,Prescaler;
	
	

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* TIM4 chennel3,4 configuration : PB.08 09 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
/* --------------------------------------------------------------------------- 
    TIM4 configuration: PWM Input mode

    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM4CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM4CLK = HCLK / 2 = SystemCoreClock /2

    External Signal Frequency = TIM4 counter clock / TIM4_CCR2 in Hz. 

    External Signal DutyCycle = (TIM4_CCR1*100)/(TIM4_CCR2) in %.

  --------------------------------------------------------------------------- */
  
	/*定时频率固定在1MHZ*/
	
	RCC_GetClocksFreq(&RCC_Clocks);
	Prescaler = 100;/*RCC_Clocks.PCLK1_Frequency * 2 / 20000*/;
	
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
  TIM_TimeBaseStructure.TIM_Prescaler = /*0;*/Prescaler - 1;//时钟分频,25MHZ / 1HZ = 25
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;/*TIM_ICPolarity_Rising;*/
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

  //TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

  /* Select the TIM4 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1 | TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM4, /*TIM_SlaveMode_Trigger*/TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);
//	TIM_ITRxExternalClockConfig(TIM4,TIM_TS_ETRF); 
//	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);

  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);


  /* Enable the CC2 Interrupt Request */
   TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	 TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
}
#endif


 uint16_t ReadVal1[2];
 uint16_t ReadVal2[2];
 uint16_t CaptureNum[2];
 uint32_t Capture[2];
 uint32_t Csensor;
 uint32_t LRMotoFreq[2];//左右电机输出的频率
 uint32_t sensorFreq;

//__IO uint16_t IC2Value = 0;
//__IO uint16_t DutyCycle = 0;
//__IO uint32_t Frequency = 0;

void TIM4_IRQHandler(void)
{
	int i,j;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);

	if(TIM_GetITStatus(TIM4,TIM_IT_CC3))
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		ReadVal1[0] = TIM_GetCapture3(TIM4);
		printf("ReadVal1 is %d\r\n",ReadVal1[L_PG_INDEX]);
#if 0
		if(CaptureNum[L_PG_INDEX] == 0)
    	{
     		 /* Get the Input Capture value */
      		ReadVal1[0] = TIM_GetCapture3(TIM4);
			 printf("ReadVal1 is %d\r\n",ReadVal1[L_PG_INDEX]);
      		CaptureNum[L_PG_INDEX]  = 1;
    	}
    	else if(CaptureNum[L_PG_INDEX]  == 1)
    	{
      		/* Get the Input Capture value */
      		ReadVal2[L_PG_INDEX] = TIM_GetCapture3(TIM4); 
      		printf("ReadVal2 is %d\r\n",ReadVal2[L_PG_INDEX]);
     		 /* Capture computation */
      		if (ReadVal2[L_PG_INDEX] > ReadVal1[L_PG_INDEX])
      		{
        		Capture[L_PG_INDEX] = (ReadVal2[L_PG_INDEX] - ReadVal1[L_PG_INDEX]); 
				Csensor = Capture[L_PG_INDEX];
     		}
      		else if (ReadVal2[L_PG_INDEX] < ReadVal1[L_PG_INDEX])
      		{
        		Capture[L_PG_INDEX] = ((0xFFFF - ReadVal1[L_PG_INDEX]) + ReadVal2[L_PG_INDEX]); 
				Csensor = Capture[L_PG_INDEX];
      		}
      		else
      		{
        		Capture[L_PG_INDEX] = 0;
				Csensor = Capture[L_PG_INDEX];
      		}
      		/* Frequency computation */ 
      		LRMotoFreq[L_PG_INDEX] = (uint32_t) (RCC_Clocks.HCLK_Frequency)/2/ Capture[L_PG_INDEX];
			sensorFreq = LRMotoFreq[L_PG_INDEX];
      		CaptureNum[L_PG_INDEX]  = 0;
				for(j=0;j<0x00f0;j++)
					for(i=0;i<0xfff0;i++);
			i=0;
			j=0;
				
    	}
#endif

		
	}

	else if(TIM_GetITStatus(TIM4,TIM_IT_CC4))
	{
/* Clear TIM1 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
    if(CaptureNum[R_PG_INDEX] == 0)
    {
      /* Get the Input Capture value */
      ReadVal1[R_PG_INDEX] = TIM_GetCapture4(TIM4);
      CaptureNum[R_PG_INDEX]  = 1;
    }
    else if(CaptureNum[R_PG_INDEX]  == 1)
    {
      /* Get the Input Capture value */
      ReadVal2[R_PG_INDEX] = TIM_GetCapture4(TIM4); 
      
      /* Capture computation */
      if (ReadVal2[R_PG_INDEX] > ReadVal1[R_PG_INDEX])
      {
        Capture[R_PG_INDEX] = (ReadVal2[R_PG_INDEX] - ReadVal1[R_PG_INDEX]); 
      }
      else if (ReadVal2[R_PG_INDEX] < ReadVal1[R_PG_INDEX])
      {
        Capture[R_PG_INDEX] = ((0xFFFF - ReadVal1[R_PG_INDEX]) + ReadVal2[R_PG_INDEX]); 
      }
      else
      {
        Capture[R_PG_INDEX] = 0;
      }
      /* Frequency computation */ 
      LRMotoFreq[R_PG_INDEX] = (uint32_t) (RCC_Clocks.HCLK_Frequency)/100/2/ Capture[R_PG_INDEX];
      CaptureNum[R_PG_INDEX]  = 0;
    }
	}
	

//  /* Clear TIM4 Capture compare interrupt pending bit */
//  TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

//  /* Get the Input Capture value */
//  IC2Value = TIM_GetCapture2(TIM4);

//  if (IC2Value != 0)
//  {
//    /* Duty cycle computation */
//    DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;

//    /* Frequency computation 
//       TIM4 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

//    Frequency = (RCC_Clocks.HCLK_Frequency)/2 / IC2Value;
//  }
//  else
//  {
//    DutyCycle = 0;
//    Frequency = 0;
//  }	
}
