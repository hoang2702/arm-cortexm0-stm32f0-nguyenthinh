/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void Delay_10us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
