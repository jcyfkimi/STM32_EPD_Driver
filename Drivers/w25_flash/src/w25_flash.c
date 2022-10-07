/*
 * w25_flash.c
 *
 *  Created on: Oct 6, 2022
 *      Author: Kimi Jin
 */

#include "w25_flash.h"


// https://github.com/ZHJ0125/USB_SPI_Flash/blob/master/Code/usb-C8T6/User/bsp/bsp_spi_flash.c
uint8_t w25_flash_initialize(void)
{

	if (sFLASH_ID == w25_flash_read_ID())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void w25_flash_sector_erase(uint32_t sector_addr)
{
	w25_flash_write_enable();
	w25_flash_wait_for_write_end();

	w25_flash_send_byte(W25X_SECTORERASE);
	w25_flash_send_byte((sector_addr & 0xFF0000) >> 16);
	w25_flash_send_byte((sector_addr & 0xFF00) >> 8);
	w25_flash_send_byte(sector_addr & 0xFF);

	w25_flash_wait_for_write_end();
}

void w25_flash_bulk_erase(void)
{
	w25_flash_write_enable();

	w25_flash_send_byte(W25X_CHIPERASE);

	w25_flash_wait_for_write_end();
}

void w25_flash_page_write(uint8_t *pBuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
	w25_flash_write_enable();

	w25_flash_send_byte(W25X_PAGEPROGRAM);
	w25_flash_send_byte((write_addr & 0xFF0000) >> 16);
	w25_flash_send_byte((write_addr & 0xFF00) >> 8);
	w25_flash_send_byte(write_addr & 0xFF);

	if (num_byte_to_write > W25X_FLASH_PER_WRITE_PAGE_SIZE)
	{
		num_byte_to_write = W25X_FLASH_PER_WRITE_PAGE_SIZE;
		FLASH_ERROR("SPI_FLASH_PageWrite too large!");
	}

	while (num_byte_to_write--)
	{
		w25_flash_send_byte(*pBuffer);
		pBuffer++;
	}

	w25_flash_wait_for_write_end();
}

void w25_flash_buffer_write(uint8_t *pBuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
	uint8_t num_of_page = 0;
	uint8_t num_of_single = 0;
	uint8_t addr = 0;
	uint8_t count = 0;
	uint8_t temp = 0;

	addr = write_addr % W25X_FLASH_PAGE_SIZE;

	count = W25X_FLASH_PAGE_SIZE - addr;

	num_of_page = num_byte_to_write / W25X_FLASH_PAGE_SIZE;

	num_of_single = num_byte_to_write % W25X_FLASH_PAGE_SIZE;

	if (addr == 0)
	{
		if (num_of_page == 0)
		{
			w25_flash_page_write(pBuffer, write_addr, num_byte_to_write);
		}
		else
		{
			while (num_of_page--)
			{
				w25_flash_page_write(pBuffer, write_addr, W25X_FLASH_PAGE_SIZE);
				write_addr += W25X_FLASH_PAGE_SIZE;
				pBuffer += W25X_FLASH_PAGE_SIZE;
			}
			w25_flash_page_write(pBuffer, write_addr, num_of_single);
		}
	}
	else
	{
		if (num_of_page == 0)
		{
			if (num_of_single > count)
			{
				temp = num_of_single - count;
				w25_flash_page_write(pBuffer, write_addr, count);

				num_of_single += count;
				pBuffer += count;
				w25_flash_page_write(pBuffer, write_addr, temp);
			}
			else
			{
				w25_flash_page_write(pBuffer, write_addr, num_byte_to_write);
			}
		}
		else
		{
			num_byte_to_write -= count;
			num_of_page = num_byte_to_write / W25X_FLASH_PAGE_SIZE;
			num_of_single = num_byte_to_write % W25X_FLASH_PAGE_SIZE;

			w25_flash_page_write(pBuffer, write_addr, count);

			write_addr += count;
			pBuffer += count;
			while (num_of_page--)
			{
				w25_flash_page_write(pBuffer, write_addr, W25X_FLASH_PAGE_SIZE);
				write_addr += W25X_FLASH_PAGE_SIZE;
				pBuffer += W25X_FLASH_PAGE_SIZE;
			}
			if (num_of_single != 0)
			{
				w25_flash_page_write(pBuffer, write_addr, num_of_single);
			}
		}
	}
}

void w25_flash_buffer_read(uint8_t *pBuffer, uint32_t read_addr, uint16_t num_byte_to_read)
{
	w25_flash_send_byte(W25X_READDATA);

	w25_flash_send_byte((read_addr & 0xFF0000) >> 16);

	w25_flash_send_byte((read_addr & 0xFF00) >> 8);

	w25_flash_send_byte(read_addr & 0xFF);

	while (num_byte_to_read--) /* while there is data to be read */
	{

		*pBuffer = w25_flash_send_byte(DUMMY_BYTE);

		pBuffer++;
	}

}

uint32_t w25_flash_read_ID(void)
{
	uint32_t temp = 0;
	uint32_t temp0 = 0;
	uint32_t temp1 = 0;
	uint32_t temp2 = 0;

	w25_flash_send_byte(W25X_JEDECDEVICEID);

	temp0 = w25_flash_send_byte(DUMMY_BYTE);

	temp1 = w25_flash_send_byte(DUMMY_BYTE);

	temp2 = w25_flash_send_byte(DUMMY_BYTE);


	temp = (temp0 << 16) | (temp1 << 8) | temp2;

	return temp;
}

uint32_t w25_flash_read_device_ID(void)
{
	uint32_t temp = 0;

	/* Send "RDID " instruction */
	w25_flash_send_byte(W25X_DEVICEID);
	w25_flash_send_byte(DUMMY_BYTE);
	w25_flash_send_byte(DUMMY_BYTE);
	w25_flash_send_byte(DUMMY_BYTE);

	/* Read a byte from the FLASH */
	temp = w25_flash_send_byte(DUMMY_BYTE);

	return temp;
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
void w25_flash_start_read_sequence(uint32_t read_addr)
{
	/* Send "Read from Memory " instruction */
	w25_flash_send_byte(W25X_READDATA);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	w25_flash_send_byte((read_addr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	w25_flash_send_byte((read_addr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	w25_flash_send_byte(read_addr & 0xFF);
}

uint8_t w25_flash_read_byte(void)
{
	return (w25_flash_send_byte(DUMMY_BYTE));
}

uint8_t w25_flash_send_byte(uint8_t byte)
{
	uint8_t rx_data = 0;

	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 2, SPIT_FLAG_TIMEOUT);

	return rx_data;
}

uint16_t w25_flash_send_half_word(uint16_t half_word)
{
	uint16_t rx_data = 0;

	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&half_word, (uint8_t *)&rx_data, 4, SPIT_FLAG_TIMEOUT);

	return rx_data;
}

void w25_flash_write_enable(void)
{
	w25_flash_send_byte(W25X_WRITEENABLE);
}

void w25_flash_wait_for_write_end(void)
{
	uint8_t flash_status = 0;

	w25_flash_send_byte(W25X_ReEADSTATUSREG);

	do
	{
		flash_status = w25_flash_send_byte(DUMMY_BYTE);
	} while ((flash_status & WIP_FLAG) == SET);

}

void w25_flash_power_down(void)
{
	w25_flash_send_byte(W25X_POWERDOWN);
}

void w25_flash_wake_up(void)
{
	w25_flash_send_byte(W25X_RELEASEPOWERDOWN);
}

void w25_flash_write_no_check(uint8_t *pBuffer,uint32_t write_addr,uint16_t num_byte_to_write)
{
	uint16_t pageremain;
	pageremain = 256 - write_addr % 256;
	if (num_byte_to_write <= pageremain)
	{
		pageremain = num_byte_to_write;
	}
	while (1)
	{
		w25_flash_page_write(pBuffer, write_addr, pageremain);
		if (num_byte_to_write == pageremain)
		{
			break;
		}
		else //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			write_addr += pageremain;

			num_byte_to_write -= pageremain;
			if (num_byte_to_write > 256)
			{
				pageremain = 256;
			}
			else
			{
				pageremain = num_byte_to_write;
			}
		}
	};
}

uint8_t W25QXX_BUFFER[4096];
void w25_flash_write(uint8_t *pBuffer,uint32_t write_addr,uint16_t num_byte_to_write)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *w25qxx_buf;
	w25qxx_buf = W25QXX_BUFFER;
	secpos = write_addr / 4096;
	secoff = write_addr % 4096;
	secremain = 4096 - secoff;
	if (num_byte_to_write <= secremain)
	{
		secremain = num_byte_to_write;
	}
	while (1)
	{
		w25_flash_buffer_read(w25qxx_buf, secpos * 4096, 4096);
		for (i = 0; i < secremain; i++)
		{
			if (w25qxx_buf[secoff + i] != 0XFF)
			{
				break;
			}
		}
		if (i < secremain)
		{
			w25_flash_sector_erase(secpos << 12);
			for (i = 0; i < secremain; i++)
			{
				w25qxx_buf[i + secoff] = pBuffer[i];
			}
			w25_flash_write_no_check(w25qxx_buf, secpos * 4096, 4096);

		}
		else
		{
			w25_flash_write_no_check(pBuffer, write_addr, secremain);
		}
		if (num_byte_to_write == secremain)
		{
			break;
		}
		else
		{
			secpos++;
			secoff = 0;
			pBuffer += secremain;
			write_addr += secremain;
			num_byte_to_write -= secremain;
			if (num_byte_to_write > 4096)
			{
				secremain = 4096;
			}
			else
			{
				secremain = num_byte_to_write;
			}
		}
	};
}
