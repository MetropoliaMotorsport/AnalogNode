#include "main.h"
#include "commands.h"

//externs
extern FDCAN_HandleTypeDef hfdcan;


uint32_t ack_k=0;
uint32_t blnk_k=0;
extern uint8_t CANTxData[8];

void Acknowledge(uint8_t cmd)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	TxHeader.Identifier = CANID_ACK;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;

	CANTxData[0]=ID;
	CANTxData[1]=cmd;
	CANTxData[2]=(ack_k>>24)*0xFF;
	CANTxData[3]=(ack_k>>16)*0xFF;
	CANTxData[4]=(ack_k>>8)*0xFF;
	CANTxData[5]=ack_k&0xFF;
	CANTxData[6]=cmd;
	CANTxData[7]=0xFF;

	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) < 1)
	{
		//blinking at .5 Hz means something has gone wrong here, this should only happen during configuration so it is considered acceptable infinite loop location
		blnk_k++;
		if(blnk_k>340000000)
		{
			blnk_k=0;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
		}
	}

	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, CANTxData) != HAL_OK)
	{
		Set_Error(ERR_SEND_FAILED);
		return;
	}

	ack_k++;
}

void Save_Config()
{
	Config_Write_Flash();

	Config_Read_Flash();

	Acknowledge(SAVE_CONFIGS);
}

void Diagnose_Can()
{
	Can_Send_Diagnostics();
}

void Switch_Driver(uint8_t state)
{
	if (!driverError)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, state); //should be PA9, check with the new boards TODO to see if it works with them
		driverState = state;

		Acknowledge(SWITCH_DRIVER);
	}
}

void Clear_Error()
{
	driverError=0;

	Acknowledge(CLEAR_ERROR);
}

void Config_Measurements(uint8_t enable, uint8_t state)
{
	if (enable & (1<<0))
	{
		MeasureDriverCurrent = state&&(1<<0);
	}

	if (enable & (1<<1))
	{
		MeasureTemperature = state&&(1<<1);
	}
	Acknowledge(CONFIG_MEASUREMENTS);
}

void Config_Rolling_Averages(uint8_t enable, uint8_t newAverages[8])
{
	uint32_t pos=3; //start at pos 3 and use a length 8 array so that we can just pass rx data
	for(uint32_t i=0; i<4; i++)
	{
		if ((1<<i) & enable)
		{
			SensorRollingAverages[i] = newAverages[pos];
			pos++;
		}
	}

	Acknowledge(CONFIG_ROLLING_AVERAGES);
}

/*
uint16_t SensorRollingAverages[4]; //[AI2, AI3, AI5, AI6]
uint8_t TransferFunctions[4];

uint16_t CanId_Analog;
uint8_t AnalogSensorBytes[4]; //[AI2, AI3, AI5, AI6] //if AI2>2, AI3 not sent, if AI5>2 AI6 not sent //those situations reserved for higher resolution adcs than will be tested now
uint16_t CanId_Diagnostics;

uint16_t SendAnalogPeriod; //0 = use sync
uint16_t CanSyncDelay;

uint8_t DriverDefaultState; //TODO
uint32_t OverCurrentWarning; //in mA
uint32_t OverCurrentLimit; //in mA*/
