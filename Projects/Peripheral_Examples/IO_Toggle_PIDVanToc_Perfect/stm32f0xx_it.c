/**
  ******************************************************************************
  * @file    IO_Toggle/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-October-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f0xx_it.h"
#include "main.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "F0_Timer3_EncoderInterface.h"
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
#define Resolution 				1336
#define Sample_time 			0.01
#define Sample_fre 	100
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private UART variables -------------------------------------------------------------*/
int UART_count=0;
char UART_chuoi[8];
int8_t RS232_data=0;
/* Private ADC ---------------------------------------------------------*/

extern volatile uint16_t ADCConvertedValues[1];
/* Private Variables PID ---------------------------------------------------------*/
uint16_t dem=0;
uint16_t dem1=0;
int8_t speed=0;
uint16_t abc=0;
int16_t EnCrr=0;
/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}


//void TIM2_IRQn(void)
//{
//	
//}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
		TimingDelay_Decrement();
}

// tan so 8KHz
void TIM16_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
		
		dem++;
		dem1++;
		if(dem==80)
		{
			dem=0;
			speed=Get_Encoder()*Sample_fre/Resolution;
			PID_VanToc();
			//PID_ViTri();
			
			
		}
		if(dem1==800)
		{
			dem1=0;
			//putchar((uint8_t)PIDPos_X.e[2]);
			printf("%d \t",PIDVel_X.Direct);
			printf("%d \t",speed);
			printf("%d \t",(int16_t)PIDVel_X.e[2]);
			printf("%d \t",PIDVel_X.set_point);
			printf("%d \n",(int16_t)PIDVel_X.control[1]);
			//putchar(speed);
			
		}
		
  }
}

void USART1_IRQHandler(void)
{
	char c=0;
	//========
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET) 
	{
		c=USART_ReceiveData(USART1);
		if (c!='e')
	 {
			if (UART_count==0)
			{
				 memset(UART_chuoi, '\0', sizeof(UART_chuoi));
			}
			UART_chuoi[UART_count]=c;
			UART_count++;
	 }
	 else
   {
      UART_count=0;
			//printf("So gui: %d \n",atoi(UART_chuoi));
		 RS232_data=atoi(UART_chuoi);
		 if(RS232_data>=0)
			 PID_(&PIDVel_X,1,RS232_data);
		 else
			 PID_(&PIDVel_X,-1,RS232_data);
   }

		//USART_SendData(USART2, 'A');
		//puts("Da Nhan");
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0x8.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
