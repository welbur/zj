
#ifndef YD_ADC1_H
#define YD_ADC1_H

#include "Global_Config.h"

/*pin*/
#define ADC1_CH11_PIN   GPIO_Pin_1


/*port*/

#define ADC1_CH11_PORT       GPIOC

/*clk*/
#define ADC1_CH11_PORT_CLK       RCC_AHB1Periph_GPIOC

#define SAMP_NUM    5     /*每个通道采样次数,ADC算法中去年最大值和最小值,所以此值必须大于2*/
#define CHN_NUM     1     /*通道个数*/

#define VREF        3260   /*ADC参考电压*/

#define BAT_V       0     /*电池电量检测在数组中的位置*/

extern __IO uint16_t  AD1_Val[SAMP_NUM][CHN_NUM]; /*存放采样值*/
extern __IO uint16_t  Aver_Val[CHN_NUM];    /*存放求平均值之后的结果*/

/***************************************************/
/* 函数功能；初始化ADC1         */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/
void ADC1_Init(void);
/***************************************************/
/* 函数功能；使能ADC采样       */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/

void ADC_ENABLE(void);
/***************************************************/
/* 函数功能；失效ADC采样       */
/* 入口参数；无                                    */
/* 说明：                                          */
/***************************************************/

void ADC_DISABLE(void);

uint16_t Get_ADC_Val(uint8_t Chn);


extern __IO uint16_t  AD1_Val[SAMP_NUM][CHN_NUM]; /*存放采样值*/
extern __IO uint16_t  Aver_Val[CHN_NUM];    /*存放求平均值之后的结果*/



#endif

















