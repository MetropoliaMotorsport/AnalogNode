#include "main.h"
#include "config.h"


void Config_Setup(void)
{
	if(Flash_Read(FLASH_PAGE_63)==0xFFFFFFFF) //initialize the flash to avoid errors
	{

#if ID == 1

		Config_1();

#else //catch everything that is not a proper ID, give it settings that the debug board would get

		Config_0();

#endif

		Config_Write_Flash();
	}
	else //if flash is not blank read in values from flash
	{
		Config_Read_Flash();
	}
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
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 1; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //NC
	TransferFunctions[1] = DHABS106_20A; //AI3 //current sensor
	TransferFunctions[2] = RAW; //AI5 //regen
	TransferFunctions[3] = RAW; //AI6 //apps 0

	CanId_Analog = 0x680;
	AnalogSensorBytes[0] = 0;
	AnalogSensorBytes[1] = 2;
	AnalogSensorBytes[2] = 2;
	AnalogSensorBytes[3] = 2;
	CanId_Diagnostics = 0x681;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_9(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 32; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 1; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //brake temperature
	TransferFunctions[1] = NTC_NTC1_360; //AI3 //oil temperature
	TransferFunctions[2] = NTC_NTC1_680; //AI5 //water temperature
	TransferFunctions[3] = VOLTAGE_3V3_UNCAL; //AI6 //NC

	CanId_Analog = 0x690;
	AnalogSensorBytes[0] = 2; //TODO
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 0;
	CanId_Diagnostics = 0x691;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_10(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 32; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = RAW; //AI2 //suspension
	TransferFunctions[1] = NTC_NTC1_360; //AI3 //oil temperature
	TransferFunctions[2] = NTC_NTC1_680; //AI5 //water temperature
	TransferFunctions[3] = RAW; //AI6 //suspension

	CanId_Analog = 0x692;
	AnalogSensorBytes[0] = 2; //TODO need to calibrate
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 2; //TODO need to calibrate
	CanId_Diagnostics = 0x693;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_11(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 32; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //brake temperature
	TransferFunctions[1] = VOLTAGE_3V3_UNCAL; //AI3 //brake pressure
	TransferFunctions[2] = VOLTAGE_3V3_UNCAL; //AI5 //brake pressure
	TransferFunctions[3] = RAW; //AI6 //apps 1

	CanId_Analog = 0x694;
	AnalogSensorBytes[0] = 2; //TODO
	AnalogSensorBytes[1] = 1; //TODO
	AnalogSensorBytes[2] = 1; //TODO
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x695;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_12(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 32; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = NTC_NTC1_680; //AI2 //water temperature
	TransferFunctions[1] = NTC_NTC1_680; //AI3 //water temperature
	TransferFunctions[2] = NTC_NTC1_680; //AI5 //water temperature
	TransferFunctions[3] = NTC_NTC1_680; //AI6 //water temperature

	CanId_Analog = 0x696;
	AnalogSensorBytes[0] = 1;
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x697;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_13(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 1; //AI2
	SensorRollingAverages[1] = 1; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //NC
	TransferFunctions[1] = VOLTAGE_3V3_UNCAL; //AI3 //NC
	TransferFunctions[2] = RAW; //AI5 //suspension sensor
	TransferFunctions[3] = RAW; //AI6 //suspension sensor

	CanId_Analog = 0x698;
	AnalogSensorBytes[0] = 0;
	AnalogSensorBytes[1] = 0;
	AnalogSensorBytes[2] = 2; //todo needs to be calibrated
	AnalogSensorBytes[3] = 2; //todo needs to be calibrated
	CanId_Diagnostics = 0x699;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_14(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 32; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //brake temperature
	TransferFunctions[1] = VOLTAGE_3V3_UNCAL; //AI3 //brake temperature
	TransferFunctions[2] = NTC_NTC1_360; //AI5 //oil temperature
	TransferFunctions[3] = NTC_NTC1_360; //AI6 //oil temperature

	CanId_Analog = 0x69A;
	AnalogSensorBytes[0] = 2; //TODO
	AnalogSensorBytes[1] = 2; //TODO
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x69B;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_15(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 1; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //NC
	TransferFunctions[1] = ZTP_115M; //AI3 //tire temperature
	TransferFunctions[2] = ZTP_115M; //AI5 //tire temperature
	TransferFunctions[3] = ZTP_115M; //AI6 //tire temperature

	CanId_Analog = 0x69C;
	AnalogSensorBytes[0] = 0;
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x69D;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_16(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 1; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //NC
	TransferFunctions[1] = ZTP_115M; //AI3 //tire temperature
	TransferFunctions[2] = ZTP_115M; //AI5 //tire temperature
	TransferFunctions[3] = ZTP_115M; //AI6 //tire temperature

	CanId_Analog = 0x69E;
	AnalogSensorBytes[0] = 0;
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x69F;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_17(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 1; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //NC
	TransferFunctions[1] = ZTP_115M; //AI3 //tire temperature
	TransferFunctions[2] = ZTP_115M; //AI5 //tire temperature
	TransferFunctions[3] = ZTP_115M; //AI6 //tire temperature

	CanId_Analog = 0x6A0;
	AnalogSensorBytes[0] = 0;
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x6A1;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}

void Config_18(void)
{
	MeasureDriverCurrent = 0;
	MeasureTemperature = 1;
	SensorRollingAverages[0] = 1; //AI2
	SensorRollingAverages[1] = 32; //AI3
	SensorRollingAverages[2] = 32; //AI5
	SensorRollingAverages[3] = 32; //AI6
	TransferFunctions[0] = VOLTAGE_3V3_UNCAL; //AI2 //NC
	TransferFunctions[1] = ZTP_115M; //AI3 //tire temperature
	TransferFunctions[2] = ZTP_115M; //AI5 //tire temperature
	TransferFunctions[3] = ZTP_115M; //AI6 //tire temperature

	CanId_Analog = 0x6A2;
	AnalogSensorBytes[0] = 0;
	AnalogSensorBytes[1] = 1;
	AnalogSensorBytes[2] = 1;
	AnalogSensorBytes[3] = 1;
	CanId_Diagnostics = 0x6A3;

	SendAnalogPeriod = 0; //in 100us resolution
	CanSyncDelay = 0; //in 10us resolution

	OverCurrentWarning = 1000;
	OverCurrentLimit = 2000;
}


void Config_Write_Flash(void)
{
	uint32_t data[512] = {0};


	data[MEASURES_POS]=((MeasureDriverCurrent&0b1)<<0)+((MeasureTemperature&0b1)<<8);

	data[ROLLING_AVERAGE_0_POS]=((SensorRollingAverages[0]&0xFFFF)<<0)+((SensorRollingAverages[1]&0xFFFF)<<16);
	data[ROLLING_AVERAGE_1_POS]=((SensorRollingAverages[2]&0xFFFF)<<0)+((SensorRollingAverages[3]&0xFFFF)<<16);
	data[TRANSFER_FUNCTION_POS]=((TransferFunctions[0]&0xFF)<<0)+((TransferFunctions[1]&0xFF)<<8)+((TransferFunctions[2]&0xFF)<<16)+((TransferFunctions[3]&0xFF)<<24);

	data[CANIDS_POS]=((CanId_Analog&0xFFFF)<<0)+((CanId_Diagnostics&0xFFFF)<<16);
	data[CANBYTES_POS]=((AnalogSensorBytes[0]&0xFF)<<0)+((AnalogSensorBytes[1]&0xFF)<<8)+((AnalogSensorBytes[2]&0xFF)<<16)+((AnalogSensorBytes[3]&0xFF)<<24);
	data[DELAYS_POS]=((SendAnalogPeriod&0xFFFF)<<0)+((CanSyncDelay&0xFFFF)<<16);

	data[I_WARN_POS]=OverCurrentWarning;
	data[I_ERROR_POS]=OverCurrentLimit;


	Flash_Write(FLASH_PAGE_63, 63, data, 512);
}

void Config_Read_Flash(void)
{
	MeasureDriverCurrent=((MEASURES>>0)&0b1);
	MeasureTemperature=((MEASURES>>0)&0b1);

	SensorRollingAverages[0]=((ROLLING_AVERAGE_0>>0)&0xFFFF);
	SensorRollingAverages[1]=((ROLLING_AVERAGE_0>>16)&0xFFFF);
	SensorRollingAverages[2]=((ROLLING_AVERAGE_1>>0)&0xFFFF);
	SensorRollingAverages[3]=((ROLLING_AVERAGE_1>>16)&0xFFFF);
	TransferFunctions[0]=((TRANSFER_FUNCTION>>0)&0xFF);
	TransferFunctions[1]=((TRANSFER_FUNCTION>>8)&0xFF);
	TransferFunctions[2]=((TRANSFER_FUNCTION>>16)&0xFF);
	TransferFunctions[3]=((TRANSFER_FUNCTION>>24)&0xFF);

	CanId_Analog=((CANIDS>>0)&0xFFFF);
	CanId_Diagnostics=((CANIDS>>16)&0xFFFF);
	AnalogSensorBytes[0]=((CANBYTES>>0)&0xFF);
	AnalogSensorBytes[1]=((CANBYTES>>8)&0xFF);
	AnalogSensorBytes[2]=((CANBYTES>>16)&0xFF);
	AnalogSensorBytes[3]=((CANBYTES>>24)&0xFF);
	SendAnalogPeriod=((DELAYS>>0)&0xFFFF);
	CanSyncDelay=((DELAYS>>16)&0xFFFF);

	OverCurrentWarning=I_WARN;
	OverCurrentLimit=I_ERROR;
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
}
