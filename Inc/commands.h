#ifndef COMMANDS_H_
#define COMMANDS_H_


//config commands
#define DIAGNOSE_CAN				1
#define SWITCH_DRIVER				2
#define CLEAR_ERROR					12
#define SAVE_CONFIGS				128 //TODO this next


//function prototypes
void Acknowledge(uint8_t);
void Save_Config();

void Diagnose_Can();
void Switch_Driver(uint8_t);
void Clear_Error(uint8_t);

//stuff here

#endif
