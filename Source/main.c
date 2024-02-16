/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "Systick_Delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RCC_Config(void);
void GPIO_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/* Init Function */
	SystemCoreClockUpdate();
	RCC_Config();
	GPIO_Config();
	/* User Function */
	/* Infinite loop */
	while(1)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		SysTick_Delay_ms(100);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		SysTick_Delay_ms(100);
	}
}

/**
  * @brief  RCC Config Function
  * @param  None
  * @retval None
  */
void RCC_Config(void)
{
	//Resets the RCC clock configuration to the default reset state
	RCC_DeInit();
	//Disable HSI to reduce energy
	RCC_HSICmd(DISABLE);
	//Enable HSE
	RCC_HSEConfig(RCC_HSE_ON);
	//Wait for HSE oscillator is stable and ready to use
	while(RCC_WaitForHSEStartUp() != SUCCESS);
	//PLL Param
	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 4);
	//Enable PLL
	RCC_PLLCmd(ENABLE);
	//Wait for PLL Clock is Ready to use
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET);
	//Set PLLCLK as SYSCLK
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//HCLK = SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//APB1 = HCLK/4
	RCC_PCLK1Config(RCC_HCLK_Div4);
	//APB2 = HCLK/2
	RCC_PCLK2Config(RCC_HCLK_Div2);
	//Check if PLL Clock used as system clock.
	while(RCC_GetSYSCLKSource() != 0x08);
	//Enable clock for Peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

/**
  * @brief  GPIO Config Function
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Configure LED_Builtin Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
