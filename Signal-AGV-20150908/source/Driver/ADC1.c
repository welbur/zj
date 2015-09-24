

#include "ADC1.h"

__IO uint16_t  AD1_Val[SAMP_NUM][CHN_NUM]; /*存放采样值*/
__IO uint16_t  Aver_Val[CHN_NUM];    /*存放求平均值之后的结果*/
/***************************************************/
/* 函数功能；初始化ADC1         */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/
void ADC1_Init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	
	/* Enable ADC1, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | ADC1_CH11_PORT_CLK , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD1_Val;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = SAMP_NUM * CHN_NUM;  /*有CHN_NUM路ADC 需要采样SAMP_NUM次*/
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  
  /* Configure the ADC1_CH1 pin  ********************************************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  
  GPIO_InitStructure.GPIO_Pin = ADC1_CH11_PIN;  
  GPIO_Init(ADC1_CH11_PORT, &GPIO_InitStructure);


  
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  /* ADC1 Init ****************************************************************/
  ADC_DeInit();
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = CHN_NUM;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 regular channel11configuration *************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_84Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

//  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADC1);

   DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);                                //DMA5传输完成中断

   NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
	//ADC_DISABLE();
}

/***************************************************/
/* 函数功能；对CHN_NUM 个通道去掉最高值和最小值后求平均值       */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/

void Average(void)
{
   u8 i;
	 u32  max[CHN_NUM],min[CHN_NUM],sum[CHN_NUM];
	 u8   chn,count;	 
//DMA_Cmd(DMA2_Stream0, DISABLE);	
   for(i = 0; i < CHN_NUM; i++)
	   {
			 max[i] = min[i] = sum[i] = AD1_Val[0][i];
		 }
	for( chn = 0; chn < CHN_NUM; chn++)
    {
      for(count = 1; count < SAMP_NUM; count++)
			  {
					if(AD1_Val[count][chn] < min[chn])
						min[chn] = AD1_Val[count][chn];
					if(AD1_Val[count][chn] > max[chn])
						max[chn] = AD1_Val[count][chn];
					sum[chn] += AD1_Val[count][chn];
				}
		}			
	for( i = 0; i < CHN_NUM; i++)
     {
       //sum[i] = sum[i] - max[i] - min[i];
			 Aver_Val[i] = sum[i] / (SAMP_NUM /*- 2*/)	;
			 Aver_Val[i] = AD1_Val[0][0]/*Aver_Val[i]*/ * VREF / 4096;  //	
		}

	//Aver_Val[BAT_V] = Aver_Val[BAT_V] * 2;   //电池电量检测经过2/1分压后接到ADC1脚	
    
}

/***************************************************/
/* 函数功能；使能ADC采样       */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/

void ADC_ENABLE(void)
{
	ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

/***************************************************/
/* 函数功能；失效ADC采样       */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/

void ADC_DISABLE(void)
{
	ADC_Cmd(ADC1, DISABLE);
	DMA_Cmd(DMA2_Stream0, DISABLE);
}

/***************************************************/
/* 函数功能；获取某路ADC值*/
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/

uint16_t Get_ADC_Val(uint8_t Chn)
{

	return Aver_Val[Chn];
}


/**
 * ISR for ADC1  DMA Stream Interrupt
 */
 void DMA2_Stream0_IRQHandler()
 {
   // Stream2 transfer complete interrupt?
   if(DMA_GetITStatus(DMA2_Stream0,DMA_IT_TCIF0))
     {
			 Average();
       // clear pending interrupt
       DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);
  
     }
   DMA_ClearFlag(DMA2_Stream0,DMA_IT_TEIF0);
  
 }  

