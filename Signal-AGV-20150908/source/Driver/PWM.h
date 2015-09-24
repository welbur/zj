
#ifndef __PWM_H
#define __PWM_H

#include "Global_Config.h"
#include "Driver_Include.h"

/*******************************************************************************
* Function Name  : Tim1_CH2_PWM_OUT
* Description    : 配置TIM1的通道2为PWM输出
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TIM1_CH2_PWM_OUT(u32 PWM_Freq,s8 Duty);
/*******************************************************************************
* Function Name  : Tim1_CH2_PWM_OUT
* Description    : 配置TIM3的通道4为PWM输出
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
//void TIM3_CH4_PWM_OUT(u32 PWM_Freq,s8 Duty);
void TIM3_ENC_Init(void);
void TIM1_ENC_Init(void);


/*******************************************************************************
* Function Name  : TIM4_CH3_CH4_PWM_IN
* Description    : 捕获TIM4的通道3,4的PWM输入
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

