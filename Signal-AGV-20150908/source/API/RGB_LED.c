#include "RGB_LED.h"

#define RGB_TIME 20000

static int8_t Led_Default_Flash_Timer_Index = -1;
uint32_t rgbTime = 0;

/****************************************************************************
* 名    称：Led_Default_Flash_Timeout
* 功    能：默认开机RGB灯闪烁三次
* 参    数：
* 反    回：无
* 说    明：
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
* 名    称：RGB_LED_Init
* 功    能：初始化RGB LED灯,默认闪烁3秒后全部关闭
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void RGB_LED_Init(void)
{
	LED_R_DISABLE;
	LED_G_DISABLE;
	LED_B_DISABLE;
}
/****************************************************************************
* 名    称：void SetLedColor(uint8_t Color)
* 功    能：设置三色灯颜色
* 参    数：color 要设置的颜色。
* 反    回：无
* 说    明：SetLedColor(LED_COLOR_RED|LED_COLOR_BLUE) 红灯和蓝灯就会同时亮
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

