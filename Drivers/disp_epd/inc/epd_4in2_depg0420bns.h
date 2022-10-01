/*
 * epd_4in2_depg0420bns.h
 *
 *  Created on: Sep 27, 2022
 *      Author: Kimi Jin
 */

#ifndef _DISP_EPD_INC_EPD_4IN2_DEPG0420BNS_H_
#define _DISP_EPD_INC_EPD_4IN2_DEPG0420BNS_H_

#include "epd_common.h"


#define EPD_BUILTIN_TEST_IMG
#ifdef EPD_BUILTIN_TEST_IMG
extern unsigned char gImage_4in2depgbns_bw[];
extern unsigned char gImage_4in2depgbns_r[];
#endif

#define EPD_4IN2_DEPG0420BNS_WIDTH			(400)
#define EPD_4IN2_DEPG0420BNS_HEIGHT			(300)

void epd_4in2_depg0420bns_init(void);
void epd_4in2_depg0420bns_clear(void);
void epd_4in2_depg0420bns_display(unsigned char *img_black, unsigned char *img_red);
void epd_4in2_depg0420bns_sleep(void);

#endif /* DISP_EPD_INC_EPD_4IN2_DEPG0420BNS_H_ */
