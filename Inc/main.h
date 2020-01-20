#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"


//definitions
#define CANID_SYNC		0x80
#define CANID_CONFIG	0x700
#define CANID_ERROR		0x600

#define ID			0 //TODO: move to config.h if same sort of config, probably is

//function prototypes
void Error_Handler(void);
void Can_Send();
void Set_Error(uint32_t);
void Send_Error(void);


//warning and error codes
#define ERR_CAN_FIFO_FULL			1
#define ERR_SEND_FAILED				2


#ifdef __cplusplus
}
#endif

#endif
