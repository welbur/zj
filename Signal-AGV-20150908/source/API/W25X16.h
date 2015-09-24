
#ifndef __W25X16_H
#define __W25X16_H       
   

//W25X16读写
#define FLASH_ID 0XEF00  

/*********************************************
 - W25X16的操作指令表，STM32通过向W25X16
  发送以下指令就可以对W25X16进行以下的操作
*********************************************/
#define W25X_WriteEnable  0x06 
#define W25X_WriteDisable  0x04 
#define W25X_ReadStatusReg  0x05 
#define W25X_WriteStatusReg  0x01 
#define W25X_ReadData   0x03 
#define W25X_FastReadData  0x0B 
#define W25X_FastReadDual  0x3B 
#define W25X_PageProgram  0x02 
#define W25X_BlockErase   0xD8 
#define W25X_SectorErase  0x20 
#define W25X_ChipErase   0xC7 
#define W25X_PowerDown   0xB9 
#define W25X_ReleasePowerDown 0xAB 
#define W25X_DeviceID   0xAB 
#define W25X_ManufactDeviceID 0x90 
#define W25X_JedecDeviceID  0x9F 

/*********************************************
 - STM32操作W25X16的操作函数
*********************************************/
void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);       //读取FLASH ID
u8  SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(u8 sr);   //写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void); //写保护
void SPI_Flash_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead);   //读取flash
void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);       //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);     //唤醒


#endif

