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
#ifdef EPD_BUILTIN_TEST_IMG
		epd->gImage_bw = gImage_4in2depgbns_bw;
		epd->gImage_r = gImage_4in2depgbns_r;
#endif
		break;
	case EPD_PN_FPC2302:
		epd->epd_init = epd_3in71_fpc2302_init;
		epd->epd_clear = epd_3in71_fpc2302_clear;
		epd->epd_display = epd_3in71_fpc2302_display;
		epd->epd_sleep = epd_3in71_fpc2302_sleep;
#ifdef EPD_BUILTIN_TEST_IMG
		epd->gImage_bw = gImage_3in71fpc2302_bw;
		epd->gImage_r = gImage_3in71fpc2302_r;
#endif
	default:
		break;
	}
	return ret;
}
