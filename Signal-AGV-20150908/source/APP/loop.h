

#ifndef __LOOP_H
#define __LOOP_H

#include "API_Include.h"
#include "Driver_Include.h"



typedef enum
{
	WAIT_MODE,     //�ȴ��ⲿ����
	CHANGE_MODE,    //�л�ģʽ
	TEST_MODE,     //����ģʽ
	MANUAL_MODE,   //�ֶ�ģʽ
	AUTO_MODE,     //�Զ�ģʽ
	ABORT_MODE,    //�쳣ģʽ
	UPDATE_MODE,   //����ģʽ
	
	
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


