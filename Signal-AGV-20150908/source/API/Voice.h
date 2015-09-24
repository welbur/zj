#ifndef __VOICE
#define __VOICE
#include "stm32f4xx.h"

#define VOICE_CTR1_OFFSET 0x01
#define VOICE_CTR2_OFFSET 0x02
#define VOICE_CTR3_OFFSET 0x04
#define VOICE_CTR4_OFFSET 0x08




#define VOICE_CTR1_HIGH   GPIO_SetBits(VOICE_CTR1_PORT,VOICE_CTR1_PIN)
#define VOICE_CTR1_LOW    GPIO_ResetBits(VOICE_CTR1_PORT,VOICE_CTR1_PIN)
#define VOICE_CTR2_HIGH   GPIO_SetBits(VOICE_CTR2_PORT,VOICE_CTR2_PIN)
#define VOICE_CTR2_LOW    GPIO_ResetBits(VOICE_CTR2_PORT,VOICE_CTR2_PIN)
#define VOICE_CTR3_HIGH   GPIO_SetBits(VOICE_CTR3_PORT,VOICE_CTR3_PIN)
#define VOICE_CTR3_LOW    GPIO_ResetBits(VOICE_CTR3_PORT,VOICE_CTR3_PIN)
#define VOICE_CTR4_HIGH   GPIO_SetBits(VOICE_CTR4_PORT,VOICE_CTR4_PIN)
#define VOICE_CTR4_LOW    GPIO_ResetBits(VOICE_CTR4_PORT,VOICE_CTR4_PIN)


typedef enum
{
	SPEAK_SWITCH_ON,
	SPEAK_SWITCH_OFF,
}SPEAK_SWITCH;
typedef enum
{
INSERT_DISABLE,
INSERT_ENABLE	
}INSERT_SELECT;
typedef enum
{
	SPEAK_WELCOME               =((uint8_t)0x01),	//��ӭʹ��
	SPEAK_MUSIC                 =((uint8_t)0x02),	//��������
	SPEAK_AGV_ARRIV             =((uint8_t)0x03),	//AGV��վ  
	SPEAK_BATTERY_LOW           =((uint8_t)0x04),	//��������
	SPEAK_ROUTE_BLOCK           =((uint8_t)0x05),	//ǰ���ϰ�
	SPEAK_OUT_ROUTE             =((uint8_t)0x06),	//С��������
	SPEAK_URGENT_STOP           =((uint8_t)0x07),	//����ֹͣ
	SPEAK_FAULT                 =((uint8_t)0x08),	//С������
	SPEAK_BE_CAREFUL            =((uint8_t)0x09),	//ǰ��ע�ⰲȫ
	SPEAK_MANUAL_MSG			=((uint8_t)0x0A),	//�ֶ�ģʽ��ʾ��
	SPEAK_MAGNETIC_FAULT		=((uint8_t)0x0B),	//����������
	SPEAK_RADIO_FAULT			=((uint8_t)0x0C),	//����ģ�����
	SPEAK_DRIVER_FAULT			=((uint8_t)0x0D),	//����ģ�����
	SPEAK_MUTE					=((uint8_t)0x0F),	//����
}SPEAK;
 



void SetSpeakMute(void);
void SetSpeakSwitch(SPEAK_SWITCH ss);
void VoiceInit(void);
void Speak(SPEAK conn,uint8_t enInsert); 

#endif
