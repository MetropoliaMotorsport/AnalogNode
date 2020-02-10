#include "main.h"


//initialization function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_FDCAN_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM16_Init(void);

//handlers
DMA_HandleTypeDef hdma_adc1;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc2;
ADC_HandleTypeDef hadc2;
FDCAN_HandleTypeDef hfdcan;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim16;

//buffers
uint8_t CANTxData[8];
uint32_t ADC1Data[3];
uint32_t ADC2Data[3];
uint32_t ADCRawData[6][ROLLING_AVERAGE_MAX]; //[AI2, AI3, Temp, AI5, AI6, driver current]
uint32_t AI2Pos;
uint32_t AI3Pos;
uint32_t TPos;
uint32_t AI5Pos;
uint32_t AI6Pos;
uint32_t IPos;
uint32_t AI2Written;
uint32_t AI3Written;
uint32_t TWritten;
uint32_t AI5Written;
uint32_t AI6Written;
uint32_t IWritten;

//config variables
uint8_t MeasureDriverCurrent;
uint8_t MeasureTemperature;
uint16_t SensorRollingAverages[4]; //[AI2, AI3, AI5, AI6]
uint8_t TransferFunctions[4];

uint16_t CanId_Analog;
uint8_t AnalogSensorBytes[4]; //[AI2, AI3, AI5, AI6] //if AI2>2, AI3 not sent, if AI5>2 AI6 not sent //those situations reserved for higher resolution adcs than will be tested now
uint16_t CanId_Diagnostics;

uint16_t SendAnalogPeriod; //0 = use sync
uint16_t CanSyncDelay;

//global variables
uint8_t canErrorToTransmit; //8 32 bit values, each 32 bit value can store 32 errors or warnings
uint32_t canErrors[8];
uint8_t canSendErrorFlag;
uint8_t canSendFlag;


int main(void)
{
	HAL_Init();

	SystemClock_Config();

	Config_Setup();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_FDCAN_Init();
	MX_TIM6_Init();
	MX_TIM7_Init();
	MX_TIM16_Init();

    if (HAL_ADC_Start_DMA(&hadc1, ADC1Data, hadc1.Init.NbrOfConversion) != HAL_OK) { Error_Handler(); }
    if (HAL_ADC_Start_DMA(&hadc2, ADC2Data, hadc2.Init.NbrOfConversion) != HAL_OK) { Error_Handler(); }


	while (1)
	{
		if (canSendFlag)
		{
			Can_Send_Analog();
			canSendFlag=0;
		}

		if (canErrorToTransmit && canSendErrorFlag)
		{
			Send_Error();
			if(!canErrorToTransmit)
			{
				canSendErrorFlag=0;
			}
		}
	}
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	FDCAN_RxHeaderTypeDef RxHeader = {0};
	uint8_t CANRxData[8] = {0};

	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)
	{
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, CANRxData) != HAL_OK)
		{
			Set_Error(ERR_RECIEVE_FAILED);
		}

		//set any bytes not actaully read to 0 to prevent unknown values being in them
		for(uint32_t i=(RxHeader.DataLength>>16); i<8; i++)
		{
			CANRxData[i]=0;
		}

		if (RxHeader.Identifier == CANID_SYNC)
		{
			if(!SendAnalogPeriod)
			{
				if(CanSyncDelay)
				{
					HAL_TIM_Base_Start_IT(&htim16);
				}
				else
				{
					canSendFlag = 1;
				}
			}
		}
		//TODO: config
		else
		{
			Set_Error(ERR_RECIEVED_INVALID_ID);
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM16)
	{
		HAL_TIM_Base_Stop_IT(&htim16);
		canSendFlag = 1;
	}
	else if (htim->Instance == TIM7)
	{
		canSendErrorFlag=1;
	}
	else if (htim->Instance == TIM6)
	{
		canSendFlag=1;
	}
	else
	{
		Error_Handler();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1)
	{
		ADCRawData[0][AI2Pos] = ADC1Data[0];
		AI2Pos++;
		if (AI2Pos>(SensorRollingAverages[0]-1))
		{
			AI2Pos = 0;
		}
		else if (AI2Written<AI2Pos)
		{
			AI2Written = AI2Pos;
		}

		ADCRawData[1][AI3Pos] = ADC1Data[1];
		AI3Pos++;
		if (AI3Pos>(SensorRollingAverages[1]-1))
		{
			AI3Pos = 0;
		}
		else if (AI3Written<AI3Pos)
		{
			AI3Written = AI3Pos;
		}

		ADCRawData[2][TPos] = ADC1Data[2];
		TPos++;
		if (TPos>(T_ROLLING_AVERAGE-1))
		{
			TPos = 0;
		}
		else if (TWritten<TPos)
		{
			TWritten = TPos;
		}
	}
	else if (hadc->Instance == ADC2)
	{
		ADCRawData[3][AI5Pos] = ADC2Data[0];
		AI5Pos++;
		if (AI5Pos>(SensorRollingAverages[2]-1))
		{
			AI5Pos = 0;
		}
		else if (AI5Written<AI5Pos)
		{
			AI5Written = AI5Pos;
		}

		ADCRawData[4][AI6Pos] = ADC2Data[1];
		AI6Pos++;
		if (AI6Pos>(SensorRollingAverages[3]-1))
		{
			AI6Pos = 0;
		}
		else if (AI6Written<AI6Pos)
		{
			AI6Written = AI6Pos;
		}

		ADCRawData[5][IPos] = ADC1Data[2];
		IPos++;
		if (IPos>(I_ROLLING_AVERAGE-1))
		{
			IPos = 0;
		}
		else if (IWritten<IPos)
		{
			IWritten = IPos;
		}
	}
	else
	{
		Error_Handler();
	}
}


void Can_Send_Analog()
{
	if(!(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) > 0))
	{
		Set_Error(ERR_CAN_FIFO_FULL);
		return;
	}

	FDCAN_TxHeaderTypeDef TxHeader;

	TxHeader.Identifier = 0x90;
	TxHeader.DataLength = (8<<16); //<<16 makes storing the number of bytes not require a switch statement for classic can

	//clear can tx data so that data from incorrectly configured message is 0
	for(uint32_t i=0; i<8; i++)
	{
		CANTxData[i]=0;
	}

	uint32_t raws[4] = {0};

	for(uint32_t i=0; i<ROLLING_AVERAGE_MAX; i++)
	{
		if (i<=AI2Written)
		{
			raws[0]+=ADCRawData[0][i];
		}
		if (i<=AI3Written)
		{
			raws[1]+=ADCRawData[1][i];
		}
		if (i<=AI5Written)
		{
			raws[2]+=ADCRawData[3][i];
		}
		if (i<=AI6Written)
		{
			raws[3]+=ADCRawData[4][i];
		}
	}
	raws[0]/=(AI2Written+1); raws[1]/=(AI3Written+1); raws[2]/=(AI5Written+1); raws[3]/=(AI6Written+1);

	uint32_t bytePos=0;
	for(uint32_t i=0; i<4; i++)
	{
		if (AnalogSensorBytes[i])
		{
			uint32_t transmit=TF_Select(AnalogSensorBytes[i], TransferFunctions[i], raws[i]);
			for(uint32_t j=0; j<AnalogSensorBytes[i]; j++)
			{
				CANTxData[bytePos] = transmit >> ((AnalogSensorBytes[i]-(j+1))*8);
				bytePos++;
			}
		}

		if (bytePos>7)
		{
			break;
		}
	}

	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, CANTxData) != HAL_OK)
	{
		Set_Error(ERR_SEND_FAILED);
		return;
	}
}

void Can_Send_Diagnostics()
{
	if(!(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) > 0))
	{
		Set_Error(ERR_CAN_FIFO_FULL);
		return;
	}

	FDCAN_TxHeaderTypeDef TxHeader;

	TxHeader.Identifier = 0x90;
	TxHeader.DataLength = (8<<16); //<<16 makes storing the number of bytes not require a switch statement for classic can

	//clear can tx data so that data from incorrectly configured message is 0
	for(uint32_t i=0; i<8; i++)
	{
		CANTxData[i]=0;
	}

	CANTxData[0] = 255;
	CANTxData[1] = 127;
	CANTxData[2] = 63;
	CANTxData[3] = 31;
	CANTxData[4] = 15;
	CANTxData[5] = 7;
	CANTxData[6] = 3;
	CANTxData[7] = 1;
	//TODO: SET OUTPUT BYTES

	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, CANTxData) != HAL_OK)
	{
		Set_Error(ERR_SEND_FAILED);
		return;
	}
}

void Set_Error(uint32_t error)
{
	canErrors[(error/32)]  |= (1<<(error%32));
	canErrorToTransmit |= (1<<(error/32));
}

void Send_Error(void)
{
	for(uint32_t i=0; i<8; i++)
	{
		if (canErrorToTransmit&(1<<i))
		{
			if(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) > 0)
			{
				FDCAN_TxHeaderTypeDef TxHeader;

				TxHeader.Identifier = CANID_ERROR;
				TxHeader.DataLength = FDCAN_DLC_BYTES_6;

				CANTxData[0]=ID;
				CANTxData[1]=i;
				CANTxData[2]=(canErrors[i]>>24)&0xFF;
				CANTxData[3]=(canErrors[i]>>16)&0xFF;
				CANTxData[4]=(canErrors[i]>>8)&0xFF;
				CANTxData[5]=(canErrors[i]>>0)&0xFF;

				TxHeader.IdType = FDCAN_STANDARD_ID;
				TxHeader.TxFrameType = FDCAN_DATA_FRAME;
				TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
				TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
				TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
				TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
				TxHeader.MessageMarker = 0;

				if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, CANTxData) != HAL_OK)
				{
					Set_Error(ERR_SEND_FAILED);
					return;
				}
				else
				{
					//if we sent the error message clear the error so that if it only occurs once the error is not sent continuously
					canErrors[i]=0;
					canErrorToTransmit &= ~(1<<i);
				}
			}
		}
	}
}


void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_8) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_FDCAN;
	PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_HSE;
	PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_DMA_Init(void)
{
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
}

static void MX_ADC1_Init(void)
{
	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.GainCompensation = 0;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	if (MeasureTemperature) { hadc1.Init.NbrOfConversion = 3; }
	else { hadc1.Init.NbrOfConversion = 2; }
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	multimode.Mode = ADC_MODE_INDEPENDENT;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_12_10_BITS;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_12CYCLES;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}


	sConfig.Channel = ADC_CHANNEL_3; //AI2, PA2
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_4; //AI3, PA3
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (MeasureTemperature)
	{
		sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1; //internal temperature
		sConfig.Rank = ADC_REGULAR_RANK_3;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_ADC2_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
	hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.GainCompensation = 0;
	hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc2.Init.LowPowerAutoWait = DISABLE;
	hadc2.Init.ContinuousConvMode = ENABLE;
	if (MeasureDriverCurrent) { hadc2.Init.NbrOfConversion = 3; }
	else { hadc2.Init.NbrOfConversion = 2; }
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc2.Init.DMAContinuousRequests = ENABLE;
	hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc2.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc2) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_13; //AI5, PA5
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_3; //AI6, PA6
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (MeasureDriverCurrent)
	{
		sConfig.Channel = ADC_CHANNEL_4; //current feedback from driver
		sConfig.Rank = ADC_REGULAR_RANK_3;
		if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}

	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_FDCAN_Init(void)
{
	FDCAN_FilterTypeDef	sFilterConfig;

	hfdcan.Instance = FDCAN1;
	hfdcan.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	hfdcan.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan.Init.AutoRetransmission = DISABLE;
	hfdcan.Init.TransmitPause = DISABLE;
	hfdcan.Init.ProtocolException = DISABLE;
	hfdcan.Init.NominalPrescaler = 1;
	hfdcan.Init.NominalSyncJumpWidth = 1;
	hfdcan.Init.NominalTimeSeg1 = 13;
	hfdcan.Init.NominalTimeSeg2 = 2;
	hfdcan.Init.DataPrescaler = 1;
	hfdcan.Init.DataSyncJumpWidth = 1;
	hfdcan.Init.DataTimeSeg1 = 1;
	hfdcan.Init.DataTimeSeg2 = 1;
	hfdcan.Init.StdFiltersNbr = 28;
	hfdcan.Init.ExtFiltersNbr = 0;
	hfdcan.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	if (HAL_FDCAN_Init(&hfdcan) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan, FDCAN_RX_FIFO1, FDCAN_RX_FIFO_OVERWRITE) != HAL_OK)
	{
		Error_Handler();
	}

	//only accept config/request can messages and sync can messages
	/*sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = CANID_CONFIG;
	sFilterConfig.FilterID2 = 0x7FF;
	if (HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}*/ //no config yet

	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 1;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = CANID_SYNC;
	sFilterConfig.FilterID2 = 0x7FF;
	if (HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_FDCAN_Start(&hfdcan) != HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_FDCAN_ActivateNotification(&hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_TIM6_Init(void)
{
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 16999; //100us resolution
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = SendAnalogPeriod;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}

	if (SendAnalogPeriod) { HAL_TIM_Base_Start_IT(&htim6); }
}

static void MX_TIM7_Init(void)
{
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 16999; //100us resolution
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 10000; //1 second
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_Base_Start_IT(&htim7);
}

static void MX_TIM16_Init(void)
{
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 1699; //10us resolution
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = CanSyncDelay;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
	{
		Error_Handler();
	}
}


void Error_Handler(void)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
		HAL_Delay(33);
	}
}


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{ 

}
#endif
