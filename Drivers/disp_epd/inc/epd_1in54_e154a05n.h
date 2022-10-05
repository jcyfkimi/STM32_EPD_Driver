/*
 * epd_1in54_e154a05n.h
 *
 *  Created on: 2022/10/5
 *      Author: Kimi Jin
 */

#ifndef _DISP_EPD_INC_EPD_1IN54_E154A05N_H_
#define _DISP_EPD_INC_EPD_1IN54_E154A05N_H_

#include "epd_common.h"

extern unsigned char gImage_1in54_e154a05n_bw[];

#define EPD_1IN54_E154A05N_WIDTH			(200)
#define EPD_1IN54_E154A05N0_HEIGHT			(200)

void epd_1in54_e154a05n_init(void);
void epd_1in54_e154a05n_init_partial(void);
void epd_1in54_e154a05n_clear(void);
void epd_1in54_e154a05n_display(unsigned char *img_black, unsigned char *img_red);
void epd_1in54_e154a05n_sleep(void);

#endif /* _DISP_EPD_INC_EPD_1IN54_E154A05N_H_ */
