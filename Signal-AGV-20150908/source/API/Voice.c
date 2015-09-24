
#include "Driver_Include.h"
#include "Voice.h"


SPEAK_SWITCH sw;
INSERT_SELECT EnableInsert = INSERT_ENABLE;

static int8_t Voice_Stop_Timer_Index = -1;

void SetSpeakMute(void)
{
	VOICE_CTR1_HIGH;
	VOICE_CTR2_HIGH;
	VOICE_CTR3_HIGH;
	VOICE_CTR4_HIGH;
}
void SetSpeakSwitch(SPEAK_SWITCH ss)
{
	sw=ss;
	if(sw==SPEAK_SWITCH_OFF)
	{
		SetSpeakMute();
	}
	
}

/****************************************************************************
* ��    �ƣ�void VoiceInit(void)
* ��    �ܣ���ʼ������
* ��    ������
* ��    �أ���
* ˵    ������
****************************************************************************/ 
void VoiceInit(void)
{
    SetSpeakMute();
}
/****************************************************************************
* ��    �ƣ�Voice_Stop_Timeour
* ��    �ܣ��������ֳ�ʱ��Ĵ���
* ��    ������
* ��    �أ���
* ˵    ������
****************************************************************************/ 
int8_t Voice_Stop_Timeour(void)
{
	SetSpeakMute();
}


/****************************************************************************
* ��    �ƣ�void Speak(SPEAK conn)
* ��    �ܣ�����ָ������
* ��    ����conn Ҫ���������ݣ��磺SPEAK_WELCOME, enInsert �Ƿ�������� 0Ϊ������1Ϊ���� 
* ��    �أ���
* ˵    ����Speak(SPEAK_WELCOME)
****************************************************************************/ 
void Speak(SPEAK conn,uint8_t enInsert)
{
	u32 DelayTime;
	if((sw==SPEAK_SWITCH_ON) &&( EnableInsert  == INSERT_ENABLE))
	{
		EnableInsert= (INSERT_SELECT)enInsert;
		if(conn & VOICE_CTR1_OFFSET)
			VOICE_CTR1_HIGH;
		else
			VOICE_CTR1_LOW;
		if(conn & VOICE_CTR2_OFFSET)
			VOICE_CTR2_HIGH;
		else
			VOICE_CTR2_LOW;
		if(conn & VOICE_CTR3_OFFSET)
			VOICE_CTR3_HIGH;
		else
			VOICE_CTR3_LOW;
		if(conn & VOICE_CTR4_OFFSET)
			VOICE_CTR4_HIGH;
		else
			VOICE_CTR4_LOW;

		DelayTime = 0;
		switch(conn)
		{
			case SPEAK_WELCOME:
			{
				DelayTime = 6200;
				break;
			}
			case SPEAK_AGV_ARRIV:
			{
				DelayTime = 4800;
				break;
			}
			default:
			{
				break;
			}
		}
    
		if(DelayTime)//��Ҫ�Զ�ֹͣ���ֲ���
		{
  		if(Voice_Stop_Timer_Index != -1)
  			Free_Timer(&Voice_Stop_Timer_Index);
  		Malloc_Timer(DelayTime,Only_Mode,Voice_Stop_Timeour,NULL,&Voice_Stop_Timer_Index);
		}
	};
}



