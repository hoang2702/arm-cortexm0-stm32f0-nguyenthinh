/**
  ******************************************************************************
  * @file    TIM_PWM_Output/main.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-October-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <stdio.h>

/** @addtogroup STM32F0308_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel2Pulse = 0, Channel3Pulse = 0;

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
/* Private function prototypes -----------------------------------------------*/
static void TIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* TIM Configuration */
  TIM_Config();
	
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  
	/* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
	
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
  /* Infinite loop */
	
	printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
	
  while (1)
  {}
}

/**
  * @brief  Configure the TIM1 Pins.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
  
  /* TIM1 Configuration ---------------------------------------------------
   Generate PWM signals with 4 different duty cycles:
   TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock is set to 48 MHz for STM32F0xx devices
   
   The objective is to generate 4 PWM signal at 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
   
   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 17570 ) - 1;
  /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 */
  Channel2Pulse = (uint16_t) (((uint32_t) 500 * (TimerPeriod - 1)) / 1000);
  /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 */
  Channel3Pulse = (uint16_t) (((uint32_t) 50 * (TimerPeriod - 1)) / 100);

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2, 3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;


  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);


  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
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
