
#include "Moto_Ctrl.h"


_MOTO_Status L_Moto_Status,R_Moto_Status;//当前电机的状态

int8_t Click_Break_Timer_Index = 1;//点刹定时器
uint8_t Click_Break_Count = 0;

/****************************************************************************
* 名    称：void Get_Moto_Speed(void)
* 功    能：获取马达的转速
* 参    数：无
* 反    回：无
* 说    明：取左右电机转速的平均值
****************************************************************************/
uint32_t Get_Moto_Speed(void)
{
	L_Moto_Status.PG = LRMotoFreq[L_PG_INDEX];
	R_Moto_Status.PG = LRMotoFreq[R_PG_INDEX];
	
	L_Moto_Status.EN = 
	
	LRMotoFreq[L_PG_INDEX]  = 0;
	LRMotoFreq[R_PG_INDEX]  = 0;
	return (L_Moto_Status.PG  + R_Moto_Status.PG) / 2;
	
}
/****************************************************************************
* 名    称：void Set_Moto_Speed(void)
* 功    能：设置马达的转速
* 参    数：左右电机占空比,10表示10%的占空比
* 反    回：无
* 说    明：固定频率,改变占空比来改变马达的转速
*当占空比小于5%,直接输出低电平,当占空比大于99,直接输出高电平
****************************************************************************/
void Set_Moto_Speed(int8_t L_Duty,int8_t R_Duty)
{
	if(L_Duty != L_Moto_Status.PWM || R_Duty != R_Moto_Status.PWM)
	{
//		TIM3_CH4_PWM_OUT(MOTO_PWM_FREQ,L_Duty);
//		TIM1_CH2_PWM_OUT(MOTO_PWM_FREQ,R_Duty);
		
		L_Moto_Status.PWM = L_Duty;
		R_Moto_Status.PWM = R_Duty;
	}
}
/****************************************************************************
* 名    称：InitLRMotoCtrl
* 功    能：初始化马达控制
* 参    数：无
* 反    回：无
* 说    明：上电使之处于停止状态
****************************************************************************/
void InitLRMotoCtrl(void)
{
	Set_Moto_Speed(PWM_STOP_DUTY,PWM_STOP_DUTY);
	L_EN_DISABLE;
	L_BK_DISABLE;
	L_FR_FORWARD;
	
	R_EN_DISABLE;
	R_BK_DISABLE;
	R_FR_FORWARD;
	
	TIM1_ENC_Init();

	TIM3_ENC_Init();
//	TIM4_CH3_CH4_PWM_IN();
}
/****************************************************************************
* 名    称：void StopMoto(void)
* 功    能：停止马达
* 参    数： 
* 反    回：无
* 说    明：
****************************************************************************/
void StopMoto(void)
{
	Set_Moto_Speed(PWM_STOP_DUTY,PWM_STOP_DUTY);
	L_EN_DISABLE;
	R_EN_DISABLE;
	
}
/****************************************************************************
* 名    称：Click_Break_Timeout
* 功    能：点刹定时器
* 参    数： 
* 反    回：无
* 说    明：
****************************************************************************/
int8_t Click_Break_Timeout(void *cbptr)
{
	Click_Break_Count++;
	if(Click_Break_Count > MAX_CLICK_BREAK_COUNT)
	{
		L_BK_ENABLE;
		R_BK_ENABLE;
		Click_Break_Count = 0;
		if(Click_Break_Timer_Index != -1)
		{
			Free_Timer(&Click_Break_Timer_Index);
		}
	}
	else
	{
		if(Click_Break_Count % 2)
		{
			L_BK_ENABLE;
			R_BK_ENABLE;
		}
		else
		{
			L_BK_DISABLE;
			R_BK_DISABLE;
		}
	}
	return 0;
}
/****************************************************************************
* 名    称：void BrakeMoto(void)
* 功    能：刹车
* 参    数： 
* 反    回：无
* 说    明：
****************************************************************************/
void BrakeMoto(void)
{
	uint8_t i;
  StopMoto();
	if(Get_Moto_Speed() > 500)
	{
		for(i = 0; i < 20;i++)
		{
			L_BK_DISABLE;
			R_BK_DISABLE;
			Delay(10);
			L_BK_ENABLE;
			R_BK_ENABLE;
			Delay(10);
		}
    
		L_BK_ENABLE;
		R_BK_ENABLE;
	}
	else
	{
		L_BK_ENABLE;
		R_BK_ENABLE;
		Delay(10);
		L_BK_DISABLE;
		R_BK_DISABLE;
		Delay(10);
		L_BK_ENABLE;
		R_BK_ENABLE;
	}
		
}

/****************************************************************************
* 名    称：void SetMotoForward(void)
* 功    能：设置马达为前进
* 参    数： 
* 反    回：无
* 说    明：
****************************************************************************/
void SetMotoForward(void)
{
	//如果原来电机为反转,应该先刹车,等待转速为0后再切换到正转
	if(GPIO_ReadOutputDataBit(L_FR_PORT,L_FR_PIN) == Bit_RESET /*|| GPIO_ReadOutputDataBit(R_FR_PORT,R_FR_PIN) == Bit_RESET*/)
	{
		BrakeMoto();
		while(1)
		{
			if(Get_Moto_Speed() < 5)
				break;
			Delay(10);
		}
	}
  L_FR_FORWARD;
	L_BK_DISABLE;
	L_EN_ENABLE;
	
	R_FR_FORWARD;
	R_BK_DISABLE;
	R_EN_ENABLE;
}

/****************************************************************************
* 名    称：void SetMotoBack(void)
* 功    能：设置马达为后退
* 参    数： 
* 反    回：无
* 说    明：
****************************************************************************/
void SetMotoBack(void)
{
	//如果原来电机为正转,应该先刹车,等待转速为0后再切换到反转
	if(GPIO_ReadOutputDataBit(L_FR_PORT,L_FR_PIN) == Bit_SET || GPIO_ReadOutputDataBit(R_FR_PORT,R_FR_PIN) == Bit_SET)
	{
		BrakeMoto();
		while(1)
		{
			if(Get_Moto_Speed() < 5)
				break;
			Delay(10);
		}
	}
  L_FR_BACK;
	L_BK_DISABLE;
	L_EN_ENABLE;
	
	R_FR_BACK;
	R_BK_DISABLE;
	R_EN_ENABLE;
}

/****************************************************************************
* 名    称：void LifterUp(void)
* 功    能：升降杆上升
* 参    数：无
* 反    回：无
* 说    明：无
****************************************************************************/
void LifterUp(void)
{
  LIFTR_EN_ENABLE;
  LIFTR_FOR_ENABLE;
  LIFTR_BREAK_DISABLE;
}

/****************************************************************************
* 名    称：void LifterDown(void)
* 功    能：升降杆上升
* 参    数：无
* 反    回：无
* 说    明：无
****************************************************************************/
void LifterDown(void)
{
  LIFTR_EN_ENABLE;
  LIFTR_BACK_ENABLE;
  LIFTR_BREAK_DISABLE;
}

/****************************************************************************
* 名    称：void LifterStop(void)
* 功    能：升降杆停止
* 参    数：无
* 反    回：无
* 说    明：无
****************************************************************************/
void LifterStop(void)
{
  LIFTR_EN_DISABLE;
  LIFTR_BREAK_DISABLE;
}




