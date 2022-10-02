/*
 * epd_2in66_be2266es0550.c
 *
 *  Created on: 2022/10/2
 *      Author: Kimi Jin
 */

#include "gpio.h"
#include "epd_2in66_be2266es0550.h"

static void epd_2in66_be2266es0550_spi_write(unsigned char value)
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

static void epd_2in66_be2266es0550_spi_write_cmd(unsigned char cmd)
{
	EPD_CS_0;
	EPD_DC_0;		// command write
	epd_2in66_be2266es0550_spi_write(cmd);
	EPD_CS_1;

}

static void epd_2in66_be2266es0550_write_data(unsigned char data)
{
	EPD_CS_0;
	EPD_DC_1;		// data write
	epd_2in66_be2266es0550_spi_write(data);
	EPD_CS_1;

}


static void epd_2in66_be2266es0550_reset(void)
{
	EPD_RST_1;
	HAL_Delay(200);
	EPD_RST_0;
	HAL_Delay(200);
	EPD_RST_1;
	HAL_Delay(200);
}


static void epd_2in66_be2266es0550_read_busy(void)
{
	while(isEPD_BUSY == 0)
	{
		HAL_Delay(100);
	}
	HAL_Delay(200);
}

static void epd_2in66_be2266es0550_turn_on_display(void)
{
	epd_2in66_be2266es0550_spi_write_cmd(0x12);
	epd_2in66_be2266es0550_read_busy();
}


void epd_2in66_be2266es0550_init(void)
{
	epd_2in66_be2266es0550_reset();

	epd_2in66_be2266es0550_spi_write_cmd(0x06); // BOOSTER_SOFT_START
	epd_2in66_be2266es0550_write_data(0x17);
	epd_2in66_be2266es0550_write_data(0x17);
	epd_2in66_be2266es0550_write_data(0x17);

	epd_2in66_be2266es0550_spi_write_cmd(0x04); // POWER_ON
	//Debug("SES266 POWER ON CHK Busy\r\n");
	epd_2in66_be2266es0550_read_busy();

	epd_2in66_be2266es0550_spi_write_cmd(0x00); // PANEL_SETTING
	epd_2in66_be2266es0550_write_data(0xCF);

	epd_2in66_be2266es0550_spi_write_cmd(0x50); // VCOM_AND_DATA_INTERVAL_SETTING
	epd_2in66_be2266es0550_write_data(0x77);

	epd_2in66_be2266es0550_spi_write_cmd(0x61); // RESOLUTION_SETTING
	//EPD_2IN66BSES_SendData(EPD_2IN66BSES_WIDTH); // width: 152
	//EPD_2IN66BSES_SendData(EPD_2IN66BSES_HEIGHT >> 8); // height: 292
	//EPD_2IN66BSES_SendData(EPD_2IN66BSES_HEIGHT & 0xFF);
	epd_2in66_be2266es0550_write_data(0x98);
	epd_2in66_be2266es0550_write_data(0x01);
	epd_2in66_be2266es0550_write_data(0x28);

	epd_2in66_be2266es0550_spi_write_cmd(0x82);
	epd_2in66_be2266es0550_write_data(0x0A);
}


void epd_2in66_be2266es0550_clear(void)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_2IN66_BE2266ES0550_WIDTH % 8 == 0) ? (EPD_2IN66_BE2266ES0550_WIDTH / 8) : (EPD_2IN66_BE2266ES0550_WIDTH / 8 + 1);
	height = EPD_2IN66_BE2266ES0550_HEIGHT;

	epd_2in66_be2266es0550_spi_write_cmd(0x10);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_2in66_be2266es0550_write_data(0xff);
		}
	}

	epd_2in66_be2266es0550_spi_write_cmd(0x13);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_2in66_be2266es0550_write_data(0xff);
		}
	}

	epd_2in66_be2266es0550_turn_on_display();
}


void epd_2in66_be2266es0550_display(unsigned char *img_black, unsigned char *img_red)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_2IN66_BE2266ES0550_WIDTH % 8 == 0) ? (EPD_2IN66_BE2266ES0550_WIDTH / 8) : (EPD_2IN66_BE2266ES0550_WIDTH / 8 + 1);
	height = EPD_2IN66_BE2266ES0550_HEIGHT;

	epd_2in66_be2266es0550_spi_write_cmd(0x10);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_2in66_be2266es0550_write_data(img_black[j + i * width]);
		}
	}

	epd_2in66_be2266es0550_spi_write_cmd(0x13);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			epd_2in66_be2266es0550_write_data(img_red[j + i * width]);
		}
	}

	epd_2in66_be2266es0550_turn_on_display();
}


void epd_2in66_be2266es0550_sleep(void)
{
	epd_2in66_be2266es0550_spi_write_cmd(0x02);
	epd_2in66_be2266es0550_read_busy();
	epd_2in66_be2266es0550_spi_write_cmd(0x07);
	epd_2in66_be2266es0550_write_data(0xa5);
	HAL_Delay(100);
}




