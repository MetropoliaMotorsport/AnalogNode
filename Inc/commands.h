#ifndef COMMANDS_H_
#define COMMANDS_H_


//config commands
#define DIAGNOSE_CAN				1
#define CLEAR_ERROR					12
#define SAVE_CONFIGS				128
//TODO: configuration for pwm input, but PWM input currently not supported


//function prototypes
void Acknowledge(uint8_t);
void Save_Config();

void Diagnose_Can();
void Clear_Error(uint8_t);

//stuff here

#endif
