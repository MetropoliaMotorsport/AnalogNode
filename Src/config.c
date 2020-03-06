#include "main.h"
#include "config.h"


void Config_Setup(void)
{
	/*if(Flash_Read(FLASH_PAGE_63)==0xFFFFFFFF) //initialize the flash to avoid errors
	{*/

#if ID == 1

		Config_1();

#else //catch everything that is not a proper ID, give it settings that the debug board would get

		Config_0();

#endif

		/*Config_Write_Flash();
	}
	else //if flash is not blank read in values from flash
	{
		Config_Read_Flash();
	}*/
}


void Config_0(void)
{
	MeasureDriverCurrent = 1;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 32; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_5V_UNCAL; //AI2
	TransferFunctions[1] = VOLTAGE_24V_UNCAL; //AI3
	TransferFunctions[2] = VOLTAGE_4V5_UNCAL; //AI5
	TransferFunctions[3] = VOLTAGE_3V3_UNCAL; //AI6

	CanId_Analog = 0x88;
	AnalogSensorBytes[0] = 2;
	AnalogSensorBytes[1] = 0;
	AnalogSensorBytes[2] = 0;
	AnalogSensorBytes[3] = 0;
	CanId_Diagnostics = 0x89;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_1(void)
{
	//TODO: setup code for each node that will be in the car
}

/*
void Config_Write_Flash(void)
{
	uint32_t data[512] = {0};

	//bytes: [underlimit high], [underlimit low], [overlimit high], [overlimit low]
	data[U5_VOLTAGE_WARNING_LIMIT_POS]=((warn_undervoltage_U5<<0)&0xFFFF)+((warn_overvoltage_U5<<16)&0xFFFF0000);
	data[U5_TEMPERATURE_WARNING_LIMIT_POS]=((warn_undertemperature_U5<<0)&0xFFFF)+((warn_overtemperature_U5<<16)&0xFFFF0000);
	data[U5_CURRENT_WARNING_LIMIT_I0_POS]=((warn_undercurrent_U5I0<<0)&0xFFFF)+((warn_overcurrent_U5I0<<16)&0xFFFF0000);
	data[U5_CURRENT_WARNING_LIMIT_I1_POS]=((warn_undercurrent_U5I1<<0)&0xFFFF)+((warn_overcurrent_U5I1<<16)&0xFFFF0000);
	data[U6_VOLTAGE_WARNING_LIMIT_POS]=((warn_undervoltage_U6<<0)&0xFFFF)+((warn_overvoltage_U6<<16)&0xFFFF0000);
	data[U6_TEMPERATURE_WARNING_LIMIT_POS]=((warn_undertemperature_U6<<0)&0xFFFF)+((warn_overtemperature_U6<<16)&0xFFFF0000);
	data[U6_CURRENT_WARNING_LIMIT_I0_POS]=((warn_undercurrent_U6I0<<0)&0xFFFF)+((warn_overcurrent_U6I0<<16)&0xFFFF0000);
	data[U6_CURRENT_WARNING_LIMIT_I1_POS]=((warn_undercurrent_U6I1<<0)&0xFFFF)+((warn_overcurrent_U6I1<<16)&0xFFFF0000);
	data[U7_VOLTAGE_WARNING_LIMIT_POS]=((warn_undervoltage_U7<<0)&0xFFFF)+((warn_overvoltage_U7<<16)&0xFFFF0000);
	data[U7_TEMPERATURE_WARNING_LIMIT_POS]=((warn_undertemperature_U7<<0)&0xFFFF)+((warn_overtemperature_U7<<16)&0xFFFF0000);
	data[U7_CURRENT_WARNING_LIMIT_I0_POS]=((warn_undercurrent_U7I0<<0)&0xFFFF)+((warn_overcurrent_U7I0<<16)&0xFFFF0000);
	data[U7_CURRENT_WARNING_LIMIT_I1_POS]=((warn_undercurrent_U7I1<<0)&0xFFFF)+((warn_overcurrent_U7I1<<16)&0xFFFF0000);

	//bytes: [x], [x], [digital in interrupt enable], [digital in enable]
	data[DIGITAL_IN_0_POS]=(Digital_In_EN&0xFF)+((Digital_In_Interrupt_EN&0xFF)<<8);

	//oh boy
	data[DIGITAL_IN_1_POS+0]=((Digital_In_Interrupt_Can_Falling[0]&0xFF)<<0)+((Digital_In_Interrupt_Can_Falling[1]&0xFF)<<8)+((Digital_In_Interrupt_Can_Falling[2])<<16)+((Digital_In_Interrupt_Can_Falling[3]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+1]=((Digital_In_Interrupt_Can_Falling[4]&0xFF)<<0)+((Digital_In_Interrupt_Can_Rising[0]&0xFF)<<8)+((Digital_In_Interrupt_Can_Rising[1]&0xFF)<<16)+((Digital_In_Interrupt_Can_Rising[2]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+2]=((Digital_In_Interrupt_Can_Rising[3]&0xFF)<<0)+((Digital_In_Interrupt_Can_Rising[4]&0xFF)<<8)+((Digital_In_Interrupt_Power_High_Rising[0]&0xFF)<<16)+((Digital_In_Interrupt_Power_High_Rising[1]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+3]=((Digital_In_Interrupt_Power_High_Rising[2]&0xFF)<<0)+((Digital_In_Interrupt_Power_High_Rising[3]&0xFF)<<8)+((Digital_In_Interrupt_Power_High_Rising[4]&0xFF)<<16)+((Digital_In_Interrupt_Power_High_Falling[0]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+4]=((Digital_In_Interrupt_Power_High_Falling[1]&0xFF)<<0)+((Digital_In_Interrupt_Power_High_Falling[2]&0xFF)<<8)+((Digital_In_Interrupt_Power_High_Falling[3]&0xFF)<<16)+((Digital_In_Interrupt_Power_High_Falling[4]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+5]=((Digital_In_Interrupt_PWM_Rising[0]&0xFF)<<0)+((Digital_In_Interrupt_PWM_Rising[1]&0xFF)<<8)+((Digital_In_Interrupt_PWM_Rising[2]&0xFF)<<16)+((Digital_In_Interrupt_PWM_Rising[3]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+6]=((Digital_In_Interrupt_PWM_Rising[4]&0xFF)<<0)+((Digital_In_Interrupt_PWM_Falling[0]&0xFF)<<8)+((Digital_In_Interrupt_PWM_Falling[1]&0xFF)<<16)+((Digital_In_Interrupt_PWM_Falling[2]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+7]=((Digital_In_Interrupt_PWM_Falling[3]&0xFF)<<0)+((Digital_In_Interrupt_PWM_Falling[4]&0xFF)<<8)+((Digital_In_Interrupt_Power_Low_Rising[0]&0xFF)<<16)+((Digital_In_Interrupt_Power_Low_Rising[1]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+8]=((Digital_In_Interrupt_Power_Low_Rising[2]&0xFF)<<0)+((Digital_In_Interrupt_Power_Low_Rising[3]&0xFF)<<8)+((Digital_In_Interrupt_Power_Low_Rising[4]&0xFF)<<16)+((Digital_In_Interrupt_Power_Low_Falling[0]&0xFF)<<24);
	data[DIGITAL_IN_1_POS+9]=((Digital_In_Interrupt_Power_Low_Falling[1]&0xFF)<<0)+((Digital_In_Interrupt_Power_Low_Falling[2]&0xFF)<<8)+((Digital_In_Interrupt_Power_Low_Falling[3]&0xFF)<<16)+((Digital_In_Interrupt_Power_Low_Falling[4]&0xFF)<<24);

	//bytes: [x], [x], [x], [x x U7/1 U7/0 U6/1 U6/0 U5/1 U5/0]
	data[DEFAULT_SWITCH_STATE_POS]=Default_Switch_State&0xFF;

	//bytes: [x], [x], [pwm input enable], [pwm output enable]
	data[PWM_EN_POS]=(PWM_Out_EN&0xFF)+((PWM_In_EN&0xFF)<<8);
	//bytes: [pwm prescaler high], [pwm prescaler low], [pwm DC high], [pwm DC low]; note that prescaler=32 is ~20kHz, so probably will never be more than one byte; also DC is currently set up so 255 = 100% and 0 = 0%; so is only one byte
	data[PWM_0_POS]=((PWM_Prescalers[0]&0xFFFF)<<16)+((PWM_Pulses[0]&0xFFFF)<<0);
	data[PWM_1_POS]=((PWM_Prescalers[1]&0xFFFF)<<16)+((PWM_Pulses[1]&0xFFFF)<<0);
	data[PWM_2_POS]=((PWM_Prescalers[2]&0xFFFF)<<16)+((PWM_Pulses[2]&0xFFFF)<<0);
	data[PWM_3_POS]=((PWM_Prescalers[3]&0xFFFF)<<16)+((PWM_Pulses[3]&0xFFFF)<<0);
	data[PWM_4_POS]=((PWM_Prescalers[4]&0xFFFF)<<16)+((PWM_Pulses[4]&0xFFFF)<<0);

	uint32_t CanPos[8] = {CAN_ID_0_POS, CAN_ID_1_POS, CAN_ID_2_POS, CAN_ID_3_POS, CAN_ID_4_POS, CAN_ID_5_POS, CAN_ID_6_POS, CAN_ID_7_POS};
	for(uint32_t i=0; i<8; i++)
	{
		//bytes: [unused], [can dlc], [can id high], [can id low] x8 //any can id outside the valid range of 0 to 2047 should be treated as disabled
		data[CanPos[i]]=(0)+((Can_DLCs[i]&0xFF)<<16)+(Can_IDs[i]&0xFFFF);
	}

	for(uint32_t i=0; i<8; i++)
	{
		//byte: [bytes of specific data]
		data[CAN_BYTES_1ST_POS+i*2]=Can_Config_Bytes[i][0]+(Can_Config_Bytes[i][1]<<8)+(Can_Config_Bytes[i][2]<<16)+(Can_Config_Bytes[i][3]<<24);
		data[CAN_BYTES_1ST_POS+i*2+1]=Can_Config_Bytes[i][4]+(Can_Config_Bytes[i][5]<<8)+(Can_Config_Bytes[i][6]<<16)+(Can_Config_Bytes[i][7]<<24);
		//byte: [id of data to send]
		data[CAN_DATAS_1ST_POS+i*2]=Can_Config_Datas[i][0]+(Can_Config_Datas[i][1]<<8)+(Can_Config_Datas[i][2]<<16)+(Can_Config_Datas[i][3]<<24);
		data[CAN_DATAS_1ST_POS+i*2+1]=Can_Config_Datas[i][4]+(Can_Config_Datas[i][5]<<8)+(Can_Config_Datas[i][6]<<16)+(Can_Config_Datas[i][7]<<24);
	}
	//note that only two messages may be sent on sync, or 3 if absolutely no other messages are being sent (including errors)
	//bytes: [can interval (.1 ms) high], [can interval (.1 ms) low], [send can message on timer], [send can messages on sync]
	data[CAN_SEND_EN_POS]=((Can_Sync_Enable&0xFF)<<0)+((Can_Timed_Enable&0xFF)<<8)+((Can_Interval&0xFFFF)<<16);
	//bytes: [can sync delay (10 us) high], [can sync delay (10 us) low], [x], [x]
	data[CAN_SYNC_DELAY_POS]=((Can_Sync_Delay&0xFFFF)<<16);

	//bytes: [temperature samples to take / continuous sampling], [voltage samples to take / continuous sampling], [temperature samples to take per interval], [voltage samples to take per inteval]
	data[TV_BURST_POS]=((sample_temperature&0xFF)<<0)+((sample_voltage&0xFF)<<8)+((SampleTemperatureBurst&0xFF)<<16)+((SampleVoltageBurst&0xFF)<<24);
	//bytes: [x], [x], [temperature/voltage sampling interval (.1 ms) high], [temperature/voltage sampling interval (.1 ms) low]
	data[TV_BURST_TIMING_POS]=((SampleTemperatureVoltagePeriod&0xFFFF)<<0);

	Flash_Write(FLASH_PAGE_63, 63, data, 512);
}

void Config_Read_Flash(void)
{
	warn_undervoltage_U5=((U5_VOLTAGE_WARNING_LIMIT>>0)&0xFFFF);
	warn_overvoltage_U5=((U5_VOLTAGE_WARNING_LIMIT>>16)&0xFFFF);
	warn_undertemperature_U5=((U5_TEMPERATURE_WARNING_LIMIT>>0)&0xFFFF);
	warn_overtemperature_U5=((U5_TEMPERATURE_WARNING_LIMIT>>16)&0xFFFF);
	warn_undercurrent_U5I0=((U5_CURRENT_WARNING_LIMIT_I0>>0)&0xFFFF);
	warn_overcurrent_U5I0=((U5_CURRENT_WARNING_LIMIT_I0>>16)&0xFFFF);
	warn_undercurrent_U5I1=((U5_CURRENT_WARNING_LIMIT_I1>>0)&0xFFFF);
	warn_overcurrent_U5I1=((U5_CURRENT_WARNING_LIMIT_I1>>16)&0xFFFF);
	warn_undervoltage_U6=((U6_VOLTAGE_WARNING_LIMIT>>0)&0xFFFF);
	warn_overvoltage_U6=((U6_VOLTAGE_WARNING_LIMIT>>16)&0xFFFF);
	warn_undertemperature_U6=((U6_TEMPERATURE_WARNING_LIMIT>>0)&0xFFFF);
	warn_overtemperature_U6=((U6_TEMPERATURE_WARNING_LIMIT>>16)&0xFFFF);
	warn_undercurrent_U6I0=((U6_CURRENT_WARNING_LIMIT_I0>>0)&0xFFFF);
	warn_overcurrent_U6I0=((U6_CURRENT_WARNING_LIMIT_I0>>16)&0xFFFF);
	warn_undercurrent_U6I1=((U6_CURRENT_WARNING_LIMIT_I1>>0)&0xFFFF);
	warn_overcurrent_U6I1=((U6_CURRENT_WARNING_LIMIT_I1>>16)&0xFFFF);
	warn_undervoltage_U7=((U7_VOLTAGE_WARNING_LIMIT>>0)&0xFFFF);
	warn_overvoltage_U7=((U7_VOLTAGE_WARNING_LIMIT>>16)&0xFFFF);
	warn_undertemperature_U7=((U7_TEMPERATURE_WARNING_LIMIT>>0)&0xFFFF);
	warn_overtemperature_U7=((U7_TEMPERATURE_WARNING_LIMIT>>16)&0xFFFF);
	warn_undercurrent_U7I0=((U7_CURRENT_WARNING_LIMIT_I0>>0)&0xFFFF);
	warn_overcurrent_U7I0=((U7_CURRENT_WARNING_LIMIT_I0>>16)&0xFFFF);
	warn_undercurrent_U7I1=((U7_CURRENT_WARNING_LIMIT_I1>>0)&0xFFFF);
	warn_overcurrent_U7I1=((U7_CURRENT_WARNING_LIMIT_I1>>16)&0xFFFF);

	Digital_In_EN = ((DIGITAL_IN_0>>0)&0b00011101); //bit for PB4 is 0 to ensure it isn't used as PB4 seemed to have hardware problems
	Default_Switch_State=((DEFAULT_SWITCH_STATE>>0)&0b00111111);


	//oh boy
	Digital_In_Interrupt_Can_Falling[0]=((*(&DIGITAL_IN_1+0))>>0)&0xFF;
	Digital_In_Interrupt_Can_Falling[1]=((*(&DIGITAL_IN_1+0))>>8)&0xFF;
	Digital_In_Interrupt_Can_Falling[2]=((*(&DIGITAL_IN_1+0))>>16)&0xFF;
	Digital_In_Interrupt_Can_Falling[3]=((*(&DIGITAL_IN_1+0))>>24)&0xFF;
	Digital_In_Interrupt_Can_Falling[4]=((*(&DIGITAL_IN_1+1))>>0)&0xFF;
	Digital_In_Interrupt_Can_Rising[0]=((*(&DIGITAL_IN_1+1))>>8)&0xFF;
	Digital_In_Interrupt_Can_Rising[1]=((*(&DIGITAL_IN_1+1))>>16)&0xFF;
	Digital_In_Interrupt_Can_Rising[2]=((*(&DIGITAL_IN_1+1))>>24)&0xFF;
	Digital_In_Interrupt_Can_Rising[3]=((*(&DIGITAL_IN_1+2))>>0)&0xFF;
	Digital_In_Interrupt_Can_Rising[4]=((*(&DIGITAL_IN_1+2))>>8)&0xFF;

	Digital_In_Interrupt_Power_High_Rising[0]=((*(&DIGITAL_IN_1+2))>>16)&0xFF;
	Digital_In_Interrupt_Power_High_Rising[1]=((*(&DIGITAL_IN_1+2))>>24)&0xFF;
	Digital_In_Interrupt_Power_High_Rising[2]=((*(&DIGITAL_IN_1+3))>>0)&0xFF;
	Digital_In_Interrupt_Power_High_Rising[3]=((*(&DIGITAL_IN_1+3))>>8)&0xFF;
	Digital_In_Interrupt_Power_High_Rising[4]=((*(&DIGITAL_IN_1+3))>>16)&0xFF;
	Digital_In_Interrupt_Power_High_Falling[0]=((*(&DIGITAL_IN_1+3))>>24)&0xFF;
	Digital_In_Interrupt_Power_High_Falling[1]=((*(&DIGITAL_IN_1+4))>>0)&0xFF;
	Digital_In_Interrupt_Power_High_Falling[2]=((*(&DIGITAL_IN_1+4))>>8)&0xFF;
	Digital_In_Interrupt_Power_High_Falling[3]=((*(&DIGITAL_IN_1+4))>>16)&0xFF;
	Digital_In_Interrupt_Power_High_Falling[4]=((*(&DIGITAL_IN_1+4))>>24)&0xFF;

	Digital_In_Interrupt_PWM_Rising[0]=((*(&DIGITAL_IN_1+5))>>0)&0xFF;
	Digital_In_Interrupt_PWM_Rising[1]=((*(&DIGITAL_IN_1+5))>>8)&0xFF;
	Digital_In_Interrupt_PWM_Rising[2]=((*(&DIGITAL_IN_1+5))>>16)&0xFF;
	Digital_In_Interrupt_PWM_Rising[3]=((*(&DIGITAL_IN_1+5))>>24)&0xFF;
	Digital_In_Interrupt_PWM_Rising[4]=((*(&DIGITAL_IN_1+6))>>0)&0xFF;
	Digital_In_Interrupt_PWM_Falling[0]=((*(&DIGITAL_IN_1+6))>>8)&0xFF;
	Digital_In_Interrupt_PWM_Falling[1]=((*(&DIGITAL_IN_1+6))>>16)&0xFF;
	Digital_In_Interrupt_PWM_Falling[2]=((*(&DIGITAL_IN_1+6))>>24)&0xFF;
	Digital_In_Interrupt_PWM_Falling[3]=((*(&DIGITAL_IN_1+7))>>0)&0xFF;
	Digital_In_Interrupt_PWM_Falling[4]=((*(&DIGITAL_IN_1+7))>>8)&0xFF;

	Digital_In_Interrupt_Power_Low_Rising[0]=((*(&DIGITAL_IN_1+7))>>16)&0xFF;
	Digital_In_Interrupt_Power_Low_Rising[1]=((*(&DIGITAL_IN_1+7))>>24)&0xFF;
	Digital_In_Interrupt_Power_Low_Rising[2]=((*(&DIGITAL_IN_1+8))>>0)&0xFF;
	Digital_In_Interrupt_Power_Low_Rising[3]=((*(&DIGITAL_IN_1+8))>>8)&0xFF;
	Digital_In_Interrupt_Power_Low_Rising[4]=((*(&DIGITAL_IN_1+8))>>16)&0xFF;
	Digital_In_Interrupt_Power_Low_Falling[0]=((*(&DIGITAL_IN_1+8))>>24)&0xFF;
	Digital_In_Interrupt_Power_Low_Falling[1]=((*(&DIGITAL_IN_1+9))>>0)&0xFF;
	Digital_In_Interrupt_Power_Low_Falling[2]=((*(&DIGITAL_IN_1+9))>>8)&0xFF;
	Digital_In_Interrupt_Power_Low_Falling[3]=((*(&DIGITAL_IN_1+9))>>16)&0xFF;
	Digital_In_Interrupt_Power_Low_Falling[4]=((*(&DIGITAL_IN_1+9))>>24)&0xFF;

	PWM_Out_EN = ((PWM_EN>>0)&0b00011111);
	PWM_Pulses[0] = ((PWM_0>>0)&0xFFFF);
	PWM_Prescalers[0] = ((PWM_0>>16)&0xFFFF);
	PWM_Pulses[1] = ((PWM_1>>0)&0xFFFF);
	PWM_Prescalers[1] = ((PWM_1>>16)&0xFFFF);
	PWM_Pulses[2] = ((PWM_2>>0)&0xFFFF);
	PWM_Prescalers[2] = ((PWM_2>>16)&0xFFFF);
	PWM_Pulses[3] = ((PWM_3>>0)&0xFFFF);
	PWM_Prescalers[3] = ((PWM_3>>16)&0xFFFF);
	PWM_Pulses[4] = ((PWM_4>>0)&0xFFFF);
	PWM_Prescalers[4] = ((PWM_4>>16)&0xFFFF);

	PWM_In_EN = ((PWM_EN>>8)&0b00011001); //ignore PB4 and PB5

	uint32_t CanId[8] = {CAN_ID_0, CAN_ID_1, CAN_ID_2, CAN_ID_3, CAN_ID_4, CAN_ID_5, CAN_ID_6, CAN_ID_7};
	for(uint32_t i=0; i<8; i++)
	{
		Can_IDs[i]=((CanId[i]>>0)&0xFFFF);
		Can_DLCs[i]=((CanId[i]>>16)&0xFF);
	}

	for(uint32_t i=0; i<8; i++)
	{
		uint32_t temp_can_bytes_0=*(&CAN_BYTES_1ST+i*2);
		uint32_t temp_can_bytes_1=*(&CAN_BYTES_1ST+i*2+1);
		uint32_t temp_can_datas_0=*(&CAN_DATAS_1ST+i*2);
		uint32_t temp_can_datas_1=*(&CAN_DATAS_1ST+i*2+1);
		for(uint32_t j=0; j<4; j++)
		{
			Can_Config_Bytes[i][j]=(temp_can_bytes_0>>(8*j)) & 0xFF;
			Can_Config_Bytes[i][j+4]=(temp_can_bytes_1>>(8*j)) & 0xFF;
			Can_Config_Datas[i][j]=(temp_can_datas_0>>(8*j)) & 0xFF;
			Can_Config_Datas[i][j+4]=(temp_can_datas_1>>(8*j)) & 0xFF;
		}
	}
	Can_Sync_Enable=(CAN_SEND_EN>>0)&0b11111111;
	Can_Timed_Enable=(CAN_SEND_EN>>8)&0b11111111;
	Can_Interval=(CAN_SEND_EN>>16)&0xFFFF;
	Can_Sync_Delay=(CAN_SYNC_DELAY>>16)*0xFFFF;

	sample_temperature = (TV_BURST>>0)&0xFF;
	sample_voltage = (TV_BURST>>8)&0xFF;
	SampleTemperatureBurst = (TV_BURST>>16)&0xFF;
	SampleVoltageBurst = (TV_BURST>>24)&0xFF;
	SampleTemperatureVoltagePeriod= (TV_BURST_TIMING>>0)&0xFFFF;
}


void Flash_Write(uint32_t Flash_Address, uint32_t Page, uint32_t Flash_Data[512], int Data_Words)
{
	FLASH_EraseInitTypeDef pEraseInit;
	uint32_t pError = 0;

	pEraseInit.Page = Page;
	pEraseInit.NbPages = 1;
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	__disable_irq();
	if(HAL_FLASH_Unlock() != HAL_OK)
	{
		__enable_irq();
		Error_Handler();
	}

	while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0) { }

	if(HAL_FLASHEx_Erase(&pEraseInit, &pError) != HAL_OK)
	{
		__enable_irq();
		while(1)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
			HAL_Delay(33);
		}
	}

	uint64_t data;
	for(int i=0; i<((Data_Words+1)/2); i++) //here we want integer division that rounds up instead of down
	{
		data=(uint64_t)Flash_Data[i*2];
		if((i*2)+1<Data_Words)
		{
			data+=((uint64_t)Flash_Data[i*2+1]<<32);
		}

			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Flash_Address+i*0x08, data) != HAL_OK)
			{
				__enable_irq();
				Error_Handler();
			}
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0) { }
	}

	if(HAL_FLASH_Lock() != HAL_OK)
	{
		__enable_irq();
		Error_Handler();
	}
	__enable_irq();
}

uint32_t Flash_Read(uint32_t Flash_Address)
{
	return *(uint32_t*)Flash_Address;
}*/
