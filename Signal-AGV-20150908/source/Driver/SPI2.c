
#include "Global_Config.h"
#include "SPI2.h"


/*******************************************************************************
* Function Name  : SPI2_Init
* Description    : 初始化SPI2管脚.用于连接MX25L400 SPI FLASH
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  RCC_AHB1PeriphClockCmd(W25X16_CS_PORT_CLK |W25X16_SCK_PORT_CLK | W25X16_MISO_PORT_CLK | W25X16_MOSI_PORT_CLK,  ENABLE);
  
  /*!< SPI2 Periph clock enable */
	if(W25X16 == SPI1 || W25X16 == SPI4 || W25X16 == SPI5 || W25X16 == SPI6 )
		RCC_APB2PeriphClockCmd(W25X16_CLK, ENABLE);
  else
		RCC_APB1PeriphClockCmd(W25X16_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = W25X16_CS_PIN;
  GPIO_Init(W25X16_CS_PORT, &GPIO_InitStructure);
  

  //GPIO_PinAFConfig(W25X16_CS_PORT,   W25X16_CS_SOURCE, GPIO_AF_SPI2);
  GPIO_PinAFConfig(W25X16_SCK_PORT,  W25X16_SCK_SOURCE, GPIO_AF_SPI2);
  GPIO_PinAFConfig(W25X16_MISO_PORT, W25X16_MISO_SOURCE, GPIO_AF_SPI2);
  GPIO_PinAFConfig(W25X16_MOSI_PORT, W25X16_MOSI_SOURCE, GPIO_AF_SPI2);
      
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = W25X16_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(W25X16_SCK_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = W25X16_MOSI_PIN;
  GPIO_Init(W25X16_MOSI_PORT, &GPIO_InitStructure);
	
//	  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = W25X16_MISO_PIN;
  GPIO_Init(W25X16_MISO_PORT, &GPIO_InitStructure);

  /* Deselect the SPI1: Chip Select high */
  W25X16_CS_High;

  /* SPI1 configuration */
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(W25X16, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(W25X16, ENABLE);
	
}

/*******************************************************************************
* Function Name  : W25X16_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 W25X16_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(W25X16, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(W25X16, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(W25X16, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(W25X16);
}


