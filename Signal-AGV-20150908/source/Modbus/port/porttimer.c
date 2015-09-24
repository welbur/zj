/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "stm32f4xx.h"
/* ----------------------- static functions ---------------------------------*/
//static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	RCC_ClocksTypeDef RCC_Clocks;
	
       uint16_t PrescalerValue;
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef   NVIC_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
				
				RCC_GetClocksFreq(&RCC_Clocks);
        //PrescalerValue = (uint16_t) (168000000 / 20000) - 1; // 1/20000=50us 
				//PrescalerValue = (uint16_t) (RCC_Clocks.PCLK1_Frequency * 2 / 20000) - 1; // 1/20000=50us 
				PrescalerValue = (uint16_t) (50000000 / 20000) - 1; // 1/20000=50us 
        
        TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;       //5HZ  2000,8400
        TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;    //25Hz 400,8400
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

        TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
				
        TIM_ARRPreloadConfig(TIM12, ENABLE);
        TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE);
        //TIM_Cmd(TIM12, ENABLE);
        
        
	return TRUE;;
}


//inline void
void vMBPortTimersEnable(void)
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
  	TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
	TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM12,0x0000); 
	TIM_Cmd(TIM12, ENABLE);
}

//inline void
void vMBPortTimersDisable(void)
{
    /* Disable any pending timers. */
  	TIM_Cmd(TIM12, DISABLE);
	TIM_SetCounter(TIM12,0x0000); 
	TIM_ITConfig(TIM12, TIM_IT_Update, DISABLE);
	TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
}
void TIM8_BRK_TIM12_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
		prvvTIMERExpiredISR( );
	}
}
/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

