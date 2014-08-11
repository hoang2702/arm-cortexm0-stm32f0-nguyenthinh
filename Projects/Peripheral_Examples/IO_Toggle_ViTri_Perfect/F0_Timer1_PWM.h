/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __F0_TIMER1_PWM_H
#define __F0_TIMER1_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void TIM_PWM_Config(void);
void Set_Duty(uint8_t channel, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
