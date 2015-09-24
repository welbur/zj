
#include "Driver_Include.h"
#include "W25X16.h"

  
/***************************************************************************
    - 功能描述：w25x16 Flash的初始化函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 - 说明：w25x16 Flash作为从机，由 stm32进行读写操作
    - 每256bytes为一个Page，每4Kbytes为一个Sector，每16个扇区为1个Block
 - W25X16容量为2M字节,共有32个Block, 512个Sector  , 8192个Page
 - W25X32容量为4M字节,共有64个Block, 1024个Sector ,16384个Page
 - W25X64容量为8M字节,共有128个Block,2048个Sector ,32768个Page
***************************************************************************/
void SPI_Flash_Init(void)
{

  SPI2_Init();     //初始化SPI
}  

/***************************************************************************
    - 功能描述：读w25x16 Flash状态寄存器的函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：返回w25x16 Flash的当前状态
 ------------------------------------------------------
 |  7   6   5   4   3   2   1   0      |
 | SPR   RV  TB BP2 BP1 BP0 WEL BUSY     |
 | SPR:默认0,状态寄存器保护位,配合WP使用    |
 | TB,BP2,BP1,BP0:FLASH区域写保护设置     |
 | WEL:写使能锁定          |
 | BUSY:忙标记位(1,忙;0,空闲)       |
 | 默认:0x00           |
 | 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!  |
 -----------------------------------------------------
 - 操作说明：CS电平先拉低、STM32利用SPI发送读FLASH状态寄存器命令、STM32读取数据、CS拉高，发送的命令被采集
    如果CS不拉高，则FLASH的状态会不断发送出来。根据读出的字节就可以判断当前的FLASH
    的工作准状态。
***************************************************************************/
u8 SPI_Flash_ReadSR(void)   
{  
 u8 byte=0;   
 W25X16_CS_Low;                            //使能器件   
 W25X16_SendByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
 byte=W25X16_SendByte(0Xff);             //读取一个字节  
 W25X16_CS_High;                            //取消片选     
 return byte;   
} 
/***************************************************************************
    - 功能描述：写w25x16 Flash状态寄存器的函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 -----------------------------------------------------
 |  7   6   5   4   3   2   1   0      |
 | SPR   RV  TB BP2 BP1 BP0 WEL BUSY     |
 | SPR:默认0,状态寄存器保护位,配合WP使用    |
 | TB,BP2,BP1,BP0:FLASH区域写保护设置     |
 | WEL:写使能锁定          |
 | BUSY:忙标记位(1,忙;0,空闲)       |
 | 默认:0x00           |
 | 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!  |
 -----------------------------------------------------
 - 操作说明：CS电平先拉低、STM32利用SPI发送写状态寄存器命令、STM32再继续发送要写寄存器的数据、CS拉高
    指令结束，如果指令拉晚了，值不会被写入，指令无效！
***************************************************************************/
void SPI_FLASH_Write_SR(u8 sr)   
{   
 W25X16_CS_Low;                            //使能器件   
 W25X16_SendByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
 W25X16_SendByte(sr);               //写入一个字节  
 W25X16_CS_High;                            //取消片选            
}   
/***************************************************************************
    - 功能描述：w25x16 Flash写使能函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 -----------------------------------------------------
 |  7   6   5   4   3   2   1   0      |
 | SPR   RV  TB BP2 BP1 BP0 WEL BUSY     |
 | SPR:默认0,状态寄存器保护位,配合WP使用    |
 | TB,BP2,BP1,BP0:FLASH区域写保护设置     |
 | WEL:写使能锁定          |
 | BUSY:忙标记位(1,忙;0,空闲)       |
 | 默认:0x00           |
 | 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!  |
 -----------------------------------------------------
    - 函数说明：将WEL置位
 - 操作说明：CS电平先拉低、STM32发送写使能命令、CS电平拉高
***************************************************************************/
void SPI_FLASH_Write_Enable(void)   
{
 W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_WriteEnable);      //发送写使能  
 W25X16_CS_High;                            //取消片选            
} 
/***************************************************************************
    - 功能描述：w25x16 Flash写禁能函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
    - 函数说明：将WEL复位
 - 操作说明：CS电平先拉低、STM32发送写禁能命令、CS电平拉高
***************************************************************************/
void SPI_FLASH_Write_Disable(void)   
{  
 W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_WriteDisable);     //发送写禁止指令    
 W25X16_CS_High;                            //取消片选            
}        
/***************************************************************************
    - 功能描述：读w25x16 Flash ID函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：w25x16 Flash ID
 - 操作说明：CS电平先拉低、STM32发送制造/器件命令、然后发送三个字节0X00的24位地址（其中器件ID为8位），
    0x000000发完之后，FLASH就会返回器件的制造/器件号，此时STM32发送0xff，用于提供时钟信号，
    从而读取器件号。如果发送的是0x000001,则先返回8位器件号。
***************************************************************************/
u16 SPI_Flash_ReadID(void)
{
 u16 Temp = 0;   
 W25X16_CS_Low;        
 W25X16_SendByte(0x90);//发送读取ID命令     
 W25X16_SendByte(0x00);      
 W25X16_SendByte(0x00);      
 W25X16_SendByte(0x00);         
 Temp|=W25X16_SendByte(0xFF)<<8;  
 Temp|=W25X16_SendByte(0xFF);  
 W25X16_CS_High;        
 return Temp;
}   
/***************************************************************************
    - 功能描述：读w25x16 Flash操作函数，在指定地址开始读取指定长度的数据
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：pBuffer:数据存储区
    ReadAddr:开始读取的地址(24bit)
    NumByteToRead:要读取的字节数(最大65535)
  - 返回说明：无
 - 操作说明：CS电平先拉低选中、然后发送三个字节24位地址,依次A23~A16、A15~A8、A7~A0 ，通过(u8)强制
    类型转换，每次取走字节的低八位，依次将24位地址按顺序送出。发送完成以后就开始，读取数据，
    STM32通过向FLASH发送0XFF提供时钟，从而读取数据存储到数据存储区中,最后将片选CS拉低，停止度数据。
***************************************************************************/
void SPI_Flash_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead)   
{ 
  u16 i;                    
 W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_ReadData);         //发送读取命令W25X_ReadData=0x03   
    W25X16_SendByte((u8)((ReadAddr)>>16));  //发送24bit地址，u8取走的是低字节数据；    
    W25X16_SendByte((u8)((ReadAddr)>>8));   
    W25X16_SendByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
 { 
        pBuffer[i]=W25X16_SendByte(0XFF);   //循环读数  
    }
 W25X16_CS_High;                            //取消片选            
}   
/***************************************************************************
    - 功能描述：页写w25x16 Flash操作函数，SPI在一页(0~65535)内写入少于256个字节的数据
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：pBuffer:数据存储区
    WriteAddr:开始写的地址(24bit)
    NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!! 
  - 返回说明：无
 - 操作说明：
  （1）先发送写使能指令
  （2）CS拉低，选中芯片。
  （3）发送页写指令0x02
  （4）发送24位地址
  （5）发送待写的数据字节，注意每一页最大为256字节
  （6）写完数据CS拉为低电平
  （7）在指令执行过程中，状态寄存器BUSY为忙，当执行完成，BUSY为0；所以最后要加入等待BUSY，就是等待写入结束
***************************************************************************/ 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
  u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
 W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_PageProgram);      //发送写页命令   
    W25X16_SendByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    W25X16_SendByte((u8)((WriteAddr)>>8));   
    W25X16_SendByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)W25X16_SendByte(pBuffer[i]);//循环写数  
 W25X16_CS_High;                            //取消片选 
 SPI_Flash_Wait_Busy();        //等待写入结束
} 

/*************************************************************************************
    - 功能描述：连续页写SPI FLASH w25x16 Flash操作函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：pBuffer:数据存储区
    WriteAddr:开始写的地址(24bit)
    NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
 - 注意事项：（1）必须确保所写的地址范围内的数据全部在0XFF以内,否则在非0XFF处写入的数据将失败!
    （2）具有自动换页功能 
    （3）在指定地址开始写入指定长度的数据,但是要确保地址不越界! 
  - 返回说明：无
 - 操作说明：（1）先计算FLASH中当前页中剩下的可写字节数，由于FALSH的地址编址按24位进行编址，每256字节为一页，所以
      根据地址可以计算出一页中剩余的可写字节数。
    （2）如果剩余的字节数大于等于待写入的字节数，则表示能够写下，从而进行赋值操作。
       （3）进入循环，先按页剩余字节数进行页写操作。
    （4）再判断剩余字节数和待写字节数是否相等，如果是相等，退出循环。
    （5）如果按剩余字节数进行页写，页写完了以后，表示此页已经写完，可是待写的字节数还没写完，则进行以下处理
    （6）进入下一页进行写操作  
***************************************************************************************/
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{        
 u16 pageremain;    
 pageremain=256-WriteAddr%256; //单页剩余的字节数，因为一页有256字节，24位地址是按字节增加的        
 if(NumByteToWrite<=pageremain)
  pageremain=NumByteToWrite;//不大于256个字节
 while(1)
 {    
  SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
  if(NumByteToWrite==pageremain)
   break;//写入结束了
   else //表示NumByteToWrite>pageremain，以下操作就是进行指针变换、地址变换、NumByteToWrite变换，以便进入下一页的操作 
  {
   pBuffer+=pageremain;
   WriteAddr+=pageremain; 

   NumByteToWrite-=pageremain;     //减去已经写入了的字节数
   if(NumByteToWrite>256)
    pageremain=256; //一次可以写入256个字节
   else 
    pageremain=NumByteToWrite;    //不够256个字节了
  }
 }     
}  
/***************************************************************************
    - 功能描述：写扇区w25x16 Flash操作函数，在指定地址开始写入指定长度的数据，该函数带擦除操作!
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：pBuffer:数据存储区
    WriteAddr:开始写的地址(24bit)
    NumByteToWrite:要写入的字节数(最大65535)  
  - 返回说明：无
 - 操作说明：
  - 每256bytes为一个Page，每4Kbytes为一个Sector，每16个扇区为1个Block
  - W25X16容量为2M字节,共有32个Block, 512个Sector  , 8192个Page
  - W25X32容量为4M字节,共有64个Block, 1024个Sector ,16384个Page
  - W25X64容量为8M字节,共有128个Block,2048个Sector ,32768个Page
***************************************************************************/       
void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
 u32 secpos;
 u16 secoff;
 u16 secremain;    
//  u16 i;    

 secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
 secoff=WriteAddr%4096;//在扇区内的偏移
 secremain=4096-secoff;//扇区剩余空间大小   

 if(NumByteToWrite<=secremain)
  secremain=NumByteToWrite;//不大于4096个字节
// while(1) 
 { 
   SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
   SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,NumByteToWrite);//写入整个扇区  

  }
   
}
// u8 SPI_FLASH_BUF[4096];
// void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
// { 
//  u32 secpos;
//  u16 secoff;
//  u16 secremain;    
//   u16 i;    

//  secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
//  secoff=WriteAddr%4096;//在扇区内的偏移
//  secremain=4096-secoff;//扇区剩余空间大小   

//  if(NumByteToWrite<=secremain)
//   secremain=NumByteToWrite;//不大于4096个字节
//  while(1) 
//  { 
//   SPI_Flash_Read(secpos*4096,SPI_FLASH_BUF,4096);//读出整个扇区的内容
//   for(i=0;i<secremain;i++)//校验数据
//   {
//    if(SPI_FLASH_BUF[secoff+i]!=0XFF)
//     break;//需要擦除     
//   }
//   if(i<secremain)//需要擦除
//   {
//    SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
//    for(i=0;i<secremain;i++)    //复制
//    {
//     SPI_FLASH_BUF[i+secoff]=pBuffer[i];   
//    }
//    SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  

//   }
//   else
//     SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.        
//   if(NumByteToWrite==secremain)
//    break;//写入结束了
//   else//写入未结束
//   {
//    secpos++;//扇区地址增1
//    secoff=0;//偏移位置为0   

//       pBuffer+=secremain;  //指针偏移
//    WriteAddr+=secremain;//写地址偏移    
//       NumByteToWrite-=secremain;    //字节数递减
//    if(NumByteToWrite>4096)
//     secremain=4096; //下一个扇区还是写不完
//    else 
//     secremain=NumByteToWrite;   //下一个扇区可以写完了
//   }  
//  };    
// }

/***************************************************************************
    - 功能描述：w25x16 Flash擦除整个芯片操作函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 - 操作说明：W25X16:25s W25X32:40s W25X64:40s 等待时间超长...  擦除完以后，所有字节数据为0XFF
  （1）因为要擦除FLASH芯片，所以要发送写使能命令
  （2）读状态寄存器状态，通过BUSY位判断指令是否写完。
  （3）片选拉低，选中FLASH
  （4）发送擦除芯片命令
  （5）片选拉高,指令发送完成
  （6）检查BUSY位，等待芯片擦除结束
***************************************************************************/
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
   W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_ChipErase);        //发送片擦除命令  
 W25X16_CS_High;                            //取消片选            
 SPI_Flash_Wait_Busy();          //等待芯片擦除结束
}   
/***************************************************************************
    - 功能描述：w25x16 Flash擦除一个扇区操作函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：Dst_Addr:扇区地址 0~511 for w25x16
  - 返回说明：无
 - 操作说明：擦除一个山区的最少时间:150msXFF
  - 每256bytes为一个Page，每4Kbytes为一个Sector，每16个扇区为1个Block
  - W25X16容量为2M字节,共有32个Block, 512个Sector  , 8192个Page
  - W25X32容量为4M字节,共有64个Block, 1024个Sector ,16384个Page
  - W25X64容量为8M字节,共有128个Block,2048个Sector ,32768个Page

  （1）发送写使能命令
  （2）等待发送完成，通过检测状态寄存器的最低位是否为0
  （3）CS拉低，选中芯片
  （4）发送擦除命令
  （5）CS拉高，取消片选
  （6）等待擦除结束，如果结束BUSY会为0
***************************************************************************/

void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
 Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL   
    SPI_Flash_Wait_Busy();   
   W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_SectorErase);      //发送扇区擦除指令 
    W25X16_SendByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    W25X16_SendByte((u8)((Dst_Addr)>>8));   
    W25X16_SendByte((u8)Dst_Addr);  
 W25X16_CS_High;                            //取消片选            
    SPI_Flash_Wait_Busy();          //等待擦除完成
}  
/***************************************************************************
    - 功能描述：w25x16 Flash等待空闲操作函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 - 操作说明：就是检查FLASH的最低位是否为1
***************************************************************************/
void SPI_Flash_Wait_Busy(void)   
{   
 while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
/***************************************************************************
    - 功能描述：w25x16 Flash掉电模式操作函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 - 操作说明：
  （1）CS拉低，选中芯片。
  （2）STM32发送W25X_PowerDown指令
  （3）CS拉高，取消芯片。
  （4）延时TPD个延时
***************************************************************************/ 
void SPI_Flash_PowerDown(void)   
{ 
 u16 i;
   W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_PowerDown);        //发送掉电命令  
 W25X16_CS_High;                            //取消片选            
  for(i =0; i< 20000;i++);                               //等待TPD  
}   
/***************************************************************************
    - 功能描述：w25x16 Flash唤醒操作函数
    - 隶属模块：w25x16 Flash操作
  - 函数属性：外部，使用户使用
  - 参数说明：无
  - 返回说明：无
 - 操作说明：
  （1）CS拉低，选中芯片。
  （2）STM32发送 W25X_ReleasePowerDown指令
  （3）CS拉高，取消芯片。
  （4）延时tres个延时
***************************************************************************/ 
void SPI_Flash_WAKEUP(void)   
{  
u16 i;
   W25X16_CS_Low;                            //使能器件   
    W25X16_SendByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
 W25X16_CS_High;                            //取消片选            
    for(i =0; i< 20000;i++);                                //等待TRES1
}   
