

#ifndef SPI2_H
#define SPI2_H

#include "stm32f4xx.h"

#define W25X16               SPI2
#define W25X16_CLK           RCC_APB1Periph_SPI2


#define W25X16_CS_PIN    GPIO_Pin_12
#define W25X16_SCK_PIN   GPIO_Pin_13
#define W25X16_MISO_PIN  GPIO_Pin_14
#define W25X16_MOSI_PIN  GPIO_Pin_15

#define W25X16_CS_PORT   GPIOB
#define W25X16_SCK_PORT  GPIOB
#define W25X16_MISO_PORT GPIOB
#define W25X16_MOSI_PORT GPIOB

#define W25X16_CS_PORT_CLK     RCC_AHB1Periph_GPIOB
#define W25X16_SCK_PORT_CLK    RCC_AHB1Periph_GPIOB
#define W25X16_MISO_PORT_CLK   RCC_AHB1Periph_GPIOB
#define W25X16_MOSI_PORT_CLK   RCC_AHB1Periph_GPIOB

#define W25X16_CS_SOURCE       GPIO_PinSource12
#define W25X16_SCK_SOURCE      GPIO_PinSource13
#define W25X16_MISO_SOURCE     GPIO_PinSource14
#define W25X16_MOSI_SOURCE     GPIO_PinSource15
 




#define W25X16_CS_High      GPIO_SetBits(W25X16_CS_PORT, W25X16_CS_PIN)
#define W25X16_CS_Low       GPIO_ResetBits(W25X16_CS_PORT, W25X16_CS_PIN)

void SPI2_Init(void);
u8 W25X16_SendByte(u8 byte);


#endif




