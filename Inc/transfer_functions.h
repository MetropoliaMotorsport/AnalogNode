#ifndef TRANSFER_FUNCTIONS_H_
#define TRANSFER_FUNCTIONS_H_


//definitions
#define VOLTAGE_3V3_UNCAL		1
#define VOLTAGE_4V5_UNCAL		2
#define VOLTAGE_5V_UNCAL		3
#define VOLTAGE_9V_UNCAL		4
#define VOLTAGE_12V_UNCAL		5
#define VOLTAGE_24V_UNCAL		6
#define VOLTAGE_30V_UNCAL		7

//prototypes
uint32_t TF_Voltage(uint8_t, uint8_t, uint16_t);


#endif
