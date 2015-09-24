
#include "API_Include.h"
#include "FS.h"

#define SPI_FLASH_DEBUG 1

_Car_Save_Info Car_Save_Info;
_Route_Save_Info Route_Save_Info;

/****************************************************************************
* 名    称：FS_Init
* 功    能：初始化系统配置,小车配置等信息
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
void FS_Init(void)
{
	#if SPI_FLASH_DEBUG
	u16 ChipID = 0;
	#endif
	
	SPI_Flash_Init();
	
  #if SPI_FLASH_DEBUG
	ChipID = SPI_Flash_ReadID();
	if((ChipID & FLASH_ID) == FLASH_ID)
		printf("Read SPI Flash Chip ID Success\r\n");
	else
		printf("Read SPI Flash Chip ID Error\r\n");
	#endif
	
	SPI_Flash_Read( CAR_CFG_OFFSET, (u8*)&Car_Save_Info, sizeof(Car_Save_Info));
	if( strcmp( Car_Save_Info.CheckStr , CAR_CHECK_STR)  !=  NULL )
	{
		#if SPI_FLASH_DEBUG
		printf("Read CarPara Error,Write to Default Info\r\n");
		#endif
		memset( (char*)&Car_Save_Info, 0x00, sizeof(Car_Save_Info));
		sprintf(Car_Save_Info.CheckStr,"%s",CAR_CHECK_STR);

		Car_Save_Info.CarPara.RunMode = CAR_MODE_MANUAL;
		Car_Save_Info.CarPara.SetSpeed = 15;
		
		SPI_Flash_Write( CAR_CFG_OFFSET, (unsigned char *)&Car_Save_Info, sizeof(Car_Save_Info));
	}
	#if SPI_FLASH_DEBUG
	printf("Read CarPara Success\r\n");
	#endif
	
	SPI_Flash_Read( ROUTE_CFG_OFFSET, (u8*)&Route_Save_Info, sizeof(Route_Save_Info));
	if( strcmp( Route_Save_Info.CheckStr , ROUTE_CHECK_STR)  !=  NULL )
	{
		#if SPI_FLASH_DEBUG
		printf("Read Route Error,Write to Default Info\r\n");
		#endif
		memset( (char*)&Route_Save_Info, 0x00, sizeof(Route_Save_Info));
		sprintf(Route_Save_Info.CheckStr,"%s",ROUTE_CHECK_STR);
		
		SPI_Flash_Write( ROUTE_CFG_OFFSET, (unsigned char *)&Route_Save_Info, sizeof(Route_Save_Info));
	}
	#if SPI_FLASH_DEBUG
	printf("Read Route Success\r\n");
	#endif
	

}


