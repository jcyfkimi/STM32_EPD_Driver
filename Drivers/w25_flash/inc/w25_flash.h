/*
 * w25_flash.h
 *
 *  Created on: Oct 6, 2022
 *      Author: Kimi Jin
 */

#ifndef _W25_FLASH_INC_W25_FLASH_H_
#define _W25_FLASH_INC_W25_FLASH_H_

#include "main.h"
#include "spi.h"
#include <stdio.h>

// https://github.com/ZHJ0125/USB_w25_flash/blob/master/Code/usb-C8T6/User/bsp/bsp_w25_flash.h

//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4018   //W25Q128
//#define  sFLASH_ID              0XEF4017    //W25Q64

#define W25X_FLASH_PAGE_SIZE            (256)
#define W25X_FLASH_PER_WRITE_PAGE_SIZE  (256)

#define W25X_WRITESTATUSREG		    	(0x01)
#define W25X_PAGEPROGRAM		      	(0x02)
#define W25X_READDATA			        (0x03)
#define W25X_WRITEDISABLE		      	(0x04)
#define W25X_ReEADSTATUSREG		    	(0x05)
#define W25X_WRITEENABLE		      	(0x06)
#define W25X_FASTREADDATA		      	(0x0B)
#define W25X_SECTORERASE		      	(0x20)
#define W25X_FASTREADDUAL		      	(0x3B)
#define W25X_MANUFACTDEVICEID   		(0x90)
#define W25X_JEDECDEVICEID		    	(0x9F)
#define W25X_RELEASEPOWERDOWN	    	(0xAB)
#define W25X_DEVICEID			        (0xAB)
#define W25X_POWERDOWN			      	(0xB9)
#define W25X_CHIPERASE			      	(0xC7)
#define W25X_BLOCKERASE			      	(0xD8)


#define WIP_FLAG                  		(0x01)
#define DUMMY_BYTE                		(0xFF)


#define SPIT_FLAG_TIMEOUT         		((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         		((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


#define FLASH_DEBUG_ON         			(1)
#define FLASH_DEBUG_FUNC_ON    			(0)

#define FLASH_INFO(fmt,arg...)          printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)         printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)         do{\
                                        	if(FLASH_DEBUG_ON)\
                                        	printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                        }while(0)

#define FLASH_DEBUG_FUNC()              do{\
                                        	if(FLASH_DEBUG_FUNC_ON)\
                                        	printf("<<-FLASH-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                        }while(0)

uint8_t w25_flash_initialize(void);
void w25_flash_sector_erase(uint32_t sector_addr);
void w25_flash_bulk_erase(void);
void w25_flash_page_write(uint8_t *pBuffer, uint32_t write_addr, uint16_t num_byte_to_write);
void w25_flash_buffer_write(uint8_t *pBuffer, uint32_t write_addr, uint16_t num_byte_to_write);
void w25_flash_buffer_read(uint8_t *pBuffer, uint32_t read_addr, uint16_t num_byte_to_read);
uint32_t w25_flash_read_ID(void);
uint32_t w25_flash_read_device_ID(void);
void w25_flash_start_read_sequence(uint32_t read_addr);
void w25_flash_power_down(void);
void w25_flash_wake_up(void);


uint8_t w25_flash_read_byte(void);
uint8_t w25_flash_send_byte(uint8_t byte);
uint16_t w25_flash_send_half_word(uint16_t half_word);
void w25_flash_write_enable(void);
void w25_flash_wait_for_write_end(void);

void w25_flash_write_no_check(uint8_t *pBuffer,uint32_t write_addr,uint16_t num_byte_to_write);
void w25_flash_write(uint8_t *pBuffer,uint32_t write_addr,uint16_t num_byte_to_write);

#endif /* _W25_FLASH_INC_W25_FLASH_H_ */
