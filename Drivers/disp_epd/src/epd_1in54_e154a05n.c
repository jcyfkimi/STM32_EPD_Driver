/*
 * epd_1in54_e154a05n.c
 *
 *  Created on: 2022/10/5
 *      Author: Kimi Jin
 */
#include "gpio.h"
#include "epd_1in54_e154a05n.h"

static const unsigned char epd_1in54_e154a05n_lut_full_update[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

static const unsigned char epd_1in54_e154a05n_lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static void epd_1in54_e154a05n_spi_write(unsigned char value)
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

static void epd_1in54_e154a05n_spi_write_cmd(unsigned char cmd)
{
	EPD_CS_0;
	EPD_DC_0;		// command write
	epd_1in54_e154a05n_spi_write(cmd);
	EPD_CS_1;

}

static void epd_1in54_e154a05n_write_data(unsigned char data)
{
	EPD_CS_0;
	EPD_DC_1;		// data write
	epd_1in54_e154a05n_spi_write(data);
	EPD_CS_1;

}


static void epd_1in54_e154a05n_reset(void)
{
	EPD_RST_1;
	HAL_Delay(200);
	EPD_RST_0;
	HAL_Delay(10);
	EPD_RST_1;
	HAL_Delay(200);
}


static void epd_1in54_e154a05n_read_busy(void)
{
	while(isEPD_BUSY == 1)
	{
		HAL_Delay(100);
	}
	HAL_Delay(200);
}

static void epd_1in54_e154a05n_set_window(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
	epd_1in54_e154a05n_spi_write_cmd(0x44);
	epd_1in54_e154a05n_write_data((x_start >> 3) & 0xFF);
	epd_1in54_e154a05n_write_data((x_end >> 3) & 0xFF);

	epd_1in54_e154a05n_spi_write_cmd(0x45);
	epd_1in54_e154a05n_write_data((y_start) & 0xFF);
	epd_1in54_e154a05n_write_data((y_start >> 8) & 0xFF);
	epd_1in54_e154a05n_write_data((y_end) & 0xFF);
	epd_1in54_e154a05n_write_data((y_end >> 8) & 0xFF);
}

static void epd_1in54_e154a05n_set_cursor(uint16_t x_start, uint16_t y_start)
{
	epd_1in54_e154a05n_spi_write_cmd(0x4E);
	epd_1in54_e154a05n_write_data((x_start >> 3) & 0xFF);

	epd_1in54_e154a05n_spi_write_cmd(0x4F);
	epd_1in54_e154a05n_write_data((y_start) & 0xFF);
	epd_1in54_e154a05n_write_data((y_start >> 8) & 0xFF);
}

static void epd_1in54_e154a05n_turn_on_display(void)
{
	epd_1in54_e154a05n_spi_write_cmd(0x22);
	epd_1in54_e154a05n_write_data(0xc4);
	epd_1in54_e154a05n_spi_write_cmd(0x20);
	epd_1in54_e154a05n_spi_write_cmd(0xFF);

	epd_1in54_e154a05n_read_busy();
}

static void epd_1in54_e154a05n_init_common(void)
{
	epd_1in54_e154a05n_reset();

	epd_1in54_e154a05n_spi_write_cmd(0x01);
	epd_1in54_e154a05n_write_data((EPD_1IN54_E154A05N0_HEIGHT - 1) & 0xFF);
	epd_1in54_e154a05n_write_data(((EPD_1IN54_E154A05N0_HEIGHT - 1) >> 8) & 0xFF);
	epd_1in54_e154a05n_write_data(0x00);

	epd_1in54_e154a05n_spi_write_cmd(0x0c);
	epd_1in54_e154a05n_write_data(0xd7);
	epd_1in54_e154a05n_write_data(0xd6);
	epd_1in54_e154a05n_write_data(0x9d);

	epd_1in54_e154a05n_spi_write_cmd(0x2c);
	epd_1in54_e154a05n_write_data(0xa8);

	epd_1in54_e154a05n_spi_write_cmd(0x3a);
	epd_1in54_e154a05n_write_data(0x1a);

	epd_1in54_e154a05n_spi_write_cmd(0x3b);
	epd_1in54_e154a05n_write_data(0x08);

	epd_1in54_e154a05n_spi_write_cmd(0x11);
	epd_1in54_e154a05n_write_data(0x03);
}


void epd_1in54_e154a05n_init(void)
{
	int i = 0;
	epd_1in54_e154a05n_init_common();
	epd_1in54_e154a05n_spi_write_cmd(0x32);
	for(i = 0; i < 30; i++)
	{
		epd_1in54_e154a05n_write_data(epd_1in54_e154a05n_lut_full_update[i]);
	}
}

void epd_1in54_e154a05n_init_partial(void)
{
	int i = 0;
	epd_1in54_e154a05n_init_common();
	epd_1in54_e154a05n_spi_write_cmd(0x32);
	for(i = 0; i < 30; i++)
	{
		epd_1in54_e154a05n_write_data(epd_1in54_e154a05n_lut_partial_update[i]);
	}
}

void epd_1in54_e154a05n_clear(void)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	width = (EPD_1IN54_E154A05N_WIDTH % 8 == 0) ? (EPD_1IN54_E154A05N_WIDTH / 8) : (EPD_1IN54_E154A05N_WIDTH / 8 + 1);
	height = EPD_1IN54_E154A05N0_HEIGHT;

	epd_1in54_e154a05n_set_window(0, 0, EPD_1IN54_E154A05N_WIDTH, EPD_1IN54_E154A05N0_HEIGHT);
	for(j = 0; j < height; j++)
	{
		epd_1in54_e154a05n_set_cursor(0, j);
		epd_1in54_e154a05n_spi_write_cmd(0x24);
		for(i = 0; i < width; i++)
		{
			epd_1in54_e154a05n_write_data(0xff);
		}
	}

	epd_1in54_e154a05n_turn_on_display();
}

void epd_1in54_e154a05n_display(unsigned char *img_black, unsigned char *img_red)
{
	int width = 0;
	int height = 0;
	int i = 0;
	int j = 0;

	if(img_red);		// We're not using red image here, to avoid any kind of compile error.

	width = (EPD_1IN54_E154A05N_WIDTH % 8 == 0) ? (EPD_1IN54_E154A05N_WIDTH / 8) : (EPD_1IN54_E154A05N_WIDTH / 8 + 1);
	height = EPD_1IN54_E154A05N0_HEIGHT;

	epd_1in54_e154a05n_set_window(0, 0, EPD_1IN54_E154A05N_WIDTH, EPD_1IN54_E154A05N0_HEIGHT);
	for(j = 0; j < height; j++)
	{
		epd_1in54_e154a05n_set_cursor(0, j);
		epd_1in54_e154a05n_spi_write_cmd(0x24);
		for(i = 0; i < width; i++)
		{
			epd_1in54_e154a05n_write_data(img_black[i + j * width]);
		}
	}

	epd_1in54_e154a05n_turn_on_display();
}

void epd_1in54_e154a05n_sleep(void)
{
	epd_1in54_e154a05n_spi_write_cmd(0x10);
	epd_1in54_e154a05n_write_data(0x01);
}
