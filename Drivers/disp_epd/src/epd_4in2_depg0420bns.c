/*
 * epd_4in2_depg0420bns.c
 *
 *  Created on: Sep 27, 2022
 *      Author: Kimi Jin
 */

#include "epd_4in2_depg0420bns.h"
#include "gpio.h"

static void epd_4in2_depg0420bns_spi_write(unsigned char value)
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

static void epd_4in2_depg0420bns_spi_write_cmd(unsigned char cmd)
{
	EPD_CS_0;
	EPD_DC_0;		// command write
	epd_4in2_depg0420bns_spi_write(cmd);
	EPD_CS_1;
}

static void epd_4in2_depg0420bns_write_data(unsigned char data)
{
	EPD_CS_0;
	EPD_DC_1;		// data write
	epd_4in2_depg0420bns_spi_write(data);
	EPD_CS_1;
}

static void epd_4in2_depg0420bns_reset(void)
{
	EPD_RST_0;
	HAL_Delay(10);
	EPD_RST_1;
	HAL_Delay(10);
}


static void epd_4in2_depg0420bns_read_busy(void)
{
	while(isEPD_BUSY == 1)
	{
		HAL_Delay(100);
	}
	HAL_Delay(200);
}


static void epd_4in2_depg0420bns_turn_on_display(void)
{
	epd_4in2_depg0420bns_spi_write_cmd(0x22);
	epd_4in2_depg0420bns_write_data(0xf7);
	epd_4in2_depg0420bns_spi_write_cmd(0x20);
	epd_4in2_depg0420bns_read_busy();
}


void epd_4in2_depg0420bns_init(void)
{
	epd_4in2_depg0420bns_reset();
	epd_4in2_depg0420bns_read_busy();

	epd_4in2_depg0420bns_spi_write_cmd(0x12);
	epd_4in2_depg0420bns_read_busy();

	epd_4in2_depg0420bns_spi_write_cmd(0x01);
	epd_4in2_depg0420bns_write_data(0x2b);
	epd_4in2_depg0420bns_write_data(0x01);
	epd_4in2_depg0420bns_write_data(0x00);

	epd_4in2_depg0420bns_spi_write_cmd(0x11);
	epd_4in2_depg0420bns_write_data(0x01);

	epd_4in2_depg0420bns_spi_write_cmd(0x44);
	epd_4in2_depg0420bns_write_data(0x00);
	epd_4in2_depg0420bns_write_data(0x31);

	epd_4in2_depg0420bns_spi_write_cmd(0x45);
	epd_4in2_depg0420bns_write_data(0x2b);
	epd_4in2_depg0420bns_write_data(0x01);
	epd_4in2_depg0420bns_write_data(0x00);
	epd_4in2_depg0420bns_write_data(0x00);

	epd_4in2_depg0420bns_spi_write_cmd(0x3c);
	epd_4in2_depg0420bns_write_data(0x01);

	epd_4in2_depg0420bns_spi_write_cmd(0x18);
	epd_4in2_depg0420bns_write_data(0x80);

	epd_4in2_depg0420bns_spi_write_cmd(0x4e);
	epd_4in2_depg0420bns_write_data(0x00);

	epd_4in2_depg0420bns_spi_write_cmd(0x4f);
	epd_4in2_depg0420bns_write_data(0x2b);
	epd_4in2_depg0420bns_write_data(0x01);
}


void epd_4in2_depg0420bns_clear(void)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_4IN2_DEPG0420BNS_WIDTH % 8 == 0) ? (EPD_4IN2_DEPG0420BNS_WIDTH / 8) : (EPD_4IN2_DEPG0420BNS_WIDTH / 8 + 1);
	height = EPD_4IN2_DEPG0420BNS_HEIGHT;

	epd_4in2_depg0420bns_spi_write_cmd(0x24);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			epd_4in2_depg0420bns_write_data(0xff);
		}
	}

	epd_4in2_depg0420bns_spi_write_cmd(0x26);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_4in2_depg0420bns_write_data(0x00);
		}
	}

	epd_4in2_depg0420bns_turn_on_display();
}


void epd_4in2_depg0420bns_display(unsigned char *img_black, unsigned char *img_red)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_4IN2_DEPG0420BNS_WIDTH % 8 == 0) ? (EPD_4IN2_DEPG0420BNS_WIDTH / 8) : (EPD_4IN2_DEPG0420BNS_WIDTH / 8 + 1);
	height = EPD_4IN2_DEPG0420BNS_HEIGHT;

	epd_4in2_depg0420bns_spi_write_cmd(0x24);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_4in2_depg0420bns_write_data(img_black[j + i * width]);
		}
	}

	epd_4in2_depg0420bns_spi_write_cmd(0x26);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_4in2_depg0420bns_write_data(img_red[j + i * width]);
		}
	}

	epd_4in2_depg0420bns_turn_on_display();

}


void epd_4in2_depg0420bns_sleep(void)
{
	epd_4in2_depg0420bns_spi_write_cmd(0x10);
	epd_4in2_depg0420bns_write_data(0x01);
	HAL_Delay(100);
}

