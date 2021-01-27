#ifndef TRANSFER_FUNCTIONS_H_
#define TRANSFER_FUNCTIONS_H_


//definitions
#define	RAW						0
#define VOLTAGE_3V3_UNCAL		1
#define VOLTAGE_4V5_UNCAL		2
#define VOLTAGE_5V_UNCAL		3
#define VOLTAGE_9V_UNCAL		4
#define VOLTAGE_12V_UNCAL		5
#define VOLTAGE_24V_UNCAL		6
#define VOLTAGE_30V_UNCAL		7
#define NTC_NTC1_680			8
#define NTC_NTC1_360			9
#define DHABS106_20A			10
#define DHABS106_500A			11
#define ZTP_115M				12
#define SOE_BRK_PRES			13
#define INFKL-800				14

//prototypes
uint32_t TF_Select(uint8_t, uint8_t, uint16_t);

uint32_t TF_Raw(uint8_t, uint16_t);
uint32_t TF_Voltage(uint8_t, uint8_t, uint16_t);
uint32_t TF_NTC(uint8_t, uint8_t, uint16_t);
uint32_t TF_I_Transducer(uint8_t, uint8_t, uint16_t);
uint32_t TF_ZTP_115M(uint8_t, uint16_t);
uint32_t TF_SOE_BRK_PRES(uint8_t, uint16_t);
uint32_t TF_INFKL-800(uint8_t, uint16_t);

uint32_t LUT(uint16_t, uint32_t*, uint8_t);


#endif
