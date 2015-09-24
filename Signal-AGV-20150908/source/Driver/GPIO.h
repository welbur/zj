

#ifndef GPIO_H
#define GPIO_H

#include "Global_Config.h"


#define LT_FR_PIN     		  	GPIO_Pin_5      /*�����������ת����*/
#define LT_FR_PORT         	  GPIOA

#define LT_EN_PIN             GPIO_Pin_6    /*�������ʹ�ܿ���*/
#define LT_EN_PORT			  		GPIOA

#define LT_BK_PIN             GPIO_Pin_7     /*�������ɲ������*/
#define LT_BK_PORT			  		GPIOA

#define L_FR_PIN             	GPIO_Pin_3     /*��ת�������ת����*/
#define L_FR_PORT							GPIOC

#define L_EN_PIN             	GPIO_Pin_5     /*��ת���ʹ�ܿ���*/
#define L_EN_PORT							GPIOC

#define L_BK_PIN             	GPIO_Pin_0     /*��ת���ɲ������*/
#define L_BK_PORT							GPIOB

#define R_FR_PIN             	GPIO_Pin_8     /*��ת�������ת����*/
#define R_FR_PORT							GPIOE

#define R_EN_PIN             	GPIO_Pin_9     /*��ת���ʹ�ܿ���*/
#define R_EN_PORT							GPIOE

#define R_BK_PIN             	GPIO_Pin_10    /*��ת���ɲ������*/
#define R_BK_PORT							GPIOE

#define IR_Far_PIN             	GPIO_Pin_2   /*����Զ�ж�2*/
#define IR_Far_PORT							GPIOA
#define IR_Far_EXIT_PORT        EXTI_PortSourceGPIOA
#define IR_Far_EXIT_PIN         EXTI_PinSource2

#define IR_Near_PIN             GPIO_Pin_3   /*������ж�3*/
#define IR_Near_PORT					  GPIOA
#define IR_Near_EXIT_PORT       EXTI_PortSourceGPIOA
#define IR_Near_EXIT_PIN        EXTI_PinSource3

#define LRS_Far_PIN             GPIO_Pin_0   /*���Ҵ�����Զ�ж�0*/
#define LRS_Far_PORT					  GPIOA
#define LRS_Far_EXIT_PORT       EXTI_PortSourceGPIOA
#define LRS_Far_EXIT_PIN        EXTI_PinSource0

#define LRS_Near_PIN            GPIO_Pin_1   /*���Ҵ������ж�1*/
#define LRS_Near_PORT			      GPIOA
#define LRS_Near_EXIT_PORT      EXTI_PortSourceGPIOA
#define LRS_Near_EXIT_PIN       EXTI_PinSource1

//#define LT_Up_PIN            		GPIO_Pin_2   /*�������������ж�1*/
//#define LT_Up_PORT					 		GPIOC
//#define LT_Up_EXIT_PORT      		EXTI_PortSourceGPIOC
//#define LT_Up_EXIT_PIN       		EXTI_PinSource2

//#define LT_Down_PIN           	GPIO_Pin_3   /*�������������ж�1*/
//#define LT_Down_PORT						GPIOC
//#define LT_Down_EXIT_PORT      	EXTI_PortSourceGPIOC
//#define LT_Down_EXIT_PIN       	EXTI_PinSource3

#define LT_Up_PIN            		GPIO_Pin_5   /*�������������ж�1*/
#define LT_Up_PORT					 		GPIOE
#define LT_Up_EXIT_PORT      		EXTI_PortSourceGPIOE
#define LT_Up_EXIT_PIN       		EXTI_PinSource5

#define LT_Down_PIN           	GPIO_Pin_6   /*�������������ж�1*/
#define LT_Down_PORT						GPIOE
#define LT_Down_EXIT_PORT      	EXTI_PortSourceGPIOE
#define LT_Down_EXIT_PIN       	EXTI_PinSource6

/////////////////����������ָʾ,�ܽŻ�δ����/////////////////


#define STOP_PIN             	GPIO_Pin_4     /*����ֹͣ��ť�ж�4*/
#define STOP_PORT							GPIOA
#define STOP_EXIT_PORT      	EXTI_PortSourceGPIOA
#define STOP_EXIT_PIN       	EXTI_PinSource4

#define KEY1_PIN              GPIO_Pin_12   /*��������1�ж�12*/
#define KEY1_PORT						  GPIOD
#define KEY1_EXIT_PORT        EXTI_PortSourceGPIOD
#define KEY1_EXIT_PIN         EXTI_PinSource12

#define KEY2_PIN              GPIO_Pin_13   /*��������2�ж�13*/
#define KEY2_PORT							GPIOD
#define KEY2_EXIT_PORT      	EXTI_PortSourceGPIOD
#define KEY2_EXIT_PIN       	EXTI_PinSource13

#define KEY3_PIN           	 	GPIO_Pin_14   /*��������3�ж�14*/
#define KEY3_PORT							GPIOD
#define KEY3_EXIT_PORT      	EXTI_PortSourceGPIOD
#define KEY3_EXIT_PIN       	EXTI_PinSource14

#define KEY4_PIN            	GPIO_Pin_15   /*��������4�ж�15*/
#define KEY4_PORT							GPIOD
#define KEY4_EXIT_PORT      	EXTI_PortSourceGPIOD
#define KEY4_EXIT_PIN       	EXTI_PinSource15


#define LED_R_PIN             GPIO_Pin_6   /*LED RGB ����*/
#define LED_R_PORT						GPIOC

#define LED_G_PIN             GPIO_Pin_7
#define LED_G_PORT						GPIOC

#define LED_B_PIN             GPIO_Pin_8
#define LED_B_PORT						GPIOC

#define WORK_LED_PIN          GPIO_Pin_15  /*�������е�*/
#define WORK_LED_PORT					GPIOE

#define VOICE_CTR1_PIN        GPIO_Pin_8   /*��������*/
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
		u16 LRS_Far_Flag:1;       //���Ҵ�����Զ�ж�
		u16 LRS_Near_Flag:1;      //���Ҵ��������ж�
		u16 IR_Far_Flag:1;        //���⴫����Զ�ж�
		u16 IR_Near_Flag:1;       //���⴫�������ж�
		u16 STOP_Flag:1;          //������ť�ж�
		u16 LT_UP_Flag:1;         //�������������ж�
		u16 LT_DOWN_Flag:1;       //�������������ж�
		u16 KEY1_Flag:1;          //����1�ж�
		u16 KEY2_Flag:1;          //����2�ж�
		u16 KEY3_Flag:1;          //����3�ж�
		u16 KEY4_Flag:1;          //����4�ж�
	}BITS;
	u16 ALL_INT;
}_INT_EVENT;
extern _INT_EVENT INT_Event;

void IO_Init(void);

#endif




