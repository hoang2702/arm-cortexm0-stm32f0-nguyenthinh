/**
  ******************************************************************************
  * @file    ---- 
  * @author  TT Team
  * @version V1.0
  * @date    06-Jun-2013
  * @brief   Mini Servo Board
  ******************************************************************************
  * @attention
  * Noi dung chuong trinh:
	* - GPIO Output chan:
			+ PA0: ----- RE
			+ PA1: ----- DE
		- UASRT:
			+ PA2: ----- TX ----- DI
			+ PA3: ----- RX ----- RO
		- PWM dung Timer 1:
			+ PA10:----- PWM1
			+ PA9: ----- PWM2
		- Encoder Interface:
			+ PA6: ----- Kenh A
			+ PA7: ----- Kenh B
		- ADC Feedback Current
			+ PA5: ----- Feedback current
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "main.h"
#include "F0_USART.h"
#include "F0_Timer1_PWM.h"
#include "F0_Timer3_EncoderInterface.h"
#include "F0_Timer14.h"
#include "F0_DMA_ADC.h"
#include "pid.h"
/** @addtogroup STM32F0308_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile uint16_t ADCConvertedValues[1];
static __IO uint32_t TimingDelay;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
	
void Delay_10us(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void SystemTick(void)
{
	SysTick_Config(4800);//SystemCoreClock / 1000000); // Delay 10us
}


void GPIO_Debug(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void GPIO_config(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SystemTick();
	GPIO_Debug();
	GPIO_config();
	//USART_Configuration();
	USART_Config();
	TIM_PWM_Config();
	Encoder_Config();
	
	//ADCgo();
	ADC_CH_DMA_Config();
	 // channel 3 ok
	printf("Nguyen Thinh");
	
	TIM16_Config();
	
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	
	PID_Init();
	PID_(&PIDPos_X,1,334*4);
  while (1)
  {
//		GPIO_ResetBits(GPIOA,GPIO_Pin_9);
//		TIM_SetCompare3(TIM1,1000);
		/*			printf("Nguyen Thinh \n");
			TIM1->CCR3 = (uint32_t)1000;
			//TIM_SetCompare3(TIM1,1000);
			GPIO_SetBits(GPIOA,GPIO_Pin_9);
			Delay_10us(10000);
			printf("off \n");
			TIM1->CCR3 = (uint32_t)4998;
			//TIM_SetCompare3(TIM1,4998);
			
			Delay_10us(10000);
			printf("on \n");
		//printf("Encoder: %d \n",Get_Encoder());
		//Delay_10us(10000);
		*/
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
