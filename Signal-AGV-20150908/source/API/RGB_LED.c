#include "RGB_LED.h"

#define RGB_TIME 20000

static int8_t Led_Default_Flash_Timer_Index = -1;
uint32_t rgbTime = 0;

/****************************************************************************
* ��    �ƣ�Led_Default_Flash_Timeout
* ��    �ܣ�Ĭ�Ͽ���RGB����˸����
* ��    ����
* ��    �أ���
* ˵    ����
****************************************************************************/
int8_t Led_Default_Flash_Timeout(void *cbptr)
{
	static uint8_t Led_Flash_Count = 0;
	Led_Flash_Count++;
  
	if(Led_Flash_Count > 3)
  {
    Led_Flash_Count = 0;
    Free_Timer(&Led_Default_Flash_Timer_Index);
    LED_R_DISABLE;
    LED_G_DISABLE;
    LED_B_DISABLE;
  }
  else
  {
    GPIO_ToggleBits(LED_R_PORT,LED_R_PIN);
    GPIO_ToggleBits(LED_G_PORT,LED_G_PIN);
    GPIO_ToggleBits(LED_B_PORT,LED_B_PIN);
  }
		
	return 0;	
}

/****************************************************************************
* ��    �ƣ�RGB_LED_Init
* ��    �ܣ���ʼ��RGB LED��,Ĭ����˸3���ȫ���ر�
* ��    ����
* ��    �أ���
* ˵    ����
****************************************************************************/
void RGB_LED_Init(void)
{
	LED_R_DISABLE;
	LED_G_DISABLE;
	LED_B_DISABLE;
}
/****************************************************************************
* ��    �ƣ�void SetLedColor(uint8_t Color)
* ��    �ܣ�������ɫ����ɫ
* ��    ����color Ҫ���õ���ɫ��
* ��    �أ���
* ˵    ����SetLedColor(LED_COLOR_RED|LED_COLOR_BLUE) ��ƺ����ƾͻ�ͬʱ��
****************************************************************************/ 
void SetLedColor(uint8_t  Color)
{
  if((0 < rgbTime++) && (rgbTime <  RGB_TIME))
  {
    switch(Color)
    {
      case RED:
        LED_R_ENABLE;
        LED_G_DISABLE;
        LED_B_DISABLE;
        break;
        
      case GRN:
        LED_G_ENABLE;
        LED_R_DISABLE;
        LED_B_DISABLE;
        break;
        
      case BLU:
        LED_B_ENABLE;
        LED_R_DISABLE;
        LED_G_DISABLE;
        break;
        
      case RED_GRN:
        LED_R_ENABLE;
        LED_G_ENABLE;
        LED_B_DISABLE;
        break;
        
      case RED_BLU:
        LED_R_ENABLE;
        LED_B_ENABLE;
        LED_G_DISABLE;
        break;
        
      case GRN_BLU:
        LED_G_ENABLE;
        LED_B_ENABLE;
        LED_R_DISABLE;
        break;
        
      case RED_GRN_BLU:
        LED_R_ENABLE;
        LED_G_ENABLE;
        LED_B_ENABLE;
        break;
        
      default:
        LED_R_DISABLE;
        LED_B_DISABLE;
        LED_G_DISABLE;
        break;
    }
  }
	else if((RGB_TIME < rgbTime) && (rgbTime <  RGB_TIME * 2))
	{
		LED_R_DISABLE;
		LED_B_DISABLE;
		LED_G_DISABLE;
	}
  else if(rgbTime >  RGB_TIME * 2)
  {
    rgbTime = 0;
  }
}

