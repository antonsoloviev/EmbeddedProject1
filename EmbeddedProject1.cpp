#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void RCC_init(void)
{
	//Enable external clocking source
 RCC->CR |= ((uint32_t)RCC_CR_HSEON);              // Enable HSE
 while(!(RCC->CR & RCC_CR_HSERDY));              // Wait until HSE is ready
 
 //Flash memory
 FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;      // Set Clock Flash memory
 
 //HSE bus divider and multiplier
 RCC->CFGR &= ~RCC_CFGR_PLLSRC;                // Clearn PLLSRC bits (not necessarily)
 RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;               // Clearn PLLXTPRE bits (not necessarily)
 RCC->CFGR &= ~RCC_CFGR_PLLMULL;                       // Clear PLLMULL bits (not necessarily)
 RCC->CFGR |= RCC_CFGR_PLLSRC_Msk;               // Set cloacking source HSE
 RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;              // Set HSE source divider 1: 8/1 = 8 MHz
 RCC->CFGR |= RCC_CFGR_PLLMULL9;                // Set HSE source multiplier 9: 8*9 = 72 MHz (max)
 
 //AHB and APB bus divider
 RCC->CFGR |= RCC_CFGR_HPRE_DIV1;               // Set AHB divider SYSCLK/1: 72/1 = 72MHz (max)
 RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;               // Set APB1 divider HCLK/2: 72/2=36MHz (max)
 RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;               // Set APB2 divider HCLK/1: 72/1=72MHz (max)
 
 //Enable PLL which was configured at "Set HSE bus divider and multiplier"
 RCC->CR |= RCC_CR_PLLON;                              // Enable PLL
 while((RCC->CR & RCC_CR_PLLRDY) == 0) {}             // Wait until PLL is ready
 
 //Switch system clock mux (set source)
 RCC->CFGR &= ~RCC_CFGR_SW;                             // Clear SW bits
  RCC->CFGR |= RCC_CFGR_SW_PLL;                         // Select source SYSCLK = PLL
 while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {}    // Wait utill PLL is used
}

int main(void)
{
	HAL_Init();
	RCC_init();

	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_13;

	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	for (;;)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(500);
	}
}
