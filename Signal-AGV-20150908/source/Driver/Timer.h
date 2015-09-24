

#ifndef YD_TIMER_H
#define YD_TIMER_H

#include "Global_Config.h"

#define MAX_TIMER_NUM  30


#define	Only_Mode  1              /*һ�ι���ģʽ*/
#define	Auto_Mode  0xffffffff     /*ѭ������ģʽ*/

#define SEC 1000
#define MS   1


typedef struct
{
	volatile u32 TimeNum;  /*��ʱ����*/
	volatile u8 Flag;     /*��ʱ�����־*/
	volatile u32 Count;   /*��ʱʱ��*/
	volatile u32 Preload; /*ѭ����ʱԤװֵ*/
	volatile s32 Address;
	Time_FuncPtr_TypeDef Callback; /*��ʱ��ʱ����Ӧ�Ĵ�����*/
	void *ptr;
}_Timer_Str;




extern _Timer_Str TimeList[];

/*******************************************************************************
* Function Name  : Init_SYSTICK
* Description    : ����SYSTICK������ʽ
* Input          : ����ÿ�� Period���ж�
* Output         : Period
* Return         : None
*******************************************************************************/
void Init_SYSTICK(_TIMER_PERIOD Period);

/*******************************************************************************
* Function Name  : Init_SYS_Timer
* Description    : ��ʼ����ʱ��ʹ��Ӳ����ʱ��
* Input          : ����ÿ�� Period���ж�
* Output         : Period
* Return         : None
*******************************************************************************/
void SYS_Timer_Init(_TIMER_PERIOD Period);
/*******************************************************************************
* Function Name  : SYS_Timer_Close
* Description    : �ر�Ӳ����ʱ��
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void SYS_Timer_Close(void);
/*******************************************************************************
* Function Name  : SYS_Timer_Close
* Description    : �ر�Ӳ����ʱ��
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void SYS_Timer_Open(void);
/*******************************************************************************
* Function Name  : Malloc_Timer
* Description    : ����ʹ�ö�ʱ��
* Input          : timeout:��ʱʱ��;TimeNum:��ʱѭ������cb:��ʱ��ʱ��Ļص�����,cbptr:�ص���������
* Output         : None
* Return         : �ɹ����ض�ʱ������;ʧ�ܷ��� -1
*******************************************************************************/
s8 Malloc_Timer(u32 timeout,u32 TimeNum,Time_FuncPtr_TypeDef cb,void *cbptr, s8 *IndexAddr);
/*******************************************************************************
* Function Name  : Free_Timer
* Description    : �ͷ�ʹ�ö�ʱ��
* Input          : timerindex,�ڶ�ʱ�б��е�λ��
* Output         : None
* Return         : �ɹ����ض�ʱ������;ʧ�ܷ��� -1

* NOTE: ��ʱ��ֻ��ʱһ��,�����ϵͳ���Զ��ͷŸö�ʱ��,
        ��ʱ���ʱ,�����������ʱ����Ҫ���ô˺����ͷŶ�ʱ��

*******************************************************************************/
s8 Free_Timer(s8 *timerindex);
/*******************************************************************************
* Function Name  : TimeDec
* Description    : �ݼ���ʱֵ
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void TimeDec(void);
/*******************************************************************************
* Function Name  : YD_Timer_Check
* Description    : ��ʱ����ʱ���
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
s8 Timer_Check(void);
/*******************************************************************************
* Function Name  : Delay
* Description    : ��ʱ����,���򽫵ȴ���ʱ�����������˺���
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
s8 Delay(u32 timeout);

void Delay_100ns(__IO uint32_t nCount);
void Delay_us(__IO uint32_t nCount);
void Delay_ms(__IO uint32_t nCount);














#endif


