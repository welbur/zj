
#ifndef __GLOBAL_STRUCT
#define __GLOBAL_STRUCT

#include "Global_Config.h"

#define LIFTER_UP     1000    //������
#define LIFTER_DOWN   1001

#define TURN_RIGHT    2000    //ת��
#define TURN_LIFT     2001

#define VOICE_ON      3000    //����
#define VOICE_OFF     3001

#define SPEED_UP      4000    //�ٶ�
#define SPEED_DOWN    4001  

#define SITE_ARR      5000    //����վ��
#define SIGNAL        6000    //�ŵ��л�

#define CAR_BATTERY_LOW_VALUE					220


//AGV״̬
typedef enum
{
	CAR_STATUS_NORMAL							=0x00,	//����
	CAR_STATUS_STOP								=0x01,	//ֹͣ
	CAR_STATUS_FAULT							=0x02,	//����
	CAR_STATUS_ROUTE_FBLOCK				=0x03,	//Զ�ϰ�
	CAR_STATUS_OUT_ROUTE					=0x04,	//����
	CAR_STATUS_MANUAL							=0x06,	//�ֶ�
	CAR_STATUS_URGENT_STOP				=0x07,	//��ͣ
	CAR_STATUS_ROUTE_BLOCK				=0x08,	//�ϰ�
	CAR_STATUS_BATTERY_LOW				=0x09,  //��������
	CAR_STATUS_UP									=0x0A		//����
	
}CAR_STATUS;

//AGV����ģʽ
typedef enum
{	
	CAR_MODE_AUTO								  =0x01,	//�Զ�ģʽ(������λ������,������Ѱ��)
	CAR_MODE_MANUAL								=0x02,	//�ֶ�ģʽ(ֻ���ܱ�����ң�ؿ���,��������Ѱ��)
}CAR_MODE;


//AGV����ָ��
typedef enum
{
	CAR_ACTION_STOP										=	0x03,	//ֹͣ
	CAR_ACTION_FORWARD      					=	0x00,	//ǰ��
	CAR_ACTION_BACK         					=	0x04,	//����
	CAR_ACTION_TURN_LEFT    					=	0x01,	//��ת
	CAR_ACTION_TURN_RIGHT   					=	0x02,	//��ת
	CAR_ACTION_URGENT_STOP            =	0x05,  //����ͣ��
	CAR_ACTION_BREAK                  = 0x06, //ɲ��
}CAR_ACTION;



//AGV��������Ϣ����
typedef enum
{
	CAR_ERROR_NORMAL							=0x01,	//����
	CAR_ERROR_EEPROM							=0x02,	//�����ļ���������
	CAR_ERROR_MAGNETIC_CONN				=0x04,	//�ŵ���ͨ�ų���
	CAR_ERROR_DRIVER_CONN					=0x06,	//����ģ��ͨ�ų���
	CAR_ERROR_COMMUNICATION				=0x07,	//�������ӳ���
}CAR_ERROR;


typedef enum
{
	CARD_TYPE_TURN								=0x54,	//ת��
	CARD_TYPE_SITE								=0x53,	//վ�㿨
	CARD_TYPE_MUTE								=0x52,	//������
	CARD_TYPE_SIGNAL							=0x45,	//�ŵ��л���
	CARD_TYPE_SPEED								=0x56,	//�ٶȿ�
	CARD_TYPE_LIFT								=0x55,	//������
}CARD_TYPE;



typedef enum 
{
	MODLUE_DRIVER,MODLUE_RADIO,MODLUE_NORMAL
}MODLUE;

typedef struct
{
	CAR_ERROR 		cError;
	MODLUE 			mModlue;	
}ERROR_MSG;

//AGV��������
typedef struct
{
  uint16_t  AGVID;
  uint16_t  SetSpeed;
  uint16_t  RunMode;
  uint16_t  StopTime;
  uint16_t  PValue;
  uint16_t  DValue;
  uint16_t	VoltageAdjust;
  uint16_t  UseLimit;
  
}CAR_PARAMETER;


//��ʾ�����ݽṹ
typedef struct
{
	uint16_t 	AGVID;
	uint16_t	SetSpeed;
	uint16_t	RunMode;
	uint16_t	StopTime;
	uint16_t	PValue;
	uint16_t	DValue;//6
	uint16_t	VoltageAdjust;
	uint16_t	ManualCmd;	
	uint16_t	NowSpeed;
	uint16_t 	Voltage;
	uint16_t	UseLimit;//11
	uint16_t	NowStatus;
	uint16_t	LastStation;
	uint16_t	Distance;
	uint16_t	NextStation;
	uint16_t	Destination;
	
}CAR_ATTRIBUTE;

typedef struct
{
	uint8_t			CardType;
	uint8_t			CardIDMSB;
	uint8_t			CardIDLSB;
}RFID;

typedef struct
{
	uint8_t		CardType;
	uint8_t		CardIDMSB;
	uint8_t		CardIDLSB;
	uint8_t		Action;
}ROUTE_SERIAL_INFO;


//������ʼ��д�����
typedef  struct
{
    uint8_t      	Speed;
    uint8_t      	PValue;
    uint8_t      	DValue;
    uint8_t      	Mode;
    
}DRIVER_INIT_PARAMTER;


//������ǰ��ʻ״̬
typedef struct
{
	uint8_t			Mode;	
    uint8_t     	Speed;
    uint16_t     	Len;
    CAR_STATUS     	Status;
	
}DRIVER_RUN_STATUS;

//�κλ����ı��־
typedef union
{
	struct
	{
		uint64_t AGVID:1;          //AGV���  
		uint64_t SetSpeed:1;       //�ٶ��趨
		uint64_t RunMode:1;        //AGV����ģʽ
		uint64_t StopTime:1;       //Ŀ��վ��ͣ��ʱ��
		uint64_t PValue:1;         //Pֵ
		uint64_t DValue:1;         //Dֵ
		uint64_t VoltageAdjust:1;  //��ѹУ׼ֵ
		uint64_t ManualCmd:1;      //�ֶ�����
		uint64_t NowSpeed:1;         //��ǰ�ٶ�
		uint64_t Voltage:1;         //��ǰ����
		uint64_t UseLimit:1;       	//�û��޶�
		uint64_t NowStatus:1;       //����״̬
		uint64_t LastStation:1;     //��һ��վ�㿨��
		uint64_t Distance:1;        //�뿪��һվ��ľ���
		uint64_t NextStation:1;     //��һ��վ�㿨��
		uint64_t Destination:1;     //Ŀ��վ�㿨��
	}BITS;
	uint64_t ALL_Env;
}_Environment_Event;
//�κ��ⲿ�¼�����
typedef union
{
	struct
	{
		uint64_t LRS_Far:1;       //���Ҵ�����Զ
		uint64_t LRS_Near:1;      //���Ҵ�������
		uint64_t IR_Far:1;        //���⴫����Զ
		uint64_t IR_Near:1;       //���⴫������
		uint64_t STOP:1;          //������ť
		uint64_t LT_UP:1;         //������������
		uint64_t LT_DOWN:1;       //������������
		uint64_t KEY1:1;          //����1
		uint64_t KEY2:1;          //����2
		uint64_t KEY3:1;          //����3
		uint64_t KEY4:1;          //����4
		
		
		uint64_t MagnetComm:1;     //��Ŵ���ͨ������
		uint64_t BatteryLow:1;     //���õ�ص�ѹ
		uint64_t CarChangeNormal:1; //С�����쳣״̬�ָ�����������
	}BITS;
	uint64_t ALL_EXT;
}_External_Event;

typedef struct
{
	_Environment_Event Env;
	_External_Event    Ext;
	
}_Change_Flag;


#endif

