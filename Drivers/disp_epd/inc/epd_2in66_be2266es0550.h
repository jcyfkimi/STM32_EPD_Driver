/*
 * epd_2in66_be2266es0550.h
 *
 *  Created on: 2022/10/2
 *      Author: Kimi Jin
 */

#ifndef _DISP_EPD_INC_EPD_2IN66_BE2266ES0550_H_
#define _DISP_EPD_INC_EPD_2IN66_BE2266ES0550_H_

#include "epd_common.h"

extern unsigned char gImage_2in66_be2266es0550_bw[];
extern unsigned char gImage_2in66_be2266es0550_r[];

#define EPD_2IN66_BE2266ES0550_WIDTH			(152)
#define EPD_2IN66_BE2266ES0550_HEIGHT			(296)

void epd_2in66_be2266es0550_init(void);
void epd_2in66_be2266es0550_clear(void);
void epd_2in66_be2266es0550_display(unsigned char *img_black, unsigned char *img_red);
void epd_2in66_be2266es0550_sleep(void);


#endif /* _DISP_EPD_INC_EPD_2IN66_BE2266ES0550_H_ */
