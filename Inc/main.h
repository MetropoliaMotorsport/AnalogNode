#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

//includes
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"
#include "config.h"
#include "transfer_functions.h"


//definitions
#define CANID_SYNC		0x80
#define CANID_CONFIG	0x700
#define CANID_ERROR		0x600

//function prototypes
void Error_Handler(void);
void Can_Send();
void Set_Error(uint32_t);
void Send_Error(void);


//warning and error codes
#define ERR_CAN_FIFO_FULL			1
#define ERR_SEND_FAILED				2

#define ERR_WRONG_BYTES				1
#define ERR_INCORRECT_TF_VOLTAGE	2


//config externs
extern uint8_t MeasureDriverCurrent;
extern uint8_t MeasureTemperature;
extern uint32_t SensorRollingAverages[4];
extern uint8_t TransferFunctions[4];


#ifdef __cplusplus
}
#endif

#endif
