#include "main.h"
#include "stm32g4xx_it.h"


//external type handlers
extern FDCAN_HandleTypeDef hfdcan;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern TIM_HandleTypeDef htim7;

void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_adc1);
}

void DMA1_Channel2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_adc2);
}

void FDCAN1_IT0_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan);
}

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim7);
}


void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
	while (1)
	{

	}
}

void MemManage_Handler(void)
{
	while (1)
	{

	}
}

void BusFault_Handler(void)
{
	while (1)
	{

	}
}

void UsageFault_Handler(void)
{
  while (1)
  {

  }
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
	HAL_IncTick();
}
