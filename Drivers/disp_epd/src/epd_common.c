/*
 * epd_common.c
 *
 *  Created on: Sep 27, 2022
 *      Author: Kimi Jin
 */

#include "epd_common.h"

int epaper_display_init(epaper_display_t *epd)
{
	int ret = 0;

	if (epd == NULL)
	{
		return -1;
	}

	if (epd->epd_pn <= EPD_PN_MIN || epd->epd_pn >= EPD_PN_MAX)
	{
		// printf("epd not supported\n");
		return -1;
	}

	// initial the functions
	switch (epd->epd_pn)
	{
	case EPD_PN_DEPG0420BNS:
		epd->epd_init = epd_4in2_depg0420bns_init;
		epd->epd_clear = epd_4in2_depg0420bns_clear;
		epd->epd_display = epd_4in2_depg0420bns_display;
		epd->epd_sleep = epd_4in2_depg0420bns_sleep;
#ifdef EPD_BUILTIN_TEST_IMG_FOR_DEPG0420BNS
		epd->gImage_bw = gImage_4in2depgbns_bw;
		epd->gImage_r = gImage_4in2depgbns_r;
#endif
		epd->epd_inited = EPD_INIT_DONE;
		break;
	case EPD_PN_FPC2302:
		epd->epd_init = epd_3in71_fpc2302_init;
		epd->epd_clear = epd_3in71_fpc2302_clear;
		epd->epd_display = epd_3in71_fpc2302_display;
		epd->epd_sleep = epd_3in71_fpc2302_sleep;
#ifdef EPD_BUILTIN_TEST_IMG_FOR_FPC2302
		epd->gImage_bw = gImage_3in71fpc2302_bw;
		epd->gImage_r = gImage_3in71fpc2302_r;
#endif
		epd->epd_inited = EPD_INIT_DONE;
	case EPD_PN_BE2266ES0550:
		epd->epd_init = epd_2in66_be2266es0550_init;
		epd->epd_clear = epd_2in66_be2266es0550_clear;
		epd->epd_display = epd_2in66_be2266es0550_display;
		epd->epd_sleep = epd_2in66_be2266es0550_sleep;
#ifdef EPD_BUILTIN_TEST_IMG_FOR_BE2266ES0550
		epd->gImage_bw = gImage_2in66_be2266es0550_bw;
		epd->gImage_r = gImage_2in66_be2266es0550_r;
#endif
		epd->epd_inited = EPD_INIT_DONE;
	default:
		break;
	}
	return ret;
}

