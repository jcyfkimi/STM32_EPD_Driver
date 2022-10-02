/*
 * epd_3in71_fpc2302.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Kimi Jin
 */

#ifndef _DISP_EPD_INC_EPD_3IN71_FPC2302_H_
#define _DISP_EPD_INC_EPD_3IN71_FPC2302_H_

#include "epd_common.h"

extern unsigned char gImage_3in71fpc2302_bw[];
extern unsigned char gImage_3in71fpc2302_r[];

#define EPD_3IN71_FPC2302_WIDTH				(240)
#define EPD_3IN71_FPC2302_HEIGHT			(416)

void epd_3in71_fpc2302_init(void);
void epd_3in71_fpc2302_clear(void);
void epd_3in71_fpc2302_display(unsigned char *img_black, unsigned char *img_red);
void epd_3in71_fpc2302_sleep(void);



#endif /* DISP_EPD_INC_EPD_3IN71_FPC2302_H_ */
