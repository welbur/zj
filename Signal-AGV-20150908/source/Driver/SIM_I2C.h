
#ifndef YD_I2C_H
#define YD_I2C_H
#include "stm32f4xx.h"


//#define SCL_H         GPIO_SetBits(GPIOB,GPIO_Pin_6)//GPIOB->BSRRL = GPIO_Pin_6 
//#define SCL_L         GPIO_ResetBits(GPIOB,GPIO_Pin_6)//GPIOB->BSRRH  = GPIO_Pin_6  
//    
//#define SDA_H         GPIO_SetBits(GPIOB,GPIO_Pin_7)//GPIOB->BSRRL = GPIO_Pin_7 
//#define SDA_L         GPIO_ResetBits(GPIOB,GPIO_Pin_7)//GPIOB->BSRRH  = GPIO_Pin_7 

//#define SCL_read      (GPIOB->IDR  & GPIO_Pin_6) 
//#define SDA_read      (GPIOB->IDR  & GPIO_Pin_7)

#define SCL_PIN          GPIO_Pin_6
#define SCL_PORT         GPIOB
#define SCL_CLK          RCC_AHB1Periph_GPIOB

#define SDA_PIN          GPIO_Pin_7
#define SDA_PORT         GPIOB
#define SDA_CLK          RCC_AHB1Periph_GPIOB

#define SCL_H         GPIO_SetBits(SCL_PORT,SCL_PIN)//GPIOB->BSRRL = GPIO_Pin_6 
#define SCL_L         GPIO_ResetBits(SCL_PORT,SCL_PIN)//GPIOB->BSRRH  = GPIO_Pin_6  
    
#define SDA_H         GPIO_SetBits(SDA_PORT,SDA_PIN)//GPIOB->BSRRL = GPIO_Pin_7 
#define SDA_L         GPIO_ResetBits(SDA_PORT,SDA_PIN)//GPIOB->BSRRH  = GPIO_Pin_7 

#define SCL_read      (SCL_PORT->IDR  & SCL_PIN) 
#define SDA_read      (SDA_PORT->IDR  & SDA_PIN)

void SIM_I2C_Init(void);
void I2C_delay(void);
unsigned char I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
unsigned char I2C_WaitAck(void);
void I2C_SendByte( char SendByte);
unsigned char I2C_ReceiveByte(void);
unsigned char I2C_Read_Byte(unsigned char DevAddr, unsigned char RegAddr,  char *Char);
unsigned char I2C_Write_Byte(unsigned char DevAddr, unsigned char RegAddr, char Char);
unsigned char I2C_Read_Str(unsigned char DevAddr, unsigned char RegAddr, unsigned char *Buf,unsigned int Len);



//void I2C_Config( void );

//extern unsigned char I2C_WriteOneByte(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char value);
////**入口参数: I2C_TypeDef *I2Cx,STM32 的I2C 寄存器基址，从器件地址,addr 预写入字节地址，value 写入数据
////**出口参数: 成功返回0

//extern unsigned char I2C_ReadOneByte(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr);
////**入口参数: I2C_TypeDef *I2Cx,STM32 的I2C 寄存器基址，从器件地址,addr 预读取字节存储位置
////**出口参数: 读取到的数值

//extern  unsigned char I2C_Read(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char *buf,unsigned char num);

//extern  unsigned char I2C_Write(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char *buf,unsigned char num);




#endif





