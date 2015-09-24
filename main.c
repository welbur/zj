/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "API_Include.h"
#include "car.h"
#include "FS.h"
#include "loop.h"


/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

int8_t WorkLEDStatus(void *cbptr)
{
	GPIO_ToggleBits(WORK_LED_PORT,WORK_LED_PIN);
	//Get_Moto_Speed();
	return 0;
}
#if MODBUS_ENABLE
/****************************************************************************
* 名    称：void InitModbus(void)
* 功    能：初始化Modbus
* 入口参数：无
* 出口参数：
****************************************************************************/
void InitModbus(void)
{
    eMBInit(MB_RTU, 0x01, 0, 9600, MB_PAR_NONE);
    eMBEnable(); 
}
#endif

/****************************************************************************
* 名    称：void InitSystem(void)
* 功    能：系统初始化
* 入口参数：无
* 出口参数：
****************************************************************************/
void InitSystem(void)
{
	#if WATCHDOG_ENABLE
	IWDG_Configuration();
	#endif
	
	IO_Init();
	SYS_Timer_Init(Period_10MS);
	InitLRMotoCtrl();
	#if MODBUS_ENABLE
	InitModbus();
	#endif
	MagnetCommInit();
	USART2_Init(115200);

	USART6_Init(115200);

	ADC1_Init();
	FS_Init();
	RGB_LED_Init();
	VoiceInit();
	
	SetLedColor(LED_COLOR_BLUE);
	SetSpeakSwitch(SPEAK_SWITCH_ON);
	
	Para_Init();
	OutsideCommInit(CarData);
	
	Delay(200);//等待外部器件准备就绪
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	InitSystem();
	
	Malloc_Timer(1000,Auto_Mode,WorkLEDStatus,NULL,NULL);
	
  /* Infinite loop */
  while (1)
  {
		#if WATCHDOG_ENABLE
		IWDG_ReloadCounter();	//喂狗
		#endif
	#if 0		
		#if MODBUS_ENABLE
		eMBPoll();
		#endif
		MagnetRecvCheck(MagnetValidDataProcess);//接收磁传感器数据处理
		OutsideRecvCheck(OutsideValidDataProcess);//接收上位机数据处理
		Timer_Check();//定时器回调函数处理
		CheckConfigChange();//配置,事件,中断改变处理
		
	#endif		
		Main_Loop_Process();	

  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
