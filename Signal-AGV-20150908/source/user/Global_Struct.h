
#ifndef __GLOBAL_STRUCT
#define __GLOBAL_STRUCT

#include "Global_Config.h"

#define LIFTER_UP     1000    //升降杆
#define LIFTER_DOWN   1001

#define TURN_RIGHT    2000    //转向
#define TURN_LIFT     2001

#define VOICE_ON      3000    //声音
#define VOICE_OFF     3001

#define SPEED_UP      4000    //速度
#define SPEED_DOWN    4001  

#define SITE_ARR      5000    //到达站点
#define SIGNAL        6000    //信道切换

#define CAR_BATTERY_LOW_VALUE					220


//AGV状态
typedef enum
{
	CAR_STATUS_NORMAL							=0x00,	//正常
	CAR_STATUS_STOP								=0x01,	//停止
	CAR_STATUS_FAULT							=0x02,	//故障
	CAR_STATUS_ROUTE_FBLOCK				=0x03,	//远障碍
	CAR_STATUS_OUT_ROUTE					=0x04,	//出轨
	CAR_STATUS_MANUAL							=0x06,	//手动
	CAR_STATUS_URGENT_STOP				=0x07,	//急停
	CAR_STATUS_ROUTE_BLOCK				=0x08,	//障碍
	CAR_STATUS_BATTERY_LOW				=0x09,  //电量不足
	CAR_STATUS_UP									=0x0A		//开机
	
}CAR_STATUS;

//AGV工作模式
typedef enum
{	
	CAR_MODE_AUTO								  =0x01,	//自动模式(接受上位机控制,按磁条寻走)
	CAR_MODE_MANUAL								=0x02,	//手动模式(只接受本机及遥控控制,不按磁条寻走)
}CAR_MODE;


//AGV动作指令
typedef enum
{
	CAR_ACTION_STOP										=	0x03,	//停止
	CAR_ACTION_FORWARD      					=	0x00,	//前进
	CAR_ACTION_BACK         					=	0x04,	//后退
	CAR_ACTION_TURN_LEFT    					=	0x01,	//左转
	CAR_ACTION_TURN_RIGHT   					=	0x02,	//右转
	CAR_ACTION_URGENT_STOP            =	0x05,  //紧急停车
	CAR_ACTION_BREAK                  = 0x06, //刹车
}CAR_ACTION;



//AGV出错误信息定义
typedef enum
{
	CAR_ERROR_NORMAL							=0x01,	//正常
	CAR_ERROR_EEPROM							=0x02,	//配置文件操作出错
	CAR_ERROR_MAGNETIC_CONN				=0x04,	//磁导航通信出错
	CAR_ERROR_DRIVER_CONN					=0x06,	//驱动模块通信出错
	CAR_ERROR_COMMUNICATION				=0x07,	//网络连接出错
}CAR_ERROR;


typedef enum
{
	CARD_TYPE_TURN								=0x54,	//转向卡
	CARD_TYPE_SITE								=0x53,	//站点卡
	CARD_TYPE_MUTE								=0x52,	//语音卡
	CARD_TYPE_SIGNAL							=0x45,	//信道切换卡
	CARD_TYPE_SPEED								=0x56,	//速度卡
	CARD_TYPE_LIFT								=0x55,	//升降卡
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

//AGV参数定义
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


//显示屏数据结构
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


//驱动初始化写入参数
typedef  struct
{
    uint8_t      	Speed;
    uint8_t      	PValue;
    uint8_t      	DValue;
    uint8_t      	Mode;
    
}DRIVER_INIT_PARAMTER;


//驱动当前行驶状态
typedef struct
{
	uint8_t			Mode;	
    uint8_t     	Speed;
    uint16_t     	Len;
    CAR_STATUS     	Status;
	
}DRIVER_RUN_STATUS;

//任何环境改变标志
typedef union
{
	struct
	{
		uint64_t AGVID:1;          //AGV编号  
		uint64_t SetSpeed:1;       //速度设定
		uint64_t RunMode:1;        //AGV工作模式
		uint64_t StopTime:1;       //目的站点停靠时间
		uint64_t PValue:1;         //P值
		uint64_t DValue:1;         //D值
		uint64_t VoltageAdjust:1;  //电压校准值
		uint64_t ManualCmd:1;      //手动命令
		uint64_t NowSpeed:1;         //当前速度
		uint64_t Voltage:1;         //当前电量
		uint64_t UseLimit:1;       	//用户限定
		uint64_t NowStatus:1;       //现在状态
		uint64_t LastStation:1;     //上一个站点卡号
		uint64_t Distance:1;        //离开上一站点的距离
		uint64_t NextStation:1;     //下一个站点卡号
		uint64_t Destination:1;     //目的站点卡号
	}BITS;
	uint64_t ALL_Env;
}_Environment_Event;
//任何外部事件触发
typedef union
{
	struct
	{
		uint64_t LRS_Far:1;       //左右传感器远
		uint64_t LRS_Near:1;      //左右传感器近
		uint64_t IR_Far:1;        //红外传感器远
		uint64_t IR_Near:1;       //红外传感器近
		uint64_t STOP:1;          //紧急按钮
		uint64_t LT_UP:1;         //升降传感器上
		uint64_t LT_DOWN:1;       //升降传感器下
		uint64_t KEY1:1;          //按键1
		uint64_t KEY2:1;          //按键2
		uint64_t KEY3:1;          //按键3
		uint64_t KEY4:1;          //按键4
		
		
		uint64_t MagnetComm:1;     //与磁传感通信正常
		uint64_t BatteryLow:1;     //备用电池低压
		uint64_t CarChangeNormal:1; //小车从异常状态恢复到正常行走
	}BITS;
	uint64_t ALL_EXT;
}_External_Event;

typedef struct
{
	_Environment_Event Env;
	_External_Event    Ext;
	
}_Change_Flag;


#endif

