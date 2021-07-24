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
#include "commands.h"


//definitions
#define CANID_SYNC		0x80
#define CANID_CONFIG	0x602
#define CANID_ERROR		0x600
#define CANID_ACK	0x601

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
#define ERR_INVALID_COMMAND			5
#define ERR_COMMAND_SHORT			6

#define ERR_CANOFFLINE				11

#define ERR_WRONG_BYTES				33
#define ERR_INCORRECT_TF			34
#define ERR_INCORRECT_TF_VOLTAGE	35
#define ERR_INCORRECT_TF_NTC		36
#define ERR_INCORRECT_TF_I_TRANS	37

#define WARN_OVERCURR				49
#define ERR_OVERCURR_SHUTOFF		50

#define ERR_INVALID_CONFIG_ID		65


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

extern uint8_t DriverDefaultState; //TODO
extern uint32_t OverCurrentWarning; //in mA
extern uint32_t OverCurrentLimit; //in mA

extern uint8_t canDLC;

//externs
extern uint8_t driverState;
extern uint8_t driverError;


//more details about what is stored in each word can be found in config.c
//flash address definitions
#define MEASURES_POS					(0)
#define MEASURES						(*(uint32_t*)(FLASH_PAGE_63+0x4*MEASURES_POS))
#define ROLLING_AVERAGE_0_POS			(MEASURES_POS+1)
#define ROLLING_AVERAGE_0				(*(uint32_t*)(FLASH_PAGE_63+0x4*ROLLING_AVERAGE_0_POS))
#define ROLLING_AVERAGE_1_POS			(ROLLING_AVERAGE_0_POS+1)
#define ROLLING_AVERAGE_1				(*(uint32_t*)(FLASH_PAGE_63+0x4*ROLLING_AVERAGE_1_POS))
#define TRANSFER_FUNCTION_POS			(ROLLING_AVERAGE_1_POS+1)
#define TRANSFER_FUNCTION				(*(uint32_t*)(FLASH_PAGE_63+0x4*TRANSFER_FUNCTION_POS))
#define CANIDS_POS						(TRANSFER_FUNCTION_POS+1)
#define CANIDS							(*(uint32_t*)(FLASH_PAGE_63+0x4*CANIDS_POS))
#define CANBYTES_POS					(CANIDS_POS+1)
#define CANBYTES						(*(uint32_t*)(FLASH_PAGE_63+0x4*CANBYTES_POS))
#define DELAYS_POS						(CANBYTES_POS+1)
#define DELAYS							(*(uint32_t*)(FLASH_PAGE_63+0x4*DELAYS_POS))
#define I_WARN_POS						(DELAYS_POS+1)
#define I_WARN							(*(uint32_t*)(FLASH_PAGE_63+0x4*I_WARN_POS))
#define I_ERROR_POS						(I_WARN_POS+1)
#define I_ERROR							(*(uint32_t*)(FLASH_PAGE_63+0x4*I_ERROR_POS))
#define CANDLC_POS						(I_ERROR_POS+1)
#define CANDLC							(*(uint32_t*)(FLASH_PAGE_63+0x4*CANDLC_POS))


//flash page definitions
#define FLASH_PAGE_0 (0x8000000)
#define FLASH_PAGE_1 (FLASH_PAGE_0+0x800)
#define FLASH_PAGE_2 (FLASH_PAGE_1+0x800)
#define FLASH_PAGE_3 (FLASH_PAGE_2+0x800)
#define FLASH_PAGE_4 (FLASH_PAGE_3+0x800)
#define FLASH_PAGE_5 (FLASH_PAGE_4+0x800)
#define FLASH_PAGE_6 (FLASH_PAGE_5+0x800)
#define FLASH_PAGE_7 (FLASH_PAGE_6+0x800)
#define FLASH_PAGE_8 (FLASH_PAGE_7+0x800)
#define FLASH_PAGE_9 (FLASH_PAGE_8+0x800)
#define FLASH_PAGE_10 (FLASH_PAGE_9+0x800)
#define FLASH_PAGE_11 (FLASH_PAGE_10+0x800)
#define FLASH_PAGE_12 (FLASH_PAGE_11+0x800)
#define FLASH_PAGE_13 (FLASH_PAGE_12+0x800)
#define FLASH_PAGE_14 (FLASH_PAGE_13+0x800)
#define FLASH_PAGE_15 (FLASH_PAGE_14+0x800)
#define FLASH_PAGE_16 (FLASH_PAGE_15+0x800)
#define FLASH_PAGE_17 (FLASH_PAGE_16+0x800)
#define FLASH_PAGE_18 (FLASH_PAGE_17+0x800)
#define FLASH_PAGE_19 (FLASH_PAGE_18+0x800)
#define FLASH_PAGE_20 (FLASH_PAGE_19+0x800)
#define FLASH_PAGE_21 (FLASH_PAGE_20+0x800)
#define FLASH_PAGE_22 (FLASH_PAGE_21+0x800)
#define FLASH_PAGE_23 (FLASH_PAGE_22+0x800)
#define FLASH_PAGE_24 (FLASH_PAGE_23+0x800)
#define FLASH_PAGE_25 (FLASH_PAGE_24+0x800)
#define FLASH_PAGE_26 (FLASH_PAGE_25+0x800)
#define FLASH_PAGE_27 (FLASH_PAGE_26+0x800)
#define FLASH_PAGE_28 (FLASH_PAGE_27+0x800)
#define FLASH_PAGE_29 (FLASH_PAGE_28+0x800)
#define FLASH_PAGE_30 (FLASH_PAGE_29+0x800)
#define FLASH_PAGE_31 (FLASH_PAGE_30+0x800)
#define FLASH_PAGE_32 (FLASH_PAGE_31+0x800)
#define FLASH_PAGE_33 (FLASH_PAGE_32+0x800)
#define FLASH_PAGE_34 (FLASH_PAGE_33+0x800)
#define FLASH_PAGE_35 (FLASH_PAGE_34+0x800)
#define FLASH_PAGE_36 (FLASH_PAGE_35+0x800)
#define FLASH_PAGE_37 (FLASH_PAGE_36+0x800)
#define FLASH_PAGE_38 (FLASH_PAGE_37+0x800)
#define FLASH_PAGE_39 (FLASH_PAGE_38+0x800)
#define FLASH_PAGE_40 (FLASH_PAGE_39+0x800)
#define FLASH_PAGE_41 (FLASH_PAGE_40+0x800)
#define FLASH_PAGE_42 (FLASH_PAGE_41+0x800)
#define FLASH_PAGE_43 (FLASH_PAGE_42+0x800)
#define FLASH_PAGE_44 (FLASH_PAGE_43+0x800)
#define FLASH_PAGE_45 (FLASH_PAGE_44+0x800)
#define FLASH_PAGE_46 (FLASH_PAGE_45+0x800)
#define FLASH_PAGE_47 (FLASH_PAGE_46+0x800)
#define FLASH_PAGE_48 (FLASH_PAGE_47+0x800)
#define FLASH_PAGE_49 (FLASH_PAGE_48+0x800)
#define FLASH_PAGE_50 (FLASH_PAGE_49+0x800)
#define FLASH_PAGE_51 (FLASH_PAGE_50+0x800)
#define FLASH_PAGE_52 (FLASH_PAGE_51+0x800)
#define FLASH_PAGE_53 (FLASH_PAGE_52+0x800)
#define FLASH_PAGE_54 (FLASH_PAGE_53+0x800)
#define FLASH_PAGE_55 (FLASH_PAGE_54+0x800)
#define FLASH_PAGE_56 (FLASH_PAGE_55+0x800)
#define FLASH_PAGE_57 (FLASH_PAGE_56+0x800)
#define FLASH_PAGE_58 (FLASH_PAGE_57+0x800)
#define FLASH_PAGE_59 (FLASH_PAGE_58+0x800)
#define FLASH_PAGE_60 (FLASH_PAGE_59+0x800)
#define FLASH_PAGE_61 (FLASH_PAGE_60+0x800)
#define FLASH_PAGE_62 (FLASH_PAGE_61+0x800)
#define FLASH_PAGE_63 (FLASH_PAGE_62+0x800)


#ifdef __cplusplus
}
#endif

#endif
