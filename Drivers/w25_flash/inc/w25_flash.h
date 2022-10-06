/*
 * w25_flash.h
 *
 *  Created on: Oct 6, 2022
 *      Author: Kimi Jin
 */

#ifndef _W25_FLASH_INC_W25_FLASH_H_
#define _W25_FLASH_INC_W25_FLASH_H_
#include "main.h"
#include <stdio.h>

// https://github.com/ZHJ0125/USB_SPI_Flash/blob/master/Code/usb-C8T6/User/bsp/bsp_spi_flash.h

//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4018   //W25Q128
//#define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256


#define W25X_WriteEnable		      0x06
#define W25X_WriteDisable		      0x04
#define W25X_ReadStatusReg		    0x05
#define W25X_WriteStatusReg		    0x01
#define W25X_ReadData			        0x03
#define W25X_FastReadData		      0x0B
#define W25X_FastReadDual		      0x3B
#define W25X_PageProgram		      0x02
#define W25X_BlockErase			      0xD8
#define W25X_SectorErase		      0x20
#define W25X_ChipErase			      0xC7
#define W25X_PowerDown			      0xB9
#define W25X_ReleasePowerDown	    0xAB
#define W25X_DeviceID			        0xAB
#define W25X_ManufactDeviceID   	0x90
#define W25X_JedecDeviceID		    0x9F

#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF




#define      FLASH_SPIx                        SPI1
#define      FLASH_SPI_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB2Periph_SPI1


#define      FLASH_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOA
#define      FLASH_SPI_CS_PORT                 GPIOA
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_4

#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA
#define      FLASH_SPI_SCK_PORT                GPIOA
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_5

#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOA
#define      FLASH_SPI_MISO_PORT               GPIOA
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_6

#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA
#define      FLASH_SPI_MOSI_PORT               GPIOA
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_7

#define  		SPI_FLASH_CS_LOW()     						GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    						GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )


#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


#define FLASH_DEBUG_ON         1
#define FLASH_DEBUG_FUNC_ON    0

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define FLASH_DEBUG_FUNC()               do{\
                                         if(FLASH_DEBUG_FUNC_ON)\
                                         printf("<<-FLASH-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

uint8_t FLASH_SPI_disk_initialize(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) ;
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   ;

#endif /* _W25_FLASH_INC_W25_FLASH_H_ */
