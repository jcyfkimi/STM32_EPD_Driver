/*
 * epd_3in71_fpc2302.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Kimi Jin
 */
#include "gpio.h"
#include "epd_3in71_fpc2302.h"

static void epd_3in71_fpc2302_spi_write(unsigned char value)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		EPD_CLK_0;
		if (value & 0x80)
		{
			EPD_MOSI_1;
		}
		else
		{
			EPD_MOSI_0;
		}
		value = (value << 1);
		EPD_CLK_1;
	}

}

static void epd_3in71_fpc2302_spi_write_cmd(unsigned char cmd)
{
	EPD_CS_0;
	EPD_DC_0;		// command write
	epd_3in71_fpc2302_spi_write(cmd);
	EPD_CS_1;

}

static void epd_3in71_fpc2302_write_data(unsigned char data)
{
	EPD_CS_0;
	EPD_DC_1;		// data write
	epd_3in71_fpc2302_spi_write(data);
	EPD_CS_1;

}


static void epd_3in71_fpc2302_reset(void)
{
	EPD_RST_0;
	HAL_Delay(20);
	EPD_RST_1;
	HAL_Delay(20);
}


static void epd_3in71_fpc2302_read_busy(void)
{
	unsigned char busy;
	do
	{
		HAL_Delay(100);
		epd_3in71_fpc2302_spi_write_cmd(0x71);
		busy = isEPD_BUSY;
		busy = !(busy & 0x01);
	} while (busy);
	HAL_Delay(200);
}



void epd_3in71_fpc2302_init(void)
{
	epd_3in71_fpc2302_reset(); //Electronic paper IC reset

	epd_3in71_fpc2302_spi_write_cmd(0x04);  //Power on
	HAL_Delay(5);
	epd_3in71_fpc2302_read_busy(); //waiting for the electronic paper IC to release the idle signal
}

static void epd_3in71_fpc2302_turn_on_display(void)
{
	epd_3in71_fpc2302_spi_write_cmd(0x12);
	HAL_Delay(1);
	epd_3in71_fpc2302_read_busy();
}

void epd_3in71_fpc2302_clear(void)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_3IN71_FPC2302_WIDTH % 8 == 0) ? (EPD_3IN71_FPC2302_WIDTH / 8) : (EPD_3IN71_FPC2302_WIDTH / 8 + 1);
	height = EPD_3IN71_FPC2302_HEIGHT;

	epd_3in71_fpc2302_spi_write_cmd(0x10);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			epd_3in71_fpc2302_write_data(0xff);
		}
	}

	epd_3in71_fpc2302_spi_write_cmd(0x13);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_3in71_fpc2302_write_data(0x00);
		}
	}

	epd_3in71_fpc2302_turn_on_display();
}


void epd_3in71_fpc2302_display(unsigned char *img_black, unsigned char *img_red)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_3IN71_FPC2302_WIDTH % 8 == 0) ? (EPD_3IN71_FPC2302_WIDTH / 8) : (EPD_3IN71_FPC2302_WIDTH / 8 + 1);
	height = EPD_3IN71_FPC2302_HEIGHT;

	epd_3in71_fpc2302_spi_write_cmd(0x10);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			epd_3in71_fpc2302_write_data(img_black[j + i * width]);
		}
	}

	epd_3in71_fpc2302_spi_write_cmd(0x13);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_3in71_fpc2302_write_data(~img_red[j + i * width]);
		}
	}

	epd_3in71_fpc2302_turn_on_display();
}


void epd_3in71_fpc2302_sleep(void)
{
	epd_3in71_fpc2302_spi_write_cmd(0X50);  //VCOM AND DATA INTERVAL SETTING
	epd_3in71_fpc2302_write_data(0xf7); //WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

	epd_3in71_fpc2302_spi_write_cmd(0X02);  	//power off
	epd_3in71_fpc2302_read_busy(); //waiting for the electronic paper IC to release the idle signal
	epd_3in71_fpc2302_spi_write_cmd(0X07);  	//deep sleep
	epd_3in71_fpc2302_write_data(0xA5);
}


