

#ifndef GPIO_H
#define GPIO_H

#include "Global_Config.h"


#define LT_FR_PIN     		  	GPIO_Pin_5      /*升降电机正反转控制*/
#define LT_FR_PORT         	  GPIOA

#define LT_EN_PIN             GPIO_Pin_6    /*升降电机使能控制*/
#define LT_EN_PORT			  		GPIOA

#define LT_BK_PIN             GPIO_Pin_7     /*升降电机刹车控制*/
#define LT_BK_PORT			  		GPIOA

#define L_FR_PIN             	GPIO_Pin_3     /*左转电机正反转控制*/
#define L_FR_PORT							GPIOC

#define L_EN_PIN             	GPIO_Pin_5     /*左转电机使能控制*/
#define L_EN_PORT							GPIOC

#define L_BK_PIN             	GPIO_Pin_0     /*左转电机刹车控制*/
#define L_BK_PORT							GPIOB

#define R_FR_PIN             	GPIO_Pin_8     /*右转电机正反转控制*/
#define R_FR_PORT							GPIOE

#define R_EN_PIN             	GPIO_Pin_9     /*右转电机使能控制*/
#define R_EN_PORT							GPIOE

#define R_BK_PIN             	GPIO_Pin_10    /*右转电机刹车控制*/
#define R_BK_PORT							GPIOE

#define IR_Far_PIN             	GPIO_Pin_2   /*红外远中断2*/
#define IR_Far_PORT							GPIOA
#define IR_Far_EXIT_PORT        EXTI_PortSourceGPIOA
#define IR_Far_EXIT_PIN         EXTI_PinSource2

#define IR_Near_PIN             GPIO_Pin_3   /*红外近中断3*/
#define IR_Near_PORT					  GPIOA
#define IR_Near_EXIT_PORT       EXTI_PortSourceGPIOA
#define IR_Near_EXIT_PIN        EXTI_PinSource3

#define LRS_Far_PIN             GPIO_Pin_0   /*左右传感器远中断0*/
#define LRS_Far_PORT					  GPIOA
#define LRS_Far_EXIT_PORT       EXTI_PortSourceGPIOA
#define LRS_Far_EXIT_PIN        EXTI_PinSource0

#define LRS_Near_PIN            GPIO_Pin_1   /*左右传感器中断1*/
#define LRS_Near_PORT			      GPIOA
#define LRS_Near_EXIT_PORT      EXTI_PortSourceGPIOA
#define LRS_Near_EXIT_PIN       EXTI_PinSource1

//#define LT_Up_PIN            		GPIO_Pin_2   /*升降传感器上中断1*/
//#define LT_Up_PORT					 		GPIOC
//#define LT_Up_EXIT_PORT      		EXTI_PortSourceGPIOC
//#define LT_Up_EXIT_PIN       		EXTI_PinSource2

//#define LT_Down_PIN           	GPIO_Pin_3   /*升降传感器下中断1*/
//#define LT_Down_PORT						GPIOC
//#define LT_Down_EXIT_PORT      	EXTI_PortSourceGPIOC
//#define LT_Down_EXIT_PIN       	EXTI_PinSource3

#define LT_Up_PIN            		GPIO_Pin_5   /*升降传感器上中断1*/
#define LT_Up_PORT					 		GPIOE
#define LT_Up_EXIT_PORT      		EXTI_PortSourceGPIOE
#define LT_Up_EXIT_PIN       		EXTI_PinSource5

#define LT_Down_PIN           	GPIO_Pin_6   /*升降传感器下中断1*/
#define LT_Down_PORT						GPIOE
#define LT_Down_EXIT_PORT      	EXTI_PortSourceGPIOE
#define LT_Down_EXIT_PIN       	EXTI_PinSource6

/////////////////升降传感器指示,管脚还未定义/////////////////


#define STOP_PIN             	GPIO_Pin_4     /*紧急停止按钮中断4*/
#define STOP_PORT							GPIOA
#define STOP_EXIT_PORT      	EXTI_PortSourceGPIOA
#define STOP_EXIT_PIN       	EXTI_PinSource4

#define KEY1_PIN              GPIO_Pin_12   /*按键开关1中断12*/
#define KEY1_PORT						  GPIOD
#define KEY1_EXIT_PORT        EXTI_PortSourceGPIOD
#define KEY1_EXIT_PIN         EXTI_PinSource12

#define KEY2_PIN              GPIO_Pin_13   /*按键开关2中断13*/
#define KEY2_PORT							GPIOD
#define KEY2_EXIT_PORT      	EXTI_PortSourceGPIOD
#define KEY2_EXIT_PIN       	EXTI_PinSource13

#define KEY3_PIN           	 	GPIO_Pin_14   /*按键开关3中断14*/
#define KEY3_PORT							GPIOD
#define KEY3_EXIT_PORT      	EXTI_PortSourceGPIOD
#define KEY3_EXIT_PIN       	EXTI_PinSource14

#define KEY4_PIN            	GPIO_Pin_15   /*按键开关4中断15*/
#define KEY4_PORT							GPIOD
#define KEY4_EXIT_PORT      	EXTI_PortSourceGPIOD
#define KEY4_EXIT_PIN       	EXTI_PinSource15


#define LED_R_PIN             GPIO_Pin_6   /*LED RGB 控制*/
#define LED_R_PORT						GPIOC

#define LED_G_PIN             GPIO_Pin_7
#define LED_G_PORT						GPIOC

#define LED_B_PIN             GPIO_Pin_8
#define LED_B_PORT						GPIOC

#define WORK_LED_PIN          GPIO_Pin_15  /*程序运行灯*/
#define WORK_LED_PORT					GPIOE

#define VOICE_CTR1_PIN        GPIO_Pin_8   /*声音控制*/
#define VOICE_CTR1_PORT				GPIOD

#define VOICE_CTR2_PIN        GPIO_Pin_9
#define VOICE_CTR2_PORT				GPIOD

#define VOICE_CTR3_PIN        GPIO_Pin_10
#define VOICE_CTR3_PORT				GPIOD

#define VOICE_CTR4_PIN        GPIO_Pin_11
#define VOICE_CTR4_PORT				GPIOD



typedef union
{
	struct
	{
		u16 LRS_Far_Flag:1;       //左右传感器远中断
		u16 LRS_Near_Flag:1;      //左右传感器近中断
		u16 IR_Far_Flag:1;        //红外传感器远中断
		u16 IR_Near_Flag:1;       //红外传感器近中断
		u16 STOP_Flag:1;          //紧急按钮中断
		u16 LT_UP_Flag:1;         //升降传感器上中断
		u16 LT_DOWN_Flag:1;       //升降传感器下中断
		u16 KEY1_Flag:1;          //按键1中断
		u16 KEY2_Flag:1;          //按键2中断
		u16 KEY3_Flag:1;          //按键3中断
		u16 KEY4_Flag:1;          //按键4中断
	}BITS;
	u16 ALL_INT;
}_INT_EVENT;
extern _INT_EVENT INT_Event;

void IO_Init(void);

#endif




