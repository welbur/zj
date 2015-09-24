

#ifndef __MOTO_CTRL_H
#define __MOTO_CTRL_H

#include "Driver_Include.h"

#define MOTO_PWM_FREQ 1000  //PWM�̶����2KƵ��
#define PWM_STOP_DUTY 1     //ֹͣPWM���,���ת��Ϊ0 
#define PWM_FULL_DUTY 100   //ֱ������ߵ�ƽ,���ת�����

#define MAX_CLICK_BREAK_COUNT  20//����ɲ����

//��������
#define L_EN_ENABLE       	GPIO_ResetBits(L_EN_PORT,L_EN_PIN)
#define L_EN_DISABLE      	GPIO_SetBits(L_EN_PORT,L_EN_PIN)
#define L_EN_STATUS         GPIO_ReadOutputDataBit(L_EN_PORT,L_EN_PIN);
#define L_FR_FORWARD				GPIO_SetBits(L_FR_PORT,L_FR_PIN)
#define L_FR_BACK  					GPIO_ResetBits(L_FR_PORT,L_FR_PIN)
#define L_FR_STATUS         GPIO_ReadOutputDataBit(L_FR_PORT,L_FR_PIN);
#define L_BK_ENABLE					GPIO_ResetBits(L_BK_PORT,L_BK_PIN)
#define L_BK_DISABLE				GPIO_SetBits(L_BK_PORT,L_BK_PIN)
#define L_BK_STATUS         GPIO_ReadOutputDataBit(L_BK_PORT,L_BK_PIN);
//�ҵ������
#define R_EN_ENABLE       	GPIO_ResetBits(R_EN_PORT,R_EN_PIN)
#define R_EN_DISABLE      	GPIO_SetBits(R_EN_PORT,R_EN_PIN)
#define R_EN_STATUS         GPIO_ReadOutputDataBit(R_EN_PORT,R_EN_PIN);
#define R_FR_FORWARD				GPIO_ResetBits(R_FR_PORT,R_FR_PIN)
#define R_FR_BACK  					GPIO_SetBits(R_FR_PORT,R_FR_PIN)
#define R_FR_STATUS         GPIO_ReadOutputDataBit(R_FR_PORT,R_FR_PIN);
#define R_BK_ENABLE					GPIO_ResetBits(R_BK_PORT,R_BK_PIN)
#define R_BK_DISABLE				GPIO_SetBits(R_BK_PORT,R_BK_PIN)
#define R_BK_STATUS         GPIO_ReadOutputDataBit(R_BK_PORT,R_BK_PIN);
//�����������
#define LIFTR_FOR_ENABLE     GPIO_SetBits(LT_FR_PORT, LT_FR_PIN)
#define LIFTR_BACK_ENABLE    GPIO_ResetBits(LT_FR_PORT, LT_FR_PIN)
#define LIFTR_EN_ENABLE      GPIO_SetBits(LT_EN_PORT, LT_EN_PIN)
#define LIFTR_EN_DISABLE     GPIO_ResetBits(LT_EN_PORT, LT_EN_PIN)
#define LIFTR_BREAK_ENABLE   GPIO_SetBits(LT_BK_PORT, LT_BK_PIN)
#define LIFTR_BREAK_DISABLE  GPIO_ResetBits(LT_BK_PORT, LT_BK_PIN)

typedef struct
{
    uint8_t 				ValidCount;
    int8_t 					MinPoint;
    int8_t 					NearPoint;
    int8_t 					MaxPoint;

} CHECK_STRUCT;

typedef struct
{
    //uint8_t 				BaseSpeed;
    uint8_t 				CurrSetSpeed;  //��ǰ���õ��ٶ�
    float						SpeedBPWM;							//�ٶ���PWMռ�ձȵı���
    uint8_t 				SetBS;   							// �趨��׼��
    //int16_t 				Proportion; 						// ��������k(p)
    //int16_t 				Derivative; 						// ΢�ֳ���k(d)
    int16_t 				PrevDeviation; 						// ΢�ֲ��ֵ� ����ƫ��e(k-1)  Error[-2]
} MOTOR_PARAMETER;

typedef struct
{
	uint8_t EN;
	uint8_t FB;
	uint8_t BK;
	uint8_t PWM;
	uint32_t PG;
}_MOTO_Status;


uint32_t Get_Moto_Speed(void);
void Set_Moto_Speed(int8_t L_Duty,int8_t R_Duty);
void InitLRMotoCtrl(void);
void StopMoto(void);
void BrakeMoto(void);
void SetMotoForward(void);
void SetMotoBack(void);
void LifterUp(void);
void LifterDown(void);
void LifterStop(void);

#endif
