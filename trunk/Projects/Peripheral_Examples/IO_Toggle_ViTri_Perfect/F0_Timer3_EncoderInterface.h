/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __F0_TIMER3_ENCODERINTERFACE_H
#define __F0_TIMER3_ENCODERINTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void Reset_Encoder(void);
int16_t Get_Encoder(void);
void Encoder_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
