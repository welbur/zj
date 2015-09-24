
#ifndef __FS_H
#define __FS_H

#include "API_Include.h"

#define SECTORSIZE  0x1000

#define CAR_CFG_OFFSET  (0 * SECTORSIZE)    /*小车配置数据*/

#define ROUTE_CFG_OFFSET  (1 * SECTORSIZE)    /*路径配置数据*/

#define UPGRADE_CFG_OFFSET  (63  * SECTORSIZE)         /*升级标志信息*/
 
#define UPGRADE_OFFSET   (64  * SECTORSIZE)            /*升级数据缓存处*/


#define CAR_CHECK_STR   "CAR_CFG_INFO"

typedef struct
{
	CAR_PARAMETER CarPara;
	uint8_t bak[200];
	char CheckStr[30];
}_Car_Save_Info;

extern _Car_Save_Info Car_Save_Info;

#define ROUTE_CHECK_STR   "ROUTE_CFG_INFO"
#define MAX_ROUTE_SITE_NUM    100  //最大可保存的路径站点信息

//路径保存信息
typedef struct
{
	ROUTE_SERIAL_INFO Arr[MAX_ROUTE_SITE_NUM];//路径站点个数
	uint16_t TotalNum;
	uint8_t Bak[1024];
	char CheckStr[30];
}_Route_Save_Info;
extern _Route_Save_Info Route_Save_Info;

typedef struct 
{
 uint16_t offset;
 uint8_t  update_state; //1，正在升级，0，正常状况
 uint8_t  finish_flag;  //1.升级完成 0.正常状况
 uint32_t  Write_len;    //写入SPI FLASH长度
 uint32_t recved_len;   //已接收长度
 uint32_t total_len;	   //总长度
 uint32_t Base_Addr;    //升级区域首地址
 uint8_t  Fail_Flag;  //升级失败标志
}_UPDATE_FLAG; 


void FS_Init(void);

#endif
