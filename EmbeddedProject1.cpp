#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>
#include <stm32f1xx.h>
#include <Debounce.h>

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
 FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;      // Set Clock Flash memory
 
 //HSE bus divider and multiplier
 RCC->CFGR &= ~RCC_CFGR_PLLSRC;                // Clear PLLSRC bits (not necessarily)
 RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;               // Clear PLLXTPRE bits (not necessarily)
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


Debounce button(GPIOB, 1 << 12, 10);  // экземпл€р класса Debounce







int main(void)
{
	HAL_Init();
	RCC_init();
/*	
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB->CRH &=  ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE12);
	GPIOB->CRH |= (0b10 << GPIO_CRH_CNF12_Pos) | (0b00 << GPIO_CRH_MODE12_Pos); // B12 режим входа с подт€гивающим резистором к шине питани€
	GPIOB->BSRR = GPIO_BSRR_BS12; // B12 устанавливаем в 1 
	
	GPIOB->CRH &=  ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);  
	GPIOB->CRH |= (0b00 << GPIO_CRH_CNF13_Pos) | (0b10 << GPIO_CRH_MODE13_Pos);   // B13 режим активного выхода
*/

	__GPIOC_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* конфигураци€ вывода PC13 на выход */
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* конфигураци€ вывода PB13 на активный выход */
	GPIO_InitStructure.Pin = GPIO_PIN_13;  // номер вывода
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // режим выход
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;  // средн€€ скорость выхода
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* конфигураци€ вывода PB12 на вход с подт€гивающим резистором */
	GPIO_InitStructure.Pin = GPIO_PIN_12;  // номер вывода
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;   // режим вход
	GPIO_InitStructure.Pull = GPIO_PULLUP;   // резистор к шине питани€
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	for (;;)
	{
		/*
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(500);
		*/
		
		if (button.readFlagFalling() != 0) {
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
		}

		/*
			  if(button.readFlagLow() != 0) {
				  // кнопка нажата
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); // сброс вывода PB13
			  }
			  else {
				  // кнопка отжата
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // установка вывода PB13
			  }
			  */

		button.scanAverage();
		// button.scanStability();
		HAL_Delay(1);
		
		/*
		if ((GPIOB->IDR&GPIO_IDR_IDR12) == 0)
		{
			GPIOB->BSRR = GPIO_BSRR_BR13;
		}
		else
		{
			GPIOB->BSRR = GPIO_BSRR_BS13;
		}
		*/
		
		/*
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET) 
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                        
		}
		else 
		{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                            
		}
		*/
	}
}
