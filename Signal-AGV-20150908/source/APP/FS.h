
#ifndef __FS_H
#define __FS_H

#include "API_Include.h"

#define SECTORSIZE  0x1000

#define CAR_CFG_OFFSET  (0 * SECTORSIZE)    /*С����������*/

#define ROUTE_CFG_OFFSET  (1 * SECTORSIZE)    /*·����������*/

#define UPGRADE_CFG_OFFSET  (63  * SECTORSIZE)         /*������־��Ϣ*/
 
#define UPGRADE_OFFSET   (64  * SECTORSIZE)            /*�������ݻ��洦*/


#define CAR_CHECK_STR   "CAR_CFG_INFO"

typedef struct
{
	CAR_PARAMETER CarPara;
	uint8_t bak[200];
	char CheckStr[30];
}_Car_Save_Info;

extern _Car_Save_Info Car_Save_Info;

#define ROUTE_CHECK_STR   "ROUTE_CFG_INFO"
#define MAX_ROUTE_SITE_NUM    100  //���ɱ����·��վ����Ϣ

//·��������Ϣ
typedef struct
{
	ROUTE_SERIAL_INFO Arr[MAX_ROUTE_SITE_NUM];//·��վ�����
	uint16_t TotalNum;
	uint8_t Bak[1024];
	char CheckStr[30];
}_Route_Save_Info;
extern _Route_Save_Info Route_Save_Info;

typedef struct 
{
 uint16_t offset;
 uint8_t  update_state; //1������������0������״��
 uint8_t  finish_flag;  //1.������� 0.����״��
 uint32_t  Write_len;    //д��SPI FLASH����
 uint32_t recved_len;   //�ѽ��ճ���
 uint32_t total_len;	   //�ܳ���
 uint32_t Base_Addr;    //���������׵�ַ
 uint8_t  Fail_Flag;  //����ʧ�ܱ�־
}_UPDATE_FLAG; 


void FS_Init(void);

#endif
