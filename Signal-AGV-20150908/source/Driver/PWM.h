
#ifndef __PWM_H
#define __PWM_H

#include "Global_Config.h"
#include "Driver_Include.h"

/*******************************************************************************
* Function Name  : Tim1_CH2_PWM_OUT
* Description    : ����TIM1��ͨ��2ΪPWM���
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM1_CH2_PWM_OUT(u32 PWM_Freq,s8 Duty);
/*******************************************************************************
* Function Name  : Tim1_CH2_PWM_OUT
* Description    : ����TIM3��ͨ��4ΪPWM���
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
//void TIM3_CH4_PWM_OUT(u32 PWM_Freq,s8 Duty);
void TIM3_ENC_Init(void);
void TIM1_ENC_Init(void);


/*******************************************************************************
* Function Name  : TIM4_CH3_CH4_PWM_IN
* Description    : ����TIM4��ͨ��3,4��PWM����
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM4_CH3_CH4_PWM_IN(void);

enum
{
	L_PG_INDEX = 0,
	R_PG_INDEX = 1
};

extern uint32_t LRMotoFreq[];
extern uint32_t sensorFreq;

#endif

