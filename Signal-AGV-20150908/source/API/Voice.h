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
	SPEAK_WELCOME               =((uint8_t)0x01),	//欢迎使用
	SPEAK_MUSIC                 =((uint8_t)0x02),	//运行音乐
	SPEAK_AGV_ARRIV             =((uint8_t)0x03),	//AGV到站  
	SPEAK_BATTERY_LOW           =((uint8_t)0x04),	//电量不足
	SPEAK_ROUTE_BLOCK           =((uint8_t)0x05),	//前方障碍
	SPEAK_OUT_ROUTE             =((uint8_t)0x06),	//小车脱离轨道
	SPEAK_URGENT_STOP           =((uint8_t)0x07),	//紧急停止
	SPEAK_FAULT                 =((uint8_t)0x08),	//小车故障
	SPEAK_BE_CAREFUL            =((uint8_t)0x09),	//前防注意安全
	SPEAK_MANUAL_MSG			=((uint8_t)0x0A),	//手动模式提示音
	SPEAK_MAGNETIC_FAULT		=((uint8_t)0x0B),	//导航器故障
	SPEAK_RADIO_FAULT			=((uint8_t)0x0C),	//无线模块故障
	SPEAK_DRIVER_FAULT			=((uint8_t)0x0D),	//驱动模块故障
	SPEAK_MUTE					=((uint8_t)0x0F),	//无声
}SPEAK;
 



void SetSpeakMute(void);
void SetSpeakSwitch(SPEAK_SWITCH ss);
void VoiceInit(void);
void Speak(SPEAK conn,uint8_t enInsert); 

#endif
