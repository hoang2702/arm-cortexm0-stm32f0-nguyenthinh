#include "stm32f0xx.h"
//#include "stm32f0xx_gpio.h"
//#include "stm32f0xx_rcc.h"
//#include "stm32f0xx_usart.h"
#include "stdio.h"
  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); // USART1 TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
}
  
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  
  USART_Cmd(USART1,ENABLE);
}
  
int main(void)
{
    GPIO_Configuration();
    USART_Configuration();
		
		USART_SendData(USART1, 'A');
		USART_SendData(USART1, 'B');
		USART_SendData(USART1, 'C');
		USART_SendData(USART1, 'D');
    while(1)
    {
        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        //USART_SendData(USART1, '1');
				puts("Nguyen Xuan Thinh \n");
    }
}