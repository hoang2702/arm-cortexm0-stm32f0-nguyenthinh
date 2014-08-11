/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __F0_DMA_ADC_H
#define __F0_DMA_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void ADCgo(void);
void ADC_CH_DMA_Config(void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
