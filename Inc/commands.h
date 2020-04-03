#ifndef COMMANDS_H_
#define COMMANDS_H_


//config commands
#define DIAGNOSE_CAN				1
#define SWITCH_DRIVER				2
#define CLEAR_ERROR					12
#define SAVE_CONFIGS				128
#define CONFIG_MEASUREMENTS			129
#define CONFIG_ROLLING_AVERAGES		130
#define CONFIG_TRANSFER_FUNCTIONS	131
#define CONFIG_ANALOG_ID			132
#define CONFIG_DIAGNOSTICS_ID		133


//function prototypes
void Acknowledge(uint8_t);
void Save_Config();

void Diagnose_Can();
void Switch_Driver(uint8_t);
void Clear_Error();

void Config_Measurements(uint8_t, uint8_t);
void Config_Rolling_Averages(uint8_t, uint8_t[8]);
void Config_Transfer_Functions(uint8_t, uint8_t[8]); //see transfer_functions.h to see the numbers associated with different transfer functions
void Config_Analog_ID(uint8_t, uint8_t);
void Config_Diagnostics_ID(uint8_t, uint8_t);

#endif
