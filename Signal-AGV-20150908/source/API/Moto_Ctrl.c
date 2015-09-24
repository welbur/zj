
#include "Moto_Ctrl.h"


_MOTO_Status L_Moto_Status,R_Moto_Status;//��ǰ�����״̬

int8_t Click_Break_Timer_Index = 1;//��ɲ��ʱ��
uint8_t Click_Break_Count = 0;

/****************************************************************************
* ��    �ƣ�void Get_Moto_Speed(void)
* ��    �ܣ���ȡ����ת��
* ��    ������
* ��    �أ���
* ˵    ����ȡ���ҵ��ת�ٵ�ƽ��ֵ
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
* ��    �ƣ�void Set_Moto_Speed(void)
* ��    �ܣ���������ת��
* ��    �������ҵ��ռ�ձ�,10��ʾ10%��ռ�ձ�
* ��    �أ���
* ˵    �����̶�Ƶ��,�ı�ռ�ձ����ı�����ת��
*��ռ�ձ�С��5%,ֱ������͵�ƽ,��ռ�ձȴ���99,ֱ������ߵ�ƽ
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
* ��    �ƣ�InitLRMotoCtrl
* ��    �ܣ���ʼ��������
* ��    ������
* ��    �أ���
* ˵    �����ϵ�ʹ֮����ֹͣ״̬
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
* ��    �ƣ�void StopMoto(void)
* ��    �ܣ�ֹͣ���
* ��    ���� 
* ��    �أ���
* ˵    ����
****************************************************************************/
void StopMoto(void)
{
	Set_Moto_Speed(PWM_STOP_DUTY,PWM_STOP_DUTY);
	L_EN_DISABLE;
	R_EN_DISABLE;
	
}
/****************************************************************************
* ��    �ƣ�Click_Break_Timeout
* ��    �ܣ���ɲ��ʱ��
* ��    ���� 
* ��    �أ���
* ˵    ����
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
* ��    �ƣ�void BrakeMoto(void)
* ��    �ܣ�ɲ��
* ��    ���� 
* ��    �أ���
* ˵    ����
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
* ��    �ƣ�void SetMotoForward(void)
* ��    �ܣ��������Ϊǰ��
* ��    ���� 
* ��    �أ���
* ˵    ����
****************************************************************************/
void SetMotoForward(void)
{
	//���ԭ�����Ϊ��ת,Ӧ����ɲ��,�ȴ�ת��Ϊ0�����л�����ת
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
* ��    �ƣ�void SetMotoBack(void)
* ��    �ܣ��������Ϊ����
* ��    ���� 
* ��    �أ���
* ˵    ����
****************************************************************************/
void SetMotoBack(void)
{
	//���ԭ�����Ϊ��ת,Ӧ����ɲ��,�ȴ�ת��Ϊ0�����л�����ת
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
* ��    �ƣ�void LifterUp(void)
* ��    �ܣ�����������
* ��    ������
* ��    �أ���
* ˵    ������
****************************************************************************/
void LifterUp(void)
{
  LIFTR_EN_ENABLE;
  LIFTR_FOR_ENABLE;
  LIFTR_BREAK_DISABLE;
}

/****************************************************************************
* ��    �ƣ�void LifterDown(void)
* ��    �ܣ�����������
* ��    ������
* ��    �أ���
* ˵    ������
****************************************************************************/
void LifterDown(void)
{
  LIFTR_EN_ENABLE;
  LIFTR_BACK_ENABLE;
  LIFTR_BREAK_DISABLE;
}

/****************************************************************************
* ��    �ƣ�void LifterStop(void)
* ��    �ܣ�������ֹͣ
* ��    ������
* ��    �أ���
* ˵    ������
****************************************************************************/
void LifterStop(void)
{
  LIFTR_EN_DISABLE;
  LIFTR_BREAK_DISABLE;
}




