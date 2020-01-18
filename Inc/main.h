#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"


//definitions
#define CANID_SYNC	0x80

//function prototypes
void Error_Handler(void);
void Can_Send();


#ifdef __cplusplus
}
#endif

#endif
