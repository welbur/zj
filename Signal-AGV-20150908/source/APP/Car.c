
#include "API_Include.h"
#include "Car.h"
#include "FS.h"


CAR_ATTRIBUTE *CarData;					//指向人机寄存器的指针
CAR_ATTRIBUTE LastCarData;      //最近一次人机寄存器的指针

DRIVER_INIT_PARAMTER DriverParamter;	//驱动行驶的参数
DRIVER_RUN_STATUS NowRunStatus;			  //当前状态数据
CAR_ACTION NowAction=CAR_ACTION_STOP; //,LastAction=CAR_ACTION_STOP;	//当前执行的动作指令
RFID *rf;                             //RFID数据
_Change_Flag Change_Flag;             //任何状态的改变或外部事件的发生

uint8_t TurnFlagCount;                //判断转向调节转速次数
uint16_t MagneticValue=0;				      //磁导航器检测到的数据
uint8_t f_MagneticValueUseCount=0;    //当前数据使用次数
uint16_t MagneticRecvCount = 0,LastMagneticRecvCount = 0;  //接收到磁传感器次数

static int8_t Turn_Left_Count_Timer_Index = -1;            //左右转向定时器
static int8_t Turn_Right_Count_Timer_Index = -1;
uint8_t Turn_Left_Count,Turn_Right_Count;

MOTOR_PARAMETER mp;
CHECK_STRUCT chkValid,LastValid;
uint8_t	g_f_TurnFlag;               //转向标志
uint8_t f_TurnFinished = 0;         //转向完成标志
uint8_t CMD = 0;

/****************************************************************************
* 名    称：MagnetValidDataProcess
* 功    能：接收到磁传感器上有效数据的处理
* 参    数：data:数据区; len:数据长度
* 反    回：无
* 说    明：
****************************************************************************/
void MagnetValidDataProcess(char *buf,uint16_t len)
{
	CMD = buf[1];
	
	switch(CMD)
	{
		case CMD_MAGNET_CHECK_DATA:
		{
			MagneticValue = buf[DATA_START_POSITION] << 8 | buf[DATA_START_POSITION+1];
			f_MagneticValueUseCount = 0;
			MagneticRecvCount++;
			break;
		}
		case CMD_MAGNET_RFID_CARD:
		{
			MagnetSendACK(ACK_PASS);
			RecvDriverReadRFIDProcess((u8*)&buf[DATA_START_POSITION]);
			break;
		}
	}
}
/****************************************************************************
* 名    称：MagnetValidDataProcess
* 功    能：接收到磁传感器上有效数据的处理
* 参    数：data:数据区; len:数据长度
* 反    回：无
* 说    明：
****************************************************************************/
void InsideValidDataProcess(char *buf,uint16_t len)
{
	uint8_t CMD = buf[0];
	
	switch(CMD)
	{
		case CMD_DRIVER_INIT_PARAMTER:
		{
			//InitCarProcess(&buf[DATA_START_POSITION]);
			break;
		}
		case CMD_DRIVER_RUN_ACTION:
		{
			//GoActionProcess((CAR_ACTION)buf[DATA_START_POSITION]);
			break;
		}
	}	
}
/****************************************************************************
* 名    称：OutsideValidDataProcess
* 功    能：收到路径序列时的处理过程
* 参    数：data 为指向路径序列的首地址，len 为路径序列的数组长度
* 反    回：无
* 说    明：
****************************************************************************/
void OutsideValidDataProcess(uint8_t *buf,uint16_t len)
{
	memcpy((u8*)&Route_Save_Info,buf,len);
	Route_Save_Info.TotalNum = len / sizeof(ROUTE_SERIAL_INFO);
	SPI_Flash_Write(ROUTE_CFG_OFFSET,(u8*)&Route_Save_Info,sizeof(Route_Save_Info));
}

/****************************************************************************
* 名    称：void RouteFindProcess()
* 功    能：跟据RFID卡号在路径序列里查找看有没有相关的信息
* 参    数：rf 指向卡号结构体里的数据
* 反    回：无
* 说    明：
****************************************************************************/
void RouteFindProcess(RFID *rf)
{
  uint16_t i;
  ROUTE_SERIAL_INFO	*rsi;

  if(CarData->RunMode==CAR_MODE_AUTO)
  {
    for (i=0; i<Route_Save_Info.TotalNum; i++)
    {
      rsi=&Route_Save_Info.Arr[i];
      
      if((rsi->CardIDMSB==rf->CardIDMSB)&&(rsi->CardIDLSB==rf->CardIDLSB))
      {
         CarData->ManualCmd = rsi->Action;
      }
    }
  }

}
/****************************************************************************
* 名    称：void RecvDriverReadRFID(uint8_t *data)
* 功    能：收到RFID卡号数据处理
* 参    数：data 为指向数据区域的地址
* 反    回：无
* 说    明：
****************************************************************************/
void RecvDriverReadRFIDProcess(uint8_t *data)
{
  uint16_t cardNum = 0;  
  
  rf=(RFID *)data;
  cardNum = (rf->CardIDMSB << 8) + (rf->CardIDLSB);         //卡号
  CarData->LastStation=(rf->CardIDMSB<<8) | rf->CardIDLSB;  //上一站站点
  
	if((CarData->NowStatus!=CAR_STATUS_FAULT) && (CarData->NowStatus!=CAR_STATUS_OUT_ROUTE) &&
		(CarData->NowStatus!=CAR_STATUS_STOP) && (CarData->NowStatus!=CAR_STATUS_URGENT_STOP) && 
		(CarData->NowStatus!=CAR_STATUS_UP))
	{
		SetLedColor(LED_COLOR_RED);
    
		switch(rf->CardType)
		{
			case CARD_TYPE_LIFT:
			{
				CarData->NowStatus = CAR_STATUS_STOP;

        if(cardNum == LIFTER_UP)          //升降杆上升
        {
          LifterUp();
        }
        else if(cardNum == LIFTER_DOWN)   //升降杆下降
        {
          LifterDown();
        }
        
				break;
			}
			case CARD_TYPE_MUTE:
			{
				if(rf->CardIDLSB)
				{
					SetSpeakSwitch(SPEAK_SWITCH_ON);
				} 
        else
				{
					SetSpeakSwitch(SPEAK_SWITCH_OFF);
				}
				break;
			}
			case CARD_TYPE_SIGNAL:
			{
				break;
			}
			case CARD_TYPE_SITE:
			{
				CarData->NowStatus = CAR_STATUS_STOP;
        
				if(CarData->LastStation==CarData->Destination)
				{
					CarData->Destination=0;
					CarData->NowStatus = CAR_STATUS_STOP;
					Speak(SPEAK_AGV_ARRIV,0);
				}
        else
				{
					RouteFindProcess(rf);
				}
				
				break;
			}
			case CARD_TYPE_SPEED:
			{
        if(cardNum == SPEED_UP)          //加速
        {
          //mp.CurrSetSpeed += 1;
        }
        else if(cardNum == SPEED_DOWN)   //减速下降
        {
          //mp.CurrSetSpeed -= 1;
        }
        
				break;
			}
			case CARD_TYPE_TURN:
			{
				RouteFindProcess(rf);
				break;
			}
			default:
			{
        break;
			}
		}
	}


}
/****************************************************************************
* 名    称：RefreshLastCarPara
* 功    能：更新最近的人机寄存器参数
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
void RefreshLastCarPara(void)
{
	LastCarData.AGVID          = Car_Save_Info.CarPara.AGVID;
	LastCarData.SetSpeed       = Car_Save_Info.CarPara.SetSpeed;
	LastCarData.RunMode        = Car_Save_Info.CarPara.RunMode;
	LastCarData.StopTime       = Car_Save_Info.CarPara.StopTime;
	LastCarData.PValue         = Car_Save_Info.CarPara.PValue;
	LastCarData.DValue         = Car_Save_Info.CarPara.DValue;
	LastCarData.VoltageAdjust  = Car_Save_Info.CarPara.VoltageAdjust;
	LastCarData.UseLimit       = Car_Save_Info.CarPara.UseLimit;

	LastCarData.ManualCmd	    = CarData->ManualCmd;
	LastCarData.NowSpeed 	    = CarData->NowSpeed;
	LastCarData.Voltage   	  = CarData->Voltage;
	LastCarData.NowStatus 	  = CarData->NowStatus;
	LastCarData.LastStation 	= CarData->LastStation;
	LastCarData.Distance		  = CarData->Distance;
	LastCarData.NextStation 	= CarData->NextStation;
	LastCarData.Destination 	= CarData->Destination;
	
}

/****************************************************************************
* 名    称：Para_Init
* 功    能：全局参数初始化
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
void Para_Init(void)
{
	
	CarData=(CAR_ATTRIBUTE *)&usRegHoldingBuf[0];
	memcpy((u8*)CarData,(u8*)&Car_Save_Info.CarPara,14);
	CarData->UseLimit = Car_Save_Info.CarPara.UseLimit;
  CarData->NowStatus = CAR_STATUS_STOP;
	CarData->ManualCmd = CAR_ACTION_STOP;
  //if(Route_Save_Info.TotalNum)//有路径,上电默认进入自动模式
  CarData->RunMode = CAR_MODE_AUTO;
  //CarData->SetSpeed = 15;
  CarData->Voltage = 240;
	RefreshLastCarPara();

	mp.CurrSetSpeed = 3;
	mp.PrevDeviation = 0;
	mp.SetBS = 7;
	mp.SpeedBPWM = 1;
	
	CarData->AGVID = 0x0001;
}


/****************************************************************************
* 名    称：void BreakCar()
* 功    能：停止动作
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void BreakCar(void)
{
	BrakeMoto();
	mp.CurrSetSpeed = 0;
}

/****************************************************************************
* 名    称：void StopCar()
* 功    能：停止动作
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void StopCar(void)
{
	//BreakCar();
	StopMoto();
	mp.CurrSetSpeed = 0;
	CarData->ManualCmd = CAR_ACTION_STOP;
}

/****************************************************************************
* 名    称：void CheckConfigChange(void)
* 功    能：检查配置是否发生改变
* 参    数：无
* 反    回：无
* 说    明：
****************************************************************************/
void CheckConfigChange(void)
{
	uint8_t IsChanged=0;
	//////////////////////////////////环境事件改变///////////////////////////////
	if(CarData->AGVID != LastCarData.AGVID)
	{
		LastCarData.AGVID = CarData->AGVID;
		Car_Save_Info.CarPara.AGVID = CarData->AGVID;
		IsChanged = 1;
		Change_Flag.Env.BITS.AGVID = 1;
	}
	if(CarData->SetSpeed != LastCarData.SetSpeed)
	{
		LastCarData.SetSpeed = CarData->SetSpeed;
		Car_Save_Info.CarPara.SetSpeed = CarData->SetSpeed;
		IsChanged = 1;
		Change_Flag.Env.BITS.SetSpeed = 1;
	}
	if(CarData->RunMode != LastCarData.RunMode)
	{
		LastCarData.RunMode = CarData->RunMode;
		Car_Save_Info.CarPara.RunMode = CarData->RunMode;
		IsChanged = 1;
		Change_Flag.Env.BITS.RunMode = 1;
	}
	if(CarData->StopTime != LastCarData.StopTime)
	{
		LastCarData.StopTime = CarData->StopTime;
		Car_Save_Info.CarPara.StopTime = CarData->StopTime;
		IsChanged = 1;
		Change_Flag.Env.BITS.StopTime = 1;
	}
	if(CarData->PValue != LastCarData.PValue)
	{
		LastCarData.PValue = CarData->PValue;
		Car_Save_Info.CarPara.PValue = CarData->PValue;
		IsChanged = 1;
		Change_Flag.Env.BITS.PValue = 1;
	}
	if(CarData->DValue != LastCarData.DValue)
	{
		LastCarData.DValue = CarData->DValue;
		Car_Save_Info.CarPara.DValue = CarData->DValue;
		IsChanged = 1;
		Change_Flag.Env.BITS.DValue = 1;
	}
	if(CarData->VoltageAdjust != LastCarData.VoltageAdjust)
	{
		LastCarData.VoltageAdjust = CarData->VoltageAdjust;
		Car_Save_Info.CarPara.VoltageAdjust = CarData->VoltageAdjust;
		IsChanged = 1;
		Change_Flag.Env.BITS.VoltageAdjust = 1;
	}
	if(CarData->UseLimit != LastCarData.UseLimit)
	{
		LastCarData.UseLimit = CarData->UseLimit;
		Car_Save_Info.CarPara.UseLimit = CarData->UseLimit;
		IsChanged = 1;
		Change_Flag.Env.BITS.UseLimit = 1;
	}	
	if(CarData->ManualCmd != LastCarData.ManualCmd)
	{
		LastCarData.ManualCmd = CarData->ManualCmd;
		Change_Flag.Env.BITS.ManualCmd = 1;
	}
	if(CarData->NowSpeed != LastCarData.NowSpeed)
	{
		LastCarData.NowSpeed = CarData->NowSpeed;
		Change_Flag.Env.BITS.NowSpeed = 1;
	}
	if(CarData->Voltage != LastCarData.Voltage)
	{
		LastCarData.Voltage = CarData->Voltage;
		Change_Flag.Env.BITS.Voltage = 1;
	}
	if(CarData->NowStatus != LastCarData.NowStatus)
	{
		LastCarData.NowStatus = CarData->NowStatus;
		Change_Flag.Env.BITS.NowStatus = 1;
	}
	if(CarData->LastStation != LastCarData.LastStation)
	{
		LastCarData.LastStation = CarData->LastStation;
		Change_Flag.Env.BITS.LastStation = 1;
	}
	if(CarData->Distance != LastCarData.Distance)
	{
		LastCarData.Distance = CarData->Distance;
		Change_Flag.Env.BITS.Distance = 1;
	}
	if(CarData->NextStation != LastCarData.NextStation)
	{
		LastCarData.NextStation = CarData->NextStation;
		Change_Flag.Env.BITS.NextStation = 1;
	}
	if(CarData->Destination != LastCarData.Destination)
	{
		LastCarData.Destination = CarData->Destination;
		Change_Flag.Env.BITS.Destination = 1;
	}
	
	//////////////////////////////////外部事件发生///////////////////////////////
	if(!GPIO_ReadInputDataBit(IR_Far_PORT, IR_Far_PIN))		//红外远障碍
	{
		Change_Flag.Ext.BITS.IR_Far = 1;
	}
	
	if(!GPIO_ReadInputDataBit(IR_Near_PORT, IR_Near_PIN))	//红外近障碍
	{
		Change_Flag.Ext.BITS.IR_Near = 1;
	}

	if(INT_Event.BITS.LRS_Far_Flag)
	{
		INT_Event.BITS.LRS_Far_Flag = 0;
		Change_Flag.Ext.BITS.LRS_Far = 1;
	}
	if(INT_Event.BITS.LRS_Near_Flag)
	{
		INT_Event.BITS.LRS_Near_Flag = 0;
		Change_Flag.Ext.BITS.LRS_Near = 1;
	}
	if(INT_Event.BITS.STOP_Flag)
	{
		INT_Event.BITS.STOP_Flag = 0;
		Change_Flag.Ext.BITS.STOP = 1;
	}
	if(INT_Event.BITS.LT_UP_Flag)
	{
		INT_Event.BITS.LT_UP_Flag = 0;
		Change_Flag.Ext.BITS.LT_UP = 1;
	}
	if(INT_Event.BITS.LT_DOWN_Flag)
	{
		INT_Event.BITS.LT_DOWN_Flag = 0;
		Change_Flag.Ext.BITS.LT_DOWN = 1;
	}
	if(INT_Event.BITS.KEY1_Flag)
	{
		INT_Event.BITS.KEY1_Flag = 0;
		Change_Flag.Ext.BITS.KEY1 = 1;
	}
	if(INT_Event.BITS.KEY2_Flag)
	{
		INT_Event.BITS.KEY2_Flag = 0;
		Change_Flag.Ext.BITS.KEY2 = 1;
	}
	if(INT_Event.BITS.KEY3_Flag)
	{
		INT_Event.BITS.KEY3_Flag = 0;
		Change_Flag.Ext.BITS.KEY3 = 1;
	}
	if(INT_Event.BITS.KEY4_Flag)
	{
		INT_Event.BITS.KEY4_Flag = 0;
		Change_Flag.Ext.BITS.KEY4 = 1;
	}
	
	if(IsChanged)
	{
		SPI_Flash_Write(CAR_CFG_OFFSET,(u8*)&Car_Save_Info,sizeof(Car_Save_Info));
	}		
}

/****************************************************************************
* 名    称：Turn_Left_Count_Timeout
* 功    能：左转向处理
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
int8_t Turn_Left_Count_Timeout(void *cbptr)
{
	Turn_Left_Count++;
	
	if(Turn_Left_Count < MAXTRUNCOUNT)
		Set_Moto_Speed(CarData->SetSpeed*15/10,CarData->SetSpeed*5/10);
	else
	{
		Turn_Left_Count = 0;
		if(Turn_Left_Count_Timer_Index != -1)
			Free_Timer(&Turn_Left_Count_Timer_Index);
		CarData->ManualCmd = CAR_ACTION_FORWARD;
	}
	return 0;
}
/****************************************************************************
* 名    称：Turn_Right_Count_Timeout
* 功    能：右转向处理
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
int8_t Turn_Right_Count_Timeout(void *cbptr)
{
	Turn_Right_Count++;
	if(Turn_Right_Count < MAXTRUNCOUNT)
		Set_Moto_Speed(CarData->SetSpeed*5/10,CarData->SetSpeed*15/10);
	else
	{
		Turn_Right_Count = 0;
		if(Turn_Right_Count_Timer_Index != -1)
			Free_Timer(&Turn_Right_Count_Timer_Index);
		CarData->ManualCmd = CAR_ACTION_FORWARD;
	}
	return 0;
}
/****************************************************************************
* 名    称：Moto_Action_Process
* 功    能：电机根据指令执行动作
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void Moto_Action_Process(CAR_ACTION MotoCmd)
{
	switch(MotoCmd)
	{
		case CAR_ACTION_FORWARD://前进
			SetMotoForward();
    
			if(CarData->RunMode == CAR_MODE_MANUAL)
				Set_Moto_Speed(CarData->SetSpeed,CarData->SetSpeed);
      
		  break;
      
		case CAR_ACTION_BACK://后退
			SetMotoBack();
      
			if(CarData->RunMode == CAR_MODE_MANUAL)
				Set_Moto_Speed(CarData->SetSpeed,CarData->SetSpeed);
      
		  break;
      
		case CAR_ACTION_TURN_LEFT://左转
			if(CarData->RunMode == CAR_MODE_MANUAL)//自动模式根据磁传感器的数据自动进行转弯
			{
				Set_Moto_Speed(CarData->SetSpeed*15/10,CarData->SetSpeed*5/10);
				Turn_Left_Count = 0;
        
				if(Turn_Left_Count_Timer_Index != -1)
					Free_Timer(&Turn_Left_Count_Timer_Index);
        
				Malloc_Timer(10,Auto_Mode,Turn_Left_Count_Timeout,NULL,&Turn_Left_Count_Timer_Index);
			}
		  break;
      
		case CAR_ACTION_TURN_RIGHT://右转
			if(CarData->RunMode == CAR_MODE_MANUAL)//自动模式根据磁传感器的数据自动进行转弯
			{
				Set_Moto_Speed(CarData->SetSpeed*5/10,CarData->SetSpeed*15/10);
				Turn_Right_Count = 0;
        
				if(Turn_Right_Count_Timer_Index != -1)
					Free_Timer(&Turn_Right_Count_Timer_Index);
        
				Malloc_Timer(10,Auto_Mode,Turn_Right_Count_Timeout,NULL,&Turn_Right_Count_Timer_Index);
			}
		  break;
    
		case CAR_ACTION_BREAK://刹车
			BreakCar();
		  break;
    
		case CAR_ACTION_STOP: //普通停车
			StopCar();
		  break;
    
		case CAR_ACTION_URGENT_STOP://紧急停车
			BreakCar();
			StopCar();
		  break;
    
		default:
			//BreakCar();
			//StopCar();
		  break;
	}
  
	g_f_TurnFlag = MotoCmd;
}
/****************************************************************************
* 名    称：void ChangeCarStatus(CAR_STATUS cs)
* 功    能：更改小车的状态
* 参    数：cs 要变更的状态
* 反    回：无
* 说    明：
****************************************************************************/
void ChangeCarStatus(CAR_STATUS cs)
{
  switch(cs)
  {
    case CAR_STATUS_FAULT://目前用于磁传感通信出错
    {
  		CarData->ManualCmd = CAR_ACTION_URGENT_STOP;
  		SetLedColor(RED_GRN);
      Speak(SPEAK_FAULT,0);
      break;
    }
    
    case CAR_STATUS_OUT_ROUTE://出轨
    {
  		CarData->ManualCmd = CAR_ACTION_URGENT_STOP;
      SetLedColor(RED_GRN);
      Speak(SPEAK_OUT_ROUTE,1);
      break;
    }
    
    case CAR_STATUS_ROUTE_BLOCK://红外近
    {
  		CarData->ManualCmd = CAR_ACTION_STOP;
  		SetLedColor(BLU);
      Speak(SPEAK_ROUTE_BLOCK,1);
      break;
    }
    
    case CAR_STATUS_URGENT_STOP://紧急停车
    {
  		CarData->ManualCmd = CAR_ACTION_URGENT_STOP;
      SetLedColor(RED_GRN);
      Speak(SPEAK_URGENT_STOP,1);
      break;
    }
    
    case CAR_STATUS_ROUTE_FBLOCK://红外远
    {
      CarData->ManualCmd = CAR_ACTION_FORWARD;
  		SetLedColor(GRN);
      Speak(SPEAK_BE_CAREFUL,1);
      break;
    }
    
    case CAR_STATUS_BATTERY_LOW://电量低
    {
      Speak(SPEAK_BATTERY_LOW,1);
      break;
    }
    
    case CAR_STATUS_UP://开机
    {
      Speak(SPEAK_WELCOME,1);
      break;
    }
    
    case CAR_STATUS_MANUAL://改为手动
    {
      SetLedColor(GRN_BLU);
      break;
    }
    
    case CAR_STATUS_STOP://停车
    {
  		//CarData->ManualCmd = CAR_ACTION_STOP;
  		CarData->ManualCmd = CAR_ACTION_BREAK;
      SetLedColor(LED_COLOR_BLUE);
      Speak(SPEAK_MUTE,1);
      break;
    }
    case CAR_STATUS_NORMAL://正常
    {
  		CarData->ManualCmd = CAR_ACTION_FORWARD;
      
      
      if(Change_Flag.Ext.BITS.BatteryLow)   //电量低
      {
        //CarData->NowStatus=CAR_STATUS_BATTERY_LOW;
        Speak(SPEAK_BATTERY_LOW,1);
      }
  		else
      {
        SetLedColor(GRN);
        Speak(SPEAK_MUSIC,1);
      }

      break;
    }
  }
}


/****************************************************************************
* 名    称：void Run(int16_t runValue)
* 功    能：将计算后的值经过修正之后再发送PWM
* 参    数： 
* 反    回：无
* 说    明：
****************************************************************************/
void Run(int16_t runValue)
{
    int8_t dutyCycle;
    dutyCycle=(int8_t)mp.CurrSetSpeed* (mp.SpeedBPWM);							//按当前速度和速度比值得出相应的占空比
    Set_Moto_Speed(dutyCycle+runValue,dutyCycle-runValue);
}
/****************************************************************************
* 名    称：void InitCheckRoute(void)
* 功    能：初始化寻路数据
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
void InitCheckRoute(void)
{
    chkValid.ValidCount = 0;
    chkValid.MinPoint = 15;
    chkValid.NearPoint = 7;
    chkValid.MaxPoint = 0;
}


/****************************************************************************
* 名    称：int16_t CalcPD(int8_t cvData)
* 功    能：PID计算，这里只计算PD两个值
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
int16_t CalcPD(int8_t cvData)
{
    int8_t dDeviation, Deviation;
	if(mp.CurrSetSpeed < 10)
	{
		return 0;
	}
  else
	{
		Deviation = mp.SetBS - cvData;
		dDeviation = Deviation - mp.PrevDeviation;
		mp.PrevDeviation += dDeviation;
    
		return (CarData->PValue * Deviation  +  CarData->DValue * dDeviation );
	}
}
/****************************************************************************
* 名    称：int8_t CalcCheckValue(void)
* 功    能：根据转向指令返回计算后的值
* 参    数：
* 反    回：无
* 说    明：
****************************************************************************/
int8_t CalcCheckValue(void)
{
  if(chkValid.ValidCount>0x05)												//有效位大于5个就说明有分叉口了
  {
	  f_TurnFinished=1;
    
    switch(g_f_TurnFlag)
    {
			case CAR_ACTION_TURN_LEFT: 
      {
				return chkValid.MaxPoint;
			}
			case CAR_ACTION_TURN_RIGHT: 
      {
			  return chkValid.MinPoint;
      }
			default: 
      {
				return chkValid.NearPoint;
			}
    }
  } 
	else
  {
		if(f_TurnFinished)
		{//转弯后直走
			f_TurnFinished=0;
			g_f_TurnFlag=CAR_ACTION_FORWARD;
			CarData->ManualCmd = CAR_ACTION_FORWARD;
		}
    
    return chkValid.NearPoint;
  }

}

/****************************************************************************
* 名    称：int8_t CheckRoute(uint16_t CheckValue)
* 功    能：根据导航器发来的数据进行计算
* 参    数：CheckValue  导航器上传的数据 
* 反    回：无
* 说    明：
****************************************************************************/
int16_t CheckRoute(uint16_t CheckValue)
{
    uint8_t	i;
    uint8_t finded=0;
    uint8_t nowNear=7,preNear=16;

    memcpy(&LastValid,&chkValid,sizeof(LastValid));
    InitCheckRoute();
    
    for (i=0; i<16; i++)														//遍历16位数据，得出最大数、最小数及最接近数
    {
        if(((CheckValue>>i)&0x0001) && ((CheckValue>>(i+1))&0x0001))
        {
            finded=1;
            chkValid.ValidCount++;
            if(chkValid.MaxPoint<i)
                chkValid.MaxPoint=i;
            if(chkValid.MinPoint>i)
                chkValid.MinPoint=i;
            if(i<7)
            {
                nowNear=7-i;
            } else
            {
                nowNear=i-7;
            }
            if(preNear>nowNear)
            {
                chkValid.NearPoint=i;
                preNear=nowNear;
            }

        } else
        {
            if(finded)
            {
                chkValid.ValidCount++;
                finded=0;
            }
        }
    }
    
    if(chkValid.ValidCount>1)												//有效位数大于1才算有效，否则判定为出轨
    {
        Run(CalcPD(CalcCheckValue()));						  //跟据转向方向计算转向值，最后再通过比例及微分计算后，得出修正值
        return  1;
    } 
    else
    {
        return 0;
    }

}

