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
#define CANID_CONFIG	0x601
#define CANID_ERROR		0x600

//temperature sensor definitions
#define T110cal (*(uint16_t*)(0x1FFF75CA))
#define T30cal (*(uint16_t*)(0x1FFF75A8))
#define T_m (8000/(T110cal-T30cal)) //m is in .01 °C / 1
#define T_b (3000-(T30cal*T_m)) //b is in .01 °C

//function prototypes
void Error_Handler(void);
void Can_Send_Analog();
void Can_Send_Diagnostics();
void Set_Error(uint32_t);
void Send_Error(void);


//warning and error codes
#define ERR_CAN_FIFO_FULL			1
#define ERR_SEND_FAILED				2
#define ERR_RECIEVED_INVALID_ID		3
#define ERR_RECIEVE_FAILED			4

#define ERR_WRONG_BYTES				33
#define ERR_INCORRECT_TF			34
#define ERR_INCORRECT_TF_VOLTAGE	35
#define ERR_INCORRECT_TF_NTC		36
#define ERR_INCORRECT_TF_I_TRANS	37


//config externs
extern uint8_t MeasureDriverCurrent;
extern uint8_t MeasureTemperature;
extern uint16_t SensorRollingAverages[4];
extern uint8_t TransferFunctions[4];

extern uint16_t CanId_Analog;
extern uint8_t AnalogSensorBytes[4];
extern uint16_t CanId_Diagnostics;

extern uint16_t SendAnalogPeriod;
extern uint16_t CanSyncDelay;


#ifdef __cplusplus
}
#endif

#endif
