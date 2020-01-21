#include "main.h"


//externs
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;

void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	LL_PWR_DisableDeadBatteryPD();
}

//counter to keep track of how many peripherals require ADC12 clock, so that deinitializing ADC1 allows ADC2 to run
static uint32_t HAL_RCC_ADC12_CLK_ENABLED=0;

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hadc->Instance==ADC1)
	{
		HAL_RCC_ADC12_CLK_ENABLED++;
		if(HAL_RCC_ADC12_CLK_ENABLED==1)
		{
			__HAL_RCC_ADC12_CLK_ENABLE();
		}
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		hdma_adc1.Instance = DMA1_Channel1;
		hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc1.Init.Mode = DMA_CIRCULAR;
		hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);
	}
	else if(hadc->Instance==ADC2)
	{
		HAL_RCC_ADC12_CLK_ENABLED++;
		if(HAL_RCC_ADC12_CLK_ENABLED==1)
		{
		  __HAL_RCC_ADC12_CLK_ENABLE();
		}
		__HAL_RCC_GPIOF_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		hdma_adc2.Instance = DMA1_Channel2;
		hdma_adc2.Init.Request = DMA_REQUEST_ADC2;
		hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc2.Init.Mode = DMA_CIRCULAR;
		hdma_adc2.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
		{
		  Error_Handler();
		}

		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adc2);
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance==ADC1)
	{
		HAL_RCC_ADC12_CLK_ENABLED--;
		if(HAL_RCC_ADC12_CLK_ENABLED==0)
		{
		  __HAL_RCC_ADC12_CLK_DISABLE();
		}

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

		HAL_DMA_DeInit(hadc->DMA_Handle);
	}
	else if(hadc->Instance==ADC2)
	{
		HAL_RCC_ADC12_CLK_ENABLED--;
		if(HAL_RCC_ADC12_CLK_ENABLED==0)
		{
			__HAL_RCC_ADC12_CLK_DISABLE();
		}

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

		HAL_DMA_DeInit(hadc->DMA_Handle);
	}
}


void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hfdcan->Instance==FDCAN1)
  {
    __HAL_RCC_FDCAN_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan)
{
  if(hfdcan->Instance==FDCAN1)
  {
    __HAL_RCC_FDCAN_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	/*if(htim_base->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
	else*/ if(htim_base->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}
	/*else if(htim_base->Instance==TIM16)
	{
		__HAL_RCC_TIM16_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}*/
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	/*if(htim_base->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
	else*/ if(htim_base->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
	}
	/*else if(htim_base->Instance==TIM16)
	{
		__HAL_RCC_TIM16_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
	}*/
}
