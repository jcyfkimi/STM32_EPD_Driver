/*
 * epd_common.h
 *
 *  Created on: Sep 27, 2022
 *      Author: Kimi Jin
 */

#ifndef _DISP_EPD_INC_EPD_COMMON_H_
#define _DISP_EPD_INC_EPD_COMMON_H_

#include <stdio.h>
#include "epd_4in2_depg0420bns.h"
#include "epd_3in71_fpc2302.h"

#define MAX_EPD_PN_STR_LEN		(32)

#define EPD_BUILTIN_TEST_IMG

typedef enum
{
	EPD_PN_MIN = 0,
	EPD_PN_DEPG0420BNS = 1,
	EPD_PN_FPC2302 = 2,
	EPD_PN_MAX
}epd_part_number;

typedef struct epaper_display
{
	epd_part_number epd_pn;
	char epd_pn_str[MAX_EPD_PN_STR_LEN];
	unsigned int epd_width;
	unsigned int epd_height;
	void (*epd_init)(void);
	void (*epd_partial_init)(void);
	void (*epd_gray_init)(void);
	void (*epd_clear)(void);
	void (*epd_display)(unsigned char *, unsigned char *);
	void (*epd_partial_display)(int, int, int, int, unsigned char *);
	void (*epd_gray_display)(unsigned char *);
	void (*epd_sleep)(void);
#ifdef EPD_BUILTIN_TEST_IMG
	unsigned char *gImage_bw;
	unsigned char *gImage_r;
#endif
}epaper_display_t;


int epaper_display_init(epaper_display_t * epd);



#endif /* _DISP_EPD_INC_EPD_COMMON_H_ */
