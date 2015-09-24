

#include "GPIO.h"


EXTI_InitTypeDef IR_Far_Struct,IR_Near_Struct,LRS_Far_Struct,LRS_Near_Struct,STOP_Struct,\
                 LT_UP_Struct,LT_DOWN_Struct,KEY1_Struct,KEY2_Struct,KEY3_Struct,KEY4_Struct;

_INT_EVENT INT_Event;

void IO_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	RCC_AHB1PeriphClockCmd(/*RCC_AHB1Periph_GPIOA | */RCC_AHB1Periph_GPIOB |  RCC_AHB1Periph_GPIOC | 
	                       RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |	RCC_AHB1Periph_GPIOH , DISABLE);
	
	/* Enable the GPIO Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH, ENABLE);
	 /* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使用外部中断必须使能SYSCFG

  ///////////////////////////////////////////////升降电机控制///////////////////////////////////////
  GPIO_InitStructure.GPIO_Pin   = LT_FR_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	
  GPIO_Init(LT_FR_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LT_FR_PORT,LT_FR_PIN);// 默认正转

	GPIO_InitStructure.GPIO_Pin 	= LT_EN_PIN;
  GPIO_Init(LT_EN_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LT_EN_PORT,LT_EN_PIN);//默认停止
	
	GPIO_InitStructure.GPIO_Pin 	= LT_BK_PIN;
  GPIO_Init(LT_BK_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LT_BK_PORT,LT_BK_PIN);//默认刹车
	
	///////////////////////////////////////////////左转电机控制///////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin   = L_FR_PIN;
  GPIO_Init(L_FR_PORT, &GPIO_InitStructure);
	GPIO_SetBits(L_FR_PORT,L_FR_PIN);// 默认正转

	GPIO_InitStructure.GPIO_Pin 	= L_EN_PIN;
  GPIO_Init(L_EN_PORT, &GPIO_InitStructure);
	GPIO_SetBits(L_EN_PORT,L_EN_PIN);//默认停止
	
	GPIO_InitStructure.GPIO_Pin 	= L_BK_PIN;
  GPIO_Init(L_BK_PORT, &GPIO_InitStructure);
	GPIO_SetBits(L_BK_PORT,L_BK_PIN);//默认刹车
	
	/////////////////////////////////////////////// 右转电机控制///////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin   = R_FR_PIN;
  GPIO_Init(R_FR_PORT, &GPIO_InitStructure);
	GPIO_SetBits(R_FR_PORT,R_FR_PIN);// 默认正转

	GPIO_InitStructure.GPIO_Pin 	= R_EN_PIN;
  GPIO_Init(R_EN_PORT, &GPIO_InitStructure);
	GPIO_SetBits(R_EN_PORT,R_EN_PIN);//默认停止
	
	GPIO_InitStructure.GPIO_Pin 	= R_BK_PIN;
  GPIO_Init(R_BK_PORT, &GPIO_InitStructure);
	GPIO_SetBits(R_BK_PORT,R_BK_PIN);//默认刹车
	
	///////////////////////////////////////////////LED-RGB//////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin 	= WORK_LED_PIN;
  GPIO_Init(WORK_LED_PORT, &GPIO_InitStructure);
	GPIO_SetBits(WORK_LED_PORT,WORK_LED_PIN);//
	
	GPIO_InitStructure.GPIO_Pin 	= LED_R_PIN;
  GPIO_Init(LED_R_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_R_PORT,LED_R_PIN);//
	
	GPIO_InitStructure.GPIO_Pin 	= LED_G_PIN;
  GPIO_Init(LED_G_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_G_PORT,LED_G_PIN);//
	
	GPIO_InitStructure.GPIO_Pin 	= LED_B_PIN;
  GPIO_Init(LED_B_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_B_PORT,LED_B_PIN);//
	
	////////////////////////////////////////////////// 声音控制//////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin 	= VOICE_CTR1_PIN;
  GPIO_Init(VOICE_CTR1_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(VOICE_CTR1_PORT,VOICE_CTR1_PIN);//
	
	GPIO_InitStructure.GPIO_Pin 	= VOICE_CTR2_PIN;
  GPIO_Init(VOICE_CTR2_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(VOICE_CTR2_PORT,VOICE_CTR2_PIN);//
	
	GPIO_InitStructure.GPIO_Pin 	= VOICE_CTR3_PIN;
  GPIO_Init(VOICE_CTR3_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(VOICE_CTR3_PORT,VOICE_CTR3_PIN);//
	
	GPIO_InitStructure.GPIO_Pin 	= VOICE_CTR4_PIN;
  GPIO_Init(VOICE_CTR4_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(VOICE_CTR4_PORT,VOICE_CTR4_PIN);//
	
	///////////////////////////////////////////红外远近传感器中断/////////////////////////////////////////////////
  GPIO_InitStructure.GPIO_Pin   = IR_Far_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(IR_Far_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = IR_Near_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(IR_Near_PORT, &GPIO_InitStructure);
	
	/////////////////////////////////////////左右远近传感器///////////////////////////////////
	GPIO_InitStructure.GPIO_Pin   = LRS_Far_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(LRS_Far_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(LRS_Far_EXIT_PORT, LRS_Far_EXIT_PIN);
  
  LRS_Far_Struct.EXTI_Line		 = EXTI_Line0;
  LRS_Far_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  LRS_Far_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  LRS_Far_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&LRS_Far_Struct);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 /* Generate software interrupt: simulate a falling edge applied on EXTI line 0 */
  //EXTI_GenerateSWInterrupt(EXTI_Line0);
	
	GPIO_InitStructure.GPIO_Pin   = LRS_Near_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(LRS_Near_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(LRS_Near_EXIT_PORT, LRS_Near_EXIT_PIN);
  
  LRS_Near_Struct.EXTI_Line		 = EXTI_Line1;
  LRS_Near_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  LRS_Near_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  LRS_Near_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&LRS_Far_Struct);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 /* Generate software interrupt: simulate a falling edge applied on EXTI line 1 */
  //EXTI_GenerateSWInterrupt(EXTI_Line1);
	
	///////////////////////////////////紧急停止按钮//////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin   = STOP_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(STOP_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(STOP_EXIT_PORT, STOP_EXIT_PIN);
  
  STOP_Struct.EXTI_Line		 = EXTI_Line4;
  STOP_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  STOP_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  STOP_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&STOP_Struct);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 /* Generate software interrupt: simulate a falling edge applied on EXTI line 4 */
  //EXTI_GenerateSWInterrupt(EXTI_Line4);
	
	///////////////////////////////////升降传感器上下中断/////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin   = LT_Up_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(LT_Up_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(LT_Up_EXIT_PORT, LT_Up_EXIT_PIN);
  
  LT_UP_Struct.EXTI_Line		 = EXTI_Line5;
  LT_UP_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  LT_UP_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  LT_UP_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&LT_UP_Struct);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 /* Generate software interrupt: simulate a falling edge applied on EXTI line 5 */
  //EXTI_GenerateSWInterrupt(EXTI_Line5);
	
	GPIO_InitStructure.GPIO_Pin   = LT_Down_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(LT_Down_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(LT_Down_EXIT_PORT, LT_Down_EXIT_PIN);
  
  LT_DOWN_Struct.EXTI_Line		 = EXTI_Line6;
  LT_DOWN_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  LT_DOWN_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  LT_DOWN_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&LT_DOWN_Struct);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 /* Generate software interrupt: simulate a falling edge applied on EXTI line 6 */
  //EXTI_GenerateSWInterrupt(EXTI_Line6);
	///////////////////////////////按键开关1-4////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin   = KEY1_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_Init(KEY1_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(KEY1_EXIT_PORT, KEY1_EXIT_PIN);
  
  KEY1_Struct.EXTI_Line		 = EXTI_Line12;
  KEY1_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  KEY1_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  KEY1_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&KEY1_Struct);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 /* Generate software interrupt: simulate a falling edge applied on EXTI line 12 */
  //EXTI_GenerateSWInterrupt(EXTI_Line12);
	GPIO_InitStructure.GPIO_Pin   = KEY2_PIN;
  GPIO_Init(KEY2_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(KEY2_EXIT_PORT, KEY2_EXIT_PIN);
  
  KEY2_Struct.EXTI_Line		 = EXTI_Line13;
  KEY2_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  KEY2_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  KEY2_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&KEY2_Struct);
	/* Generate software interrupt: simulate a falling edge applied on EXTI line 13 */
  //EXTI_GenerateSWInterrupt(EXTI_Line13);
	GPIO_InitStructure.GPIO_Pin   = KEY3_PIN;
  GPIO_Init(KEY3_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(KEY3_EXIT_PORT, KEY3_EXIT_PIN);
  
  KEY3_Struct.EXTI_Line		 = EXTI_Line14;
  KEY3_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  KEY3_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  KEY3_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&KEY3_Struct);
	/* Generate software interrupt: simulate a falling edge applied on EXTI line 14 */
  //EXTI_GenerateSWInterrupt(EXTI_Line14);
	GPIO_InitStructure.GPIO_Pin   = KEY4_PIN;
  GPIO_Init(KEY4_PORT, &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(KEY4_EXIT_PORT, KEY4_EXIT_PIN);
  
  KEY4_Struct.EXTI_Line		 = EXTI_Line15;
  KEY4_Struct.EXTI_Mode		 = EXTI_Mode_Interrupt;
  KEY4_Struct.EXTI_Trigger  = EXTI_Trigger_Falling;
  KEY4_Struct.EXTI_LineCmd  = ENABLE;
  EXTI_Init(&KEY4_Struct);
	/* Generate software interrupt: simulate a falling edge applied on EXTI line 15 */
  //EXTI_GenerateSWInterrupt(EXTI_Line15);
}
/**
  * @brief  This function handles External lines 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	
  /* Clear the  EXTI line 0 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line0);/*左右传感器远中断*/
	INT_Event.BITS.LRS_Far_Flag = 1;
}

/**
  * @brief  This function handles External lines 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  /* Clear the  EXTI line 1 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line1);/*左右传感器近中断*/
	INT_Event.BITS.LRS_Near_Flag = 1;
}

/**
  * @brief  This function handles External lines 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
	
  /* Clear the  EXTI line 4 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line4);/*紧急停止中断*/
	INT_Event.BITS.STOP_Flag = 1;
}

/**
  * @brief  This function handles External lines 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)/*升降杆上升中断*/
  {
    /* Clear the  EXTI line 5 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line5);
		INT_Event.BITS.LT_UP_Flag = 1;
  }
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)/*升降杆下降中断*/
  {
    /* Clear the  EXTI line 6 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line6);
		INT_Event.BITS.LT_DOWN_Flag = 1;
  }
}


/**
  * @brief  This function handles External lines 15-10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)/*按键1中断*/
  {
    /* Clear the  EXTI line 12 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line12);
		INT_Event.BITS.KEY1_Flag = 1;
  }
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)/*按键2中断*/
  {
    /* Clear the  EXTI line 13 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line13);
		INT_Event.BITS.KEY2_Flag = 1;
  }
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)/*按键3中断*/
  {
    /* Clear the  EXTI line 14 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line14);
		INT_Event.BITS.KEY3_Flag = 1;
  }
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)/*按键4中断*/
  {
    /* Clear the  EXTI line 15 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line15);
		INT_Event.BITS.KEY4_Flag = 1;
  }
}



