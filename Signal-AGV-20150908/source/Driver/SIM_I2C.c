

#include "SIM_I2C.h"


void SIM_I2C_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(SDA_CLK | SCL_CLK, ENABLE);
 
	/* Configure I2C1 pins: SCL and SDA  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_Pin = SCL_PIN;  
  GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;  
  GPIO_Init(SDA_PORT, &GPIO_InitStructure);
	
	SCL_L;
	SDA_L;
	I2C_delay();
	I2C_delay();
	SCL_H;
	SDA_H;
	I2C_delay();
	I2C_delay();
 
//	I2C_Config();
}
/**/ 
void I2C_delay(void) 
{ 
   u8 i=10; //这里可以优化速度 ，经测试最低到5还能写入 
   while(i)  
   {  
     i--;  
   }  
} 

unsigned char I2C_Start(void) 
{ 
SDA_H; 
SCL_H; 
I2C_delay(); 
if(!SDA_read)
	return 0; //SDA线为低电平则总线忙,退出 
SDA_L; 
I2C_delay(); 
if(SDA_read)
	return 0; //SDA线为高电平则总线出错,退出 
SDA_L; 
I2C_delay(); 
return 1; 
} 

void I2C_Stop(void) 
{ 
SCL_L; 
I2C_delay(); 
SDA_L; 
I2C_delay(); 
SCL_H; 
I2C_delay(); 
SDA_H; 
I2C_delay(); 
} 

void I2C_Ack(void) 
{ 
SCL_L; 
I2C_delay(); 
SDA_L; 
I2C_delay(); 
SCL_H; 
I2C_delay(); 
SCL_L; 
I2C_delay(); 
} 

void I2C_NoAck(void) 
{ 
SCL_L; 
I2C_delay(); 
SDA_H; 
I2C_delay(); 
SCL_H; 
I2C_delay(); 
SCL_L; 
I2C_delay(); 
} 

unsigned char I2C_WaitAck(void)   //返回为:=1有ACK,=0无ACK 
{ 
SCL_L; 
I2C_delay(); 
SDA_H; 
I2C_delay(); 
SCL_H; 
I2C_delay(); 
if(SDA_read) 
{ 
      SCL_L; 
      return 0; 
} 
SCL_L; 
return 1; 
} 

void I2C_SendByte( char SendByte) //数据从高位到低位// 
{ 
    u8 i=8; 
    while(i--) 
    { 
        SCL_L; 
        I2C_delay(); 
      if(SendByte&0x80) 
        SDA_H;   
      else  
        SDA_L;    
        SendByte<<=1; 
        I2C_delay(); 
				SCL_H; 
        I2C_delay(); 
    } 
    SCL_L; 
} 

unsigned char I2C_ReceiveByte(void)  //数据从高位到低位// 
{  
    u8 i=8; 
    u8 ReceiveByte=0; 

    SDA_H; 
    while(i--) 
    { 
      ReceiveByte<<=1;       
      SCL_L; 
      I2C_delay(); 
			SCL_H; 
      I2C_delay(); 
      if(SDA_read) 
      { 
        ReceiveByte|=0x01; 
      } 
    } 
    SCL_L; 
    return ReceiveByte; 
} 


unsigned char I2C_Write_Byte(unsigned char DevAddr, unsigned char RegAddr,  char Char) 
{ 

	if (!I2C_Start()) return 0; 
	
	I2C_SendByte(DevAddr);//设置器件写地址
	if (!I2C_WaitAck()) 
	{ 
	I2C_Stop();  
	return 0; 
	} 
	
	I2C_SendByte(RegAddr);   //设置器件      
	I2C_WaitAck(); 
		 
	I2C_SendByte(Char); 
	I2C_WaitAck(); 
  
	I2C_Stop(); 
		//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms) 
		//Systick_Delay_1ms(10); 
	I2C_delay(); 
	I2C_delay();
	I2C_delay();
	I2C_delay();
	I2C_delay();
	I2C_delay();
	return 1; 
} 


//读出1串数据          
unsigned char I2C_Read_Byte(unsigned char DevAddr, unsigned char RegAddr,  char *Char) 
{ 

	if (!I2C_Start())
	{
//		I2C_Stop(); 
		return 0; 
	}

   I2C_SendByte(DevAddr);//设置器件写地址,先写入要读的地址 
   if (!I2C_WaitAck())  
		{ 
		I2C_Stop();  
		return 0; 
		} 

    I2C_SendByte(RegAddr);   //设置低起始地址       
    I2C_WaitAck(); 
		
    I2C_Start(); 
    I2C_SendByte(DevAddr + 1); 
    I2C_WaitAck(); 
    *Char = I2C_ReceiveByte(); 
    I2C_NoAck(); 

    I2C_Stop(); 
    return 1; 
} 

//读出1串数据          
unsigned char I2C_Read_Str(unsigned char DevAddr, unsigned char RegAddr, unsigned char *Buf,unsigned int Len) 
{ 

	if (!I2C_Start())
		return 0; 

   I2C_SendByte(DevAddr);//设置器件写地址,先写入要读的地址 
   if (!I2C_WaitAck())  
		{ 
		I2C_Stop();  
		return 0; 
		} 

    I2C_SendByte(RegAddr);   //设置低起始地址       
    I2C_WaitAck(); 
		
    I2C_Start(); 
    I2C_SendByte(DevAddr + 1); 
    I2C_WaitAck(); 
		while(Len--)
    *Buf++ = I2C_ReceiveByte(); 
    I2C_NoAck(); 

    I2C_Stop(); 
    return 1; 
} 

////产生IIC起始信号
//void IIC_Start(void)
//{
//	SDA_OUT();     //sda线输出
//	IIC_SDA=1;	  	  
//	IIC_SCL=1;
//	delay_us(4);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(4);
//	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
//}	  
////产生IIC停止信号
//void IIC_Stop(void)
//{
//	SDA_OUT();//sda线输出
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(4);							   	
//}
////等待应答信号到来
////返回值：1，接收应答失败
////        0，接收应答成功
//u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	SDA_IN();      //SDA设置为输入  
//	IIC_SDA=1;delay_us(1);	   
//	IIC_SCL=1;delay_us(1);	 
//	while(READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			IIC_Stop();
//			return 1;
//		}
//	}
//	IIC_SCL=0;//时钟输出0 	   
//	return 0;  
//} 
////产生ACK应答
//void IIC_Ack(void)
//{
//	IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=0;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//}
////不产生ACK应答		    
//void IIC_NAck(void)
//{
//	IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//}					 				     
////IIC发送一个字节
////返回从机有无应答
////1，有应答
////0，无应答			  
//void IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
//	SDA_OUT(); 	    
//    IIC_SCL=0;//拉低时钟开始数据传输
//    for(t=0;t<8;t++)
//    {              
//        IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 	  
//		delay_us(2);   //对TEA5767这三个延时都是必须的
//		IIC_SCL=1;
//		delay_us(2); 
//		IIC_SCL=0;	
//		delay_us(2);
//    }	 
//} 	    
////读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
//u8 IIC_Read_Byte(unsigned char ack)
//{
//	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
//    for(i=0;i<8;i++ )
//	{
//        IIC_SCL=0; 
//        delay_us(2);
//		IIC_SCL=1;
//        receive<<=1;
//        if(READ_SDA)receive++;   
//		delay_us(1); 
//    }					 
//    if (!ack)
//        IIC_NAck();//发送nACK
//    else
//        IIC_Ack(); //发送ACK   
//    return receive;
//}



