
#ifndef __CAR_H
#define __CAR_H

#include "API_Include.h"
#include "Global_Struct.h"

extern CAR_ATTRIBUTE *CarData;
extern CAR_ATTRIBUTE LastCarData;
extern _Change_Flag Change_Flag;

extern MOTOR_PARAMETER mp;

extern uint16_t MagneticValue;				  //磁导航器检测到的数据
extern uint8_t f_MagneticValueUseCount;
extern uint16_t MagneticRecvCount,LastMagneticRecvCount; 

#define MAXTRUNCOUNT  20  //控制转向分成多次转向
void MagnetValidDataProcess(char *buf,uint16_t len);
void InsideValidDataProcess(char *buf,uint16_t len);
void OutsideValidDataProcess(uint8_t *buf,uint16_t len);
void RouteFindProcess(RFID *rf);
void RecvDriverReadRFIDProcess(uint8_t *data);
void RefreshLastCarPara(void);
void Para_Init(void);
void BreakCar(void);
void StopCar(void);
void CheckConfigChange(void);
void Moto_Action_Process(CAR_ACTION MotoCmd);
void ChangeCarStatus(CAR_STATUS cs);
void Run(int16_t runValue);
void InitCheckRoute(void);
int16_t CalcPD(int8_t cvData);
int8_t CalcCheckValue(void);
int16_t CheckRoute(uint16_t CheckValue);

#endif

