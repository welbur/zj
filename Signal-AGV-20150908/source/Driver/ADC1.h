
#ifndef YD_ADC1_H
#define YD_ADC1_H

#include "Global_Config.h"

/*pin*/
#define ADC1_CH11_PIN   GPIO_Pin_1


/*port*/

#define ADC1_CH11_PORT       GPIOC

/*clk*/
#define ADC1_CH11_PORT_CLK       RCC_AHB1Periph_GPIOC

#define SAMP_NUM    5     /*ÿ��ͨ����������,ADC�㷨��ȥ�����ֵ����Сֵ,���Դ�ֵ�������2*/
#define CHN_NUM     1     /*ͨ������*/

#define VREF        3260   /*ADC�ο���ѹ*/

#define BAT_V       0     /*��ص�������������е�λ��*/

extern __IO uint16_t  AD1_Val[SAMP_NUM][CHN_NUM]; /*��Ų���ֵ*/
extern __IO uint16_t  Aver_Val[CHN_NUM];    /*�����ƽ��ֵ֮��Ľ��*/

/***************************************************/
/* �������ܣ���ʼ��ADC1         */
/* ��ڲ�������                                    */
/* ˵����                                          */
/***************************************************/
void ADC1_Init(void);
/***************************************************/
/* �������ܣ�ʹ��ADC����       */
/* ��ڲ�������                                    */
/* ˵����                                          */
/***************************************************/

void ADC_ENABLE(void);
/***************************************************/
/* �������ܣ�ʧЧADC����       */
/* ��ڲ�������                                    */
/* ˵����                                          */
/***************************************************/

void ADC_DISABLE(void);

uint16_t Get_ADC_Val(uint8_t Chn);


extern __IO uint16_t  AD1_Val[SAMP_NUM][CHN_NUM]; /*��Ų���ֵ*/
extern __IO uint16_t  Aver_Val[CHN_NUM];    /*�����ƽ��ֵ֮��Ľ��*/



#endif

















