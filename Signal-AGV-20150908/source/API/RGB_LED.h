
#ifndef __RGB_LED_H
#define __RGB_LED_H

#include "Driver_Include.h"


#define LED_COLOR_RED  		0x01
#define LED_COLOR_GREEN  	0x02
#define LED_COLOR_BLUE  	0x04
#define RED 1          //∫Ï
#define GRN 2          //¬Ã
#define BLU 3          //¿∂
#define RED_GRN 4      //∫Ï¬Ã
#define RED_BLU 5      //∫Ï¿∂
#define GRN_BLU 6      //¬Ã¿∂
#define RED_GRN_BLU 7  //∫Ï¬Ã¿∂

#define LED_R_ENABLE     GPIO_SetBits(LED_R_PORT,LED_R_PIN)
#define LED_R_DISABLE    GPIO_ResetBits(LED_R_PORT,LED_R_PIN)
#define LED_G_ENABLE     GPIO_SetBits(LED_G_PORT,LED_G_PIN)
#define LED_G_DISABLE    GPIO_ResetBits(LED_G_PORT,LED_G_PIN)
#define LED_B_ENABLE     GPIO_SetBits(LED_B_PORT,LED_B_PIN)
#define LED_B_DISABLE    GPIO_ResetBits(LED_B_PORT,LED_B_PIN)


void RGB_LED_Init(void);
void SetLedColor(uint8_t  Color);



#endif


