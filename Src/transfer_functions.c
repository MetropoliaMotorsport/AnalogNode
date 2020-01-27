#include "main.h"
#include "transfer_functions.h"


//lookup tables
uint16_t NTC_NTC1_680_LUT[2][16] = {	{ 419, 629, 757, 985, 1223, 1582, 1953, 2310, 2641, 2918, 3153, 3343, 3496, 3616, 3711, 3785 },
										{ 10, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150 } };
uint16_t NTC_NTC1_360_LUT[2][16] = {	{ 233, 359, 439, 588, 753, 1023, 1333, 1665, 2008, 2324, 2617, 2874, 3093, 3275, 3425, 3546 },
										{ 10, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150 } };

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
	case NTC_NTC1_680:
		transmit = TF_NTC(bytes, sensor, raw);
		break;
	case NTC_NTC1_360:
		transmit = TF_NTC(bytes, sensor, raw);
		break;
	default:
		Set_Error(ERR_INCORRECT_TF);
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
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
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
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
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
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
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
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
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
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
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
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
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
			voltage+=5000000;
			voltage/=10000000; //voltage in V
			break;
		case 2:
			voltage+=500;
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

uint32_t TF_NTC(uint8_t bytes, uint8_t resistor, uint16_t raw)
{
	uint32_t temperature = 0;

	switch(resistor)
	{
	case NTC_NTC1_680:

		break;
	case NTC_NTC1_360:

		break;
	default:
		Set_Error(ERR_INCORRECT_TF_NTC);
		break;
	}

	return temperature;
}


uint32_t LUT(uint16_t input, uint16_t* LUT, uint8_t LUT_length_LN2)
{
	uint8_t bound_low = 0;

	uint8_t bound_high = 1;
	for(uint32_t i=0; i<LUT_length_LN2; i++) { bound_high*=2; }
	bound_high-=1;

	volatile uint32_t a=0;
	for(uint32_t i=0; i<LUT_length_LN2; i++)
	{
		a++;
	}

	return 0;
}
