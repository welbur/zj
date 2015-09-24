#include "FS.h"
#include "car.h"
#include "loop.h"

#define INFRA_FAR_TIME  0X1000    //红外远时间
#define INFRA_NEAR_TIME 0X1000     //红外近时间
char 	TAB_HEXCHAR[16]	= "0123456789ABCDEF";
char 	TAB_DECCHAR[16]	= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


static int8_t ChangeSpeedTimerIndex = -1;//改变速度持续时间
static int8_t CalcLenTimerIndex = -1;    //计算长度间隔时间
static int8_t NormalTimerIndex = -1;      //正常处理时间
static int8_t ReportCarStatusTimerIndex = -1;//主动上报小车状态

uint8_t OutRouteCount = 0;        //出轨统计次数
uint8_t startFlag = 0;            //启动标志
uint8_t MagnetCommErrorCount = 0; //磁传感通信错误次数
uint8_t  BatteryLowCount = 0;
uint16_t BatteryVal=0;            //铅酸电池电压
uint16_t IR_Distence=0;
uint16_t Distence=0;

//uint16_t* IR_Distence;


uint32_t uiInfraFarTime = 0;      //红外远事件离开时间
uint32_t uiInfraNearTime = 0;     //红外近事件离开时间
/****************************************************************************
* 名    称：ChangeSpeedTimeout
* 功    能：每隔一定时间就查询是否需要改变速度
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
int8_t ReportCarStatusTimeout(void *cbptr)
{
	SendCarStatusToOutSide();
	return 0;
}
/****************************************************************************
* 名    称：AutoStopCarTimeout
* 功    能：每隔一定时间就主动停止小车
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
int8_t AutoStopCarTimeout(void *cbptr)
{
	Change_Flag.Ext.BITS.KEY2 = 1;
	return 0;
}
/****************************************************************************
* 名    称：Main_Loop_Process
* 功    能：主流程处理
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void Main_Loop_Process(void)
{
	uint16_t count1 = 0;
	uint16_t count2 = 0;
#if 0
	BatteryVal = Get_ADC_Val(BAT_VAR_MONITOR);
	if(IR_Distence !=  BatteryVal)
		{
			IR_Distence =  BatteryVal;
			Distence = IR_Distence;
		printf("%d\r\n",Distence);
		}	
#endif

	count1 = TIM3->CNT;
	printf("count1 IS %d\r\n",count1);
	count2 = TIM1->CNT;
	printf("count2 IS %d\r\n",count2);

	Delay(500);
#if 0
  if(Change_Flag.Ext.BITS.KEY1) //启动按钮
  {
    Change_Flag.Ext.BITS.KEY1 = 0;
		startFlag = 1;
    CarData->NowStatus = CAR_STATUS_NORMAL;
    Change_Flag.Env.BITS.NowStatus = 1;
  }

  if(Change_Flag.Ext.BITS.KEY2) //停止按钮,不论在任何模式下,都得响应
  {
    Change_Flag.Ext.BITS.KEY2 = 0;
		startFlag = 0;
    CarData->NowStatus = CAR_STATUS_STOP;
  }

  //AGV ID不等0，设置速度不等于0，运行模式不等于0
  //if((CarData->AGVID != 0) && (CarData->SetSpeed != 0) && (CarData->RunMode & (CAR_MODE_AUTO | CAR_MODE_MANUAL)))
  if((CarData->AGVID != 0) && (CarData->SetSpeed != 0) && startFlag)
	{
		switch(CarData->RunMode)
		{
			case CAR_MODE_AUTO:   //自动模式
				Auto_Mode_Process();
			  break;
      
			case CAR_MODE_MANUAL: //手动模式
				CarData->NowStatus = CAR_STATUS_MANUAL;
				Manual_Mode_Process();
			  break;
		}
	}
  else //停车
  {
    CarData->NowStatus = CAR_STATUS_STOP;
  }
#endif   
}
#if 0
int HtoD(uint16_t x)
{
int sum = 0;
int i;
for(i = 0 ; x!= 0; i++)
{
sum = (x %16) * pow(16,i)+ sum;
x = x/ 16;
}
return sum;
}


void Hex2Byte(uint16_t*str,uint16_t*byte) 
{     
	uint16_t tmp; 
	while(*str!=0) 
		{
			tmp=((*str)<<4)&0xf0; 
			str++; 
			if(*str==0)   
				{
					*byte=(tmp>>4)&0x0f;    
					return;   
				}
			tmp+=(*str)&0x0f;  
			*byte=tmp;    
			byte++;      
		}
} 
#endif


/****************************************************************************
* 名    称：ChangeSpeedTimeout
* 功    能：每隔一定时间就查询是否需要改变速度
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
int8_t ChangeSpeedTimeout(void *cbptr)
{
	if(CarData->ManualCmd != CAR_ACTION_STOP)//速度检测,用于PID算法
	{	
  	if(CarData->NowStatus == CAR_STATUS_NORMAL || CarData->NowStatus == CAR_STATUS_MANUAL)
  	{
  		if(mp.CurrSetSpeed < CarData->SetSpeed)
  		{
  			mp.CurrSetSpeed++;
  		}
  		else if(mp.CurrSetSpeed > CarData->SetSpeed)
  		{
  			mp.CurrSetSpeed = CarData->SetSpeed;
  		}
  	}	
    else if(CarData->NowStatus == CAR_STATUS_ROUTE_FBLOCK) //远障碍时，减速
    {
      if(mp.CurrSetSpeed < 25)
      {
        mp.CurrSetSpeed = 25;
      }
      
      mp.CurrSetSpeed -= 2;
    }
	}
  
	//自动模式需要检测磁传感器是否通信得上.出故障就不需要再检测,防止反复触发通信故障
	if(LastCarData.RunMode == CAR_MODE_AUTO && CarData->NowStatus != CAR_STATUS_FAULT)
	{
  	if(MagneticRecvCount == LastMagneticRecvCount)//磁传感器接收次数不变,磁传感出问题了
  	{
  		MagnetCommErrorCount++;
  		if(MagnetCommErrorCount > 5)
  		{
  			MagnetCommErrorCount = 0;
  			//if(CarData->NowStatus != CAR_STATUS_FAULT)//
  			Change_Flag.Ext.BITS.MagnetComm = 1;
  		}
  	}
  	else
  	{
  		LastMagneticRecvCount = MagneticRecvCount;
  		Change_Flag.Ext.BITS.MagnetComm = 0;
  		MagnetCommErrorCount = 0;
  	}
	}
  
	return 0;
}

	uint32_t mc = 0;
	uint16_t len;
/****************************************************************************
* 名    称：CalcLenTimeout
* 功    能：间隔时间查询当前速度
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
int8_t CalcLenTimeout(void *cbptr)
{

	if(CarData->NowStatus != CAR_STATUS_UP)
	{
		mc = Get_Moto_Speed();
		CarData->NowSpeed = mc / 15;
		len = mc / 200;
		CarData->Distance = len;
	}
	
	BatteryVal = Get_ADC_Val(BAT_VAR_MONITOR);
	BatteryVal = BatteryVal / CarData->VoltageAdjust;
  
	if(BatteryVal < CAR_BATTERY_LOW_VALUE)//底电检测
	{
		BatteryLowCount++;
    
		if(BatteryLowCount > 5)
		{
			BatteryLowCount = 0;
			Change_Flag.Ext.BITS.BatteryLow = 1;
		}
	}
	else
	{
		BatteryLowCount = 0;
		Change_Flag.Ext.BITS.BatteryLow = 0;
	}
  
	if((BatteryVal > CarData->Voltage + 100) || (BatteryVal < CarData->Voltage - 100))
	{
		CarData->Voltage = BatteryVal;
	}
	return 0;
}
/****************************************************************************
* 名    称：NormalTimeout
* 功    能：正常处理
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
int8_t NormalTimeout(void *cbptr)
{
	if(CarData->ManualCmd != CAR_ACTION_STOP)
	{
  	if(CheckRoute(MagneticValue)) //根据磁传感器数据微调方向
		{				
			f_MagneticValueUseCount++;
      
			if(f_MagneticValueUseCount>20)
			{
					//CarErrorProcess(CAR_ERROR_MAGNETIC_CONN);
			}
      
			OutRouteCount=0;
		}
	  else
		{
			OutRouteCount++;
      
			if(OutRouteCount>20)
			{
				OutRouteCount=200;
				CarData->NowStatus = CAR_STATUS_OUT_ROUTE;
			}				
		}	
	}
	return 0;
}
const char *CmdChar[5] = {"CAR_ACTION_FORWARD","CAR_ACTION_TURN_LEFT","CAR_ACTION_TURN_RIGHT","CAR_ACTION_STOP","CAR_ACTION_BACK"};
/****************************************************************************
* 名    称：Manual_Mode_Process
* 功    能：手动控制主流程
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void Manual_Mode_Process(void)
{
	if(CalcLenTimerIndex == -1)//计算距离
		Malloc_Timer(1000,Auto_Mode,CalcLenTimeout,NULL,&CalcLenTimerIndex);
	if(Change_Flag.Env.BITS.ManualCmd)
	{
		Moto_Action_Process((CAR_ACTION)CarData->ManualCmd);
		LastCarData.ManualCmd = CarData->ManualCmd;
		Change_Flag.Env.BITS.ManualCmd = 0;
	}
	if(Change_Flag.Env.BITS.RunMode)
	{
		if(CalcLenTimerIndex == -1)
			Free_Timer(&CalcLenTimerIndex);
		memset((u8*)&Change_Flag,0x00,sizeof(Change_Flag));

	}
}

const char *StatusChar[] = {"CAR_STATUS_NORMAL","CAR_STATUS_STOP","CAR_STATUS_FAULT","CAR_STATUS_ROUTE_FBLOCK","CAR_STATUS_OUT_ROUTE",\
														"CAR_STATUS_MANUAL","CAR_STATUS_NORMAL","CAR_STATUS_URGENT_STOP","CAR_STATUS_ROUTE_BLOCK","CAR_STATUS_BATTERY_LOW","CAR_STATUS_UP"};
/****************************************************************************
* 名    称：Auto_Mode_Process
* 功    能：自动控制主流程
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void Auto_Mode_Process(void)
{	
	if(Change_Flag.Ext.BITS.STOP)     //紧急按钮事件发生
	{
		Change_Flag.Ext.BITS.STOP = 0;
		CarData->NowStatus = CAR_STATUS_URGENT_STOP;
	}
	
	if(Change_Flag.Ext.BITS.MagnetComm)//与磁传感器通信故障
	{
		Change_Flag.Ext.BITS.MagnetComm = 0;
		CarData->NowStatus = CAR_STATUS_FAULT;
	}	
	
	if(Change_Flag.Ext.BITS.IR_Far &&CarData->NowStatus != CAR_STATUS_ROUTE_FBLOCK)   //远障碍，减速
	{
		Change_Flag.Ext.BITS.IR_Far = 0;
		uiInfraFarTime = 0;
		CarData->NowStatus = CAR_STATUS_ROUTE_FBLOCK;
	}
	else 					                    //恢复正常							
	{
		if((CarData->NowStatus == CAR_STATUS_ROUTE_FBLOCK) && (uiInfraFarTime++ == INFRA_FAR_TIME) && startFlag)
		{
			uiInfraFarTime = INFRA_FAR_TIME;
			CarData->NowStatus = CAR_STATUS_NORMAL;
		}
	}
	
	if(Change_Flag.Ext.BITS.IR_Near)  //近障碍，停车
	{
		Change_Flag.Ext.BITS.IR_Near = 0;
		uiInfraNearTime = 0;
		CarData->NowStatus = CAR_STATUS_ROUTE_BLOCK;
	}
	else														 //恢复正常
	{
		if((CarData->NowStatus == CAR_STATUS_ROUTE_BLOCK) && (uiInfraNearTime++ == INFRA_NEAR_TIME) && startFlag)
    {
      uiInfraNearTime = INFRA_NEAR_TIME;
      CarData->NowStatus = CAR_STATUS_NORMAL;
    }
	}

  if(Change_Flag.Env.BITS.NowStatus)//小车状态改变
	{
		ChangeCarStatus((CAR_STATUS)CarData->NowStatus);
		Change_Flag.Env.BITS.NowStatus = 0;
	}
  
	if(Change_Flag.Env.BITS.ManualCmd)//电机命令改变
	{
		Moto_Action_Process((CAR_ACTION)CarData->ManualCmd);
		LastCarData.ManualCmd = CarData->ManualCmd;
		Change_Flag.Env.BITS.ManualCmd = 0;
	}
  
	if(Change_Flag.Env.BITS.RunMode)//运行模式改变
	{
		if(ChangeSpeedTimerIndex != -1)
			Free_Timer(&ChangeSpeedTimerIndex);
		if(CalcLenTimerIndex == -1)
			Free_Timer(&CalcLenTimerIndex);
		if(NormalTimerIndex == -1)
			Free_Timer(&NormalTimerIndex);
		
		memset((u8*)&Change_Flag,0x00,sizeof(Change_Flag));
	}

  if(ChangeSpeedTimerIndex == -1)//检查速度
  {
    Malloc_Timer(300,Auto_Mode,ChangeSpeedTimeout,NULL,&ChangeSpeedTimerIndex);
  }

  if(CalcLenTimerIndex == -1)//计算距离
  {
    Malloc_Timer(1000,Auto_Mode,CalcLenTimeout,NULL,&CalcLenTimerIndex);
  }

  if(NormalTimerIndex == -1)//根据磁传感器数据调整速度与方向
  {
    Malloc_Timer(10,Auto_Mode,NormalTimeout,NULL,&NormalTimerIndex);
  }
  
}




