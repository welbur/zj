

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
   u8 i=10; //��������Ż��ٶ� ����������͵�5����д�� 
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
	return 0; //SDA��Ϊ�͵�ƽ������æ,�˳� 
SDA_L; 
I2C_delay(); 
if(SDA_read)
	return 0; //SDA��Ϊ�ߵ�ƽ�����߳���,�˳� 
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

unsigned char I2C_WaitAck(void)   //����Ϊ:=1��ACK,=0��ACK 
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

void I2C_SendByte( char SendByte) //���ݴӸ�λ����λ// 
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

unsigned char I2C_ReceiveByte(void)  //���ݴӸ�λ����λ// 
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
	
	I2C_SendByte(DevAddr);//��������д��ַ
	if (!I2C_WaitAck()) 
	{ 
	I2C_Stop();  
	return 0; 
	} 
	
	I2C_SendByte(RegAddr);   //��������      
	I2C_WaitAck(); 
		 
	I2C_SendByte(Char); 
	I2C_WaitAck(); 
  
	I2C_Stop(); 
		//ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms) 
		//Systick_Delay_1ms(10); 
	I2C_delay(); 
	I2C_delay();
	I2C_delay();
	I2C_delay();
	I2C_delay();
	I2C_delay();
	return 1; 
} 


//����1������          
unsigned char I2C_Read_Byte(unsigned char DevAddr, unsigned char RegAddr,  char *Char) 
{ 

	if (!I2C_Start())
	{
//		I2C_Stop(); 
		return 0; 
	}

   I2C_SendByte(DevAddr);//��������д��ַ,��д��Ҫ���ĵ�ַ 
   if (!I2C_WaitAck())  
		{ 
		I2C_Stop();  
		return 0; 
		} 

    I2C_SendByte(RegAddr);   //���õ���ʼ��ַ       
    I2C_WaitAck(); 
		
    I2C_Start(); 
    I2C_SendByte(DevAddr + 1); 
    I2C_WaitAck(); 
    *Char = I2C_ReceiveByte(); 
    I2C_NoAck(); 

    I2C_Stop(); 
    return 1; 
} 

//����1������          
unsigned char I2C_Read_Str(unsigned char DevAddr, unsigned char RegAddr, unsigned char *Buf,unsigned int Len) 
{ 

	if (!I2C_Start())
		return 0; 

   I2C_SendByte(DevAddr);//��������д��ַ,��д��Ҫ���ĵ�ַ 
   if (!I2C_WaitAck())  
		{ 
		I2C_Stop();  
		return 0; 
		} 

    I2C_SendByte(RegAddr);   //���õ���ʼ��ַ       
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

////����IIC��ʼ�ź�
//void IIC_Start(void)
//{
//	SDA_OUT();     //sda�����
//	IIC_SDA=1;	  	  
//	IIC_SCL=1;
//	delay_us(4);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(4);
//	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
//}	  
////����IICֹͣ�ź�
//void IIC_Stop(void)
//{
//	SDA_OUT();//sda�����
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//����I2C���߽����ź�
//	delay_us(4);							   	
//}
////�ȴ�Ӧ���źŵ���
////����ֵ��1������Ӧ��ʧ��
////        0������Ӧ��ɹ�
//u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	SDA_IN();      //SDA����Ϊ����  
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
//	IIC_SCL=0;//ʱ�����0 	   
//	return 0;  
//} 
////����ACKӦ��
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
////������ACKӦ��		    
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
////IIC����һ���ֽ�
////���شӻ�����Ӧ��
////1����Ӧ��
////0����Ӧ��			  
//void IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
//	SDA_OUT(); 	    
//    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
//    for(t=0;t<8;t++)
//    {              
//        IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 	  
//		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
//		IIC_SCL=1;
//		delay_us(2); 
//		IIC_SCL=0;	
//		delay_us(2);
//    }	 
//} 	    
////��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
//u8 IIC_Read_Byte(unsigned char ack)
//{
//	unsigned char i,receive=0;
//	SDA_IN();//SDA����Ϊ����
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
//        IIC_NAck();//����nACK
//    else
//        IIC_Ack(); //����ACK   
//    return receive;
//}



