/**
  ******************************************************************************
  * @file    pid.h 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__PID_H
#define __PID_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Private define ------------------------------------------------------------*/
typedef struct {
	float Kp;
	float Ki;
    float Kd;
    int32_t set_point;
    int32_t pre_set_point;
    int32_t cur_value;
    float a0;
    float a1;
    float a2;
    int32_t e[3];
    uint8_t Enable;
    float control[2];
    int32_t MAX_Control;
    uint8_t T0;
} K_PID;

/* Extern variables ----------------------------------------------------------*/
extern int32_t v_cur, p_cur;
extern uint8_t s_flag, sp_flag;
extern K_PID PIDVel_X;
extern K_PID PIDPos_X;
/* Private function prototypes -----------------------------------------------*/
void PID_Init(void);
void PIDControl(void);
void PID_Position(K_PID *PID, uint8_t enable, int32_t p_set);
void PID_(K_PID *PID, uint8_t enable, int32_t set_point);
void PID_All(void);
void PID_VanToc(void);
void PID_ViTri(void);
void PID_Profile(int32_t t1, int32_t Vr, int32_t t2, int32_t s);
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
