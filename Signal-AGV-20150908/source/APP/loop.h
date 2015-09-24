

#ifndef __LOOP_H
#define __LOOP_H

#include "API_Include.h"
#include "Driver_Include.h"



typedef enum
{
	WAIT_MODE,     //等待外部启动
	CHANGE_MODE,    //切换模式
	TEST_MODE,     //测试模式
	MANUAL_MODE,   //手动模式
	AUTO_MODE,     //自动模式
	ABORT_MODE,    //异常模式
	UPDATE_MODE,   //更新模式
	
	
	MAX_MAIN_MODE
}_MAIN_LOOP;



void Main_Loop_Process(void);
void Manual_Mode_Process(void);
int8_t ChangeSpeedTimeout(void *cbptr);
int8_t CalcLenTimeout(void *cbptr);
int8_t NormalTimeout(void *cbptr);
void Auto_Mode_Process(void);
int HtoD(uint16_t x);






#endif


