/*
 * w25_flash.c
 *
 *  Created on: Oct 6, 2022
 *      Author: Kimi Jin
 */

#include "w25_flash.h"


// https://github.com/ZHJ0125/USB_SPI_Flash/blob/master/Code/usb-C8T6/User/bsp/bsp_spi_flash.c
static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

uint8_t FLASH_SPI_disk_initialize(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK, ENABLE);

	FLASH_SPI_CS_APBxClock_FUN(
			FLASH_SPI_CS_CLK | FLASH_SPI_SCK_CLK | FLASH_SPI_MISO_PIN
					| FLASH_SPI_MOSI_PIN, ENABLE);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

	SPI_FLASH_CS_HIGH();

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(FLASH_SPIx, &SPI_InitStructure);

	SPI_Cmd(FLASH_SPIx, ENABLE);

	if (sFLASH_ID == SPI_FLASH_ReadID())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void SPI_FLASH_SectorErase(u32 SectorAddr)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();

	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_SectorErase);
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	SPI_FLASH_CS_HIGH();
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BulkErase(void)
{
	SPI_FLASH_WriteEnable();

	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_ChipErase);
	SPI_FLASH_CS_HIGH();

	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_PageWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	SPI_FLASH_WriteEnable();

	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_PageProgram);
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	SPI_FLASH_SendByte(WriteAddr & 0xFF);

	if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		FLASH_ERROR("SPI_FLASH_PageWrite too large!");
	}

	while (NumByteToWrite--)
	{
		SPI_FLASH_SendByte(*pBuffer);
		pBuffer++;
	}

	SPI_FLASH_CS_HIGH();

	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BufferWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % SPI_FLASH_PageSize;

	count = SPI_FLASH_PageSize - Addr;

	NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;

	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	if (Addr == 0)
	{
		if (NumOfPage == 0)
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else
		{
			while (NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else
	{
		if (NumOfPage == 0)
		{
			if (NumOfSingle > count)
			{
				temp = NumOfSingle - count;
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

				WriteAddr += count;
				pBuffer += count;
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else
		{
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

			WriteAddr += count;
			pBuffer += count;
			while (NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			if (NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

void SPI_FLASH_BufferRead(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	SPI_FLASH_CS_LOW();

	SPI_FLASH_SendByte(W25X_ReadData);

	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);

	SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);

	SPI_FLASH_SendByte(ReadAddr & 0xFF);

	while (NumByteToRead--) /* while there is data to be read */
	{

		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);

		pBuffer++;
	}

	SPI_FLASH_CS_HIGH();
}

u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	SPI_FLASH_CS_LOW();

	SPI_FLASH_SendByte(W25X_JedecDeviceID);

	Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

	Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

	Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

	SPI_FLASH_CS_HIGH();

	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

u32 SPI_FLASH_ReadDeviceID(void)
{
	u32 Temp = 0;

	/* Select the FLASH: Chip Select low */
	SPI_FLASH_CS_LOW();

	/* Send "RDID " instruction */
	SPI_FLASH_SendByte(W25X_DeviceID);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp = SPI_FLASH_SendByte(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	SPI_FLASH_CS_HIGH();

	return Temp;
}
/*******************************************************************************
 * Function Name  : SPI_FLASH_StartReadSequence
 * Description    : Initiates a read data byte (READ) sequence from the Flash.
 *                  This is done by driving the /CS line low to select the device,
 *                  then the READ instruction is transmitted followed by 3 bytes
 *                  address. This function exit and keep the /CS line low, so the
 *                  Flash still being selected. With this technique the whole
 *                  content of the Flash is read with a single READ instruction.
 * Input          : - ReadAddr : FLASH's internal address to read from.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
	/* Select the FLASH: Chip Select low */
	SPI_FLASH_CS_LOW();

	/* Send "Read from Memory " instruction */
	SPI_FLASH_SendByte(W25X_ReadData);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

u8 SPI_FLASH_ReadByte(void)
{
	return (SPI_FLASH_SendByte(Dummy_Byte));
}

u8 SPI_FLASH_SendByte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		if ((SPITimeout--) == 0)
			return SPI_TIMEOUT_UserCallback(0);
	}

	SPI_I2S_SendData(FLASH_SPIx, byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if ((SPITimeout--) == 0)
			return SPI_TIMEOUT_UserCallback(1);
	}

	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;

	while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		if ((SPITimeout--) == 0)
			return SPI_TIMEOUT_UserCallback(2);
	}

	SPI_I2S_SendData(FLASH_SPIx, HalfWord);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if ((SPITimeout--) == 0)
			return SPI_TIMEOUT_UserCallback(3);
	}
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

void SPI_FLASH_WriteEnable(void)
{

	SPI_FLASH_CS_LOW();

	SPI_FLASH_SendByte(W25X_WriteEnable);

	SPI_FLASH_CS_HIGH();
}

#define WIP_Flag                  0x01

void SPI_FLASH_WaitForWriteEnd(void)
{
	u8 FLASH_Status = 0;

	SPI_FLASH_CS_LOW();

	SPI_FLASH_SendByte(W25X_ReadStatusReg);

	do
	{

		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	} while ((FLASH_Status & WIP_Flag) == SET);

	SPI_FLASH_CS_HIGH();
}

void SPI_Flash_PowerDown(void)
{

	SPI_FLASH_CS_LOW();

	SPI_FLASH_SendByte(W25X_PowerDown);

	SPI_FLASH_CS_HIGH();
}

void SPI_Flash_WAKEUP(void)
{

	SPI_FLASH_CS_LOW();

	SPI_FLASH_SendByte(W25X_ReleasePowerDown);

	SPI_FLASH_CS_HIGH();
}

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	FLASH_ERROR("SPI!errorCode = %d", errorCode);
	return 0;
}

void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr % 256;
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite;
	while (1)
	{
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break;
		else //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain;
			if (NumByteToWrite > 256)
				pageremain = 256;
			else
				pageremain = NumByteToWrite;
		}
	};
}

u8 W25QXX_BUFFER[4096];
void W25QXX_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;
	u8 *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;
	secpos = WriteAddr / 4096;
	secoff = WriteAddr % 4096;
	secremain = 4096 - secoff;
	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite;
	while (1)
	{
		SPI_FLASH_BufferRead(W25QXX_BUF, secpos * 4096, 4096);
		for (i = 0; i < secremain; i++)
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break;
		}
		if (i < secremain)
		{
			SPI_FLASH_SectorErase(secpos << 12);
			for (i = 0; i < secremain; i++)
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);

		}
		else
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
		if (NumByteToWrite == secremain)
			break;
		else
		{
			secpos++;
			secoff = 0;

			pBuffer += secremain;
			WriteAddr += secremain;
			NumByteToWrite -= secremain;
			if (NumByteToWrite > 4096)
				secremain = 4096;
			else
				secremain = NumByteToWrite;
		}
	};
}
