#include "main.h"
#include "transfer_functions.h"


uint32_t TF_Select(uint8_t bytes, uint8_t sensor, uint16_t raw)
{
	uint32_t transmit = 0;

	switch(sensor)
	{
	case VOLTAGE_3V3_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_4V5_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_5V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_9V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_12V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_24V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_30V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	default:
		//TODO: add error here
		break;
	}

	return transmit;
}


//note that these will overflow if higher than 12 bit resolution on the adc is used
uint32_t TF_Voltage(uint8_t bytes, uint8_t divider, uint16_t raw)
{
	uint32_t voltage = 0;

	switch(divider) //calculate voltage in uV
	{
	case VOLTAGE_3V3_UNCAL:
		voltage = (raw*103125)/128; // C = 3.3*1000000/2^12 //3.3/2^12 gives in V
		switch(bytes)
		{
		case 1:
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_4V5_UNCAL:
		voltage = (raw*55327)/50; // C*(1.21+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_5V_UNCAL:
		voltage = (raw*247667)/20; // C*(1.74+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_9V_UNCAL:
		voltage = (raw*44759)/20; // C*(5.76+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_12V_UNCAL:
		voltage = (raw*301129)/100; // C*(8.87+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_24V_UNCAL:
		voltage = (raw*150689)/25; // C*(21+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_30V_UNCAL:
		voltage = (raw*744493)/100; // C*(26.7+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage/=10000000; //voltage in V
			break;
		case 2:
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	default:
		Set_Error(ERR_INCORRECT_TF_VOLTAGE);
		break;
	}

	return voltage;
}
