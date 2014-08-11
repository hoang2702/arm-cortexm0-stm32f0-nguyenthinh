/**
  ******************************************************************************
  * @file    pid.c 
  * @author  Robot Club BK HCM
  * @version
  * @date
  * @brief	Main template body
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "stdlib.h"
#include "F0_Timer3_EncoderInterface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern int8_t speed;
extern volatile int mode;
// PID Constants
K_PID PIDVel_X;
K_PID PIDPos_X;

float P_part = 0, I_part = 0, D_part = 0;
float P_part1 = 0, I_part1 = 0, D_part1 = 0;
float PposX_part = 0, IposX_part = 0, DposX_part = 0;
float PposY_part = 0, IposY_part = 0, DposY_part = 0;
float distance = 0;
float response_stepx = 0;
float response_stepy = 0;
float max_resX = 0;
float max_resY = 0;

// PID Profile variables ===============
float s0=0;
int32_t a1=0, a2=0;
int32_t Timer_Rise=0, Timer_Fall=0;
uint32_t Timer_V=0;
int32_t Vmax=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: PID_Init
Decription: Khoi tao gia tri ban dau cho cac he so PID
*******************************************************************************/

void PID_Init(void)
{
    // X
    PIDVel_X.e[0] = PIDVel_X.e[1] = PIDVel_X.e[2] = 0;
    PIDVel_X.MAX_Control = 5998;
    PIDVel_X.pre_set_point = 0;
    PIDVel_X.T0 = 1;
    PIDVel_X.Kp = 10;
    PIDVel_X.Ki = 10;//10; >10 => rung 
    PIDVel_X.Kd = 40;//40; >40 => tang thoi gian dap ung
    
    PIDPos_X.e[0] = PIDPos_X.e[1] = PIDPos_X.e[2] = 0;
    PIDPos_X.MAX_Control = 5998;
    PIDPos_X.pre_set_point = 0;
    PIDPos_X.set_point = 30;
    PIDPos_X.T0 = 1;
    PIDPos_X.Kp = 7; // 800
    PIDPos_X.Ki = 1;// 0.5
    PIDPos_X.Kd = 15;  // 20
}

/*
void PID_Init(void)
{
    // X
    PIDVel_X.e[0] = PIDVel_X.e[1] = PIDVel_X.e[2] = 0;
    PIDVel_X.MAX_Control = 5998;
    PIDVel_X.pre_set_point = 0;
    PIDVel_X.T0 = 1;
    PIDVel_X.Kp = 0.005;
    PIDVel_X.Ki = 0.008;
    PIDVel_X.Kd = 0.01;
    
    PIDPos_X.e[0] = PIDPos_X.e[1] = PIDPos_X.e[2] = 0;
    PIDPos_X.MAX_Control = 5998;
    PIDPos_X.pre_set_point = 0;
    PIDPos_X.set_point = 0;
    PIDPos_X.T0 = 1;
    PIDPos_X.Kp = 1000;
    PIDPos_X.Ki = 0.07;
    PIDPos_X.Kd = 100;  
}
*/ 
/*******************************************************************************
Function name: PIDControl
Decription: Ham lay mau va thuc hien PID, dat trong timer systick
*******************************************************************************/
void PIDControl(void)
{ 
	  // Calculate velocity
  	PIDVel_X.cur_value = speed; // MAX
    
    // Calculate positon
    PIDPos_X.cur_value += Get_Encoder();
    
    /*********************************************************************************************************
    ************** PID VEL
    *********************************************************************************************************/
    if (PIDVel_X.Enable)    // X axis
    {      
        PIDVel_X.e[2] = PIDVel_X.set_point - PIDVel_X.cur_value;
        
        P_part = (float)PIDVel_X.Kp*PIDVel_X.e[2];
        I_part = (float)PIDVel_X.Ki*(PIDVel_X.e[2] + PIDVel_X.e[1]);
        D_part = (float)PIDVel_X.Kd*(PIDVel_X.e[2] - 2*PIDVel_X.e[1]+ PIDVel_X.e[0]);
        
        if (I_part > 3000) I_part = 3000;
        if (I_part < -3000) I_part = -3000;
        
        PIDVel_X.control[1] += P_part + I_part + D_part;
        PIDVel_X.control[1] = PIDVel_X.control[1];
        
        if (PIDVel_X.control[1] > PIDVel_X.MAX_Control) PIDVel_X.control[1] = PIDVel_X.MAX_Control;
        if (PIDVel_X.control[1] < -PIDVel_X.MAX_Control) PIDVel_X.control[1] = -PIDVel_X.MAX_Control;
        
        // Backup
        PIDVel_X.control[0] = PIDVel_X.control[1];
        PIDVel_X.e[1] = PIDVel_X.e[2];
        PIDVel_X.e[0] = PIDVel_X.e[1];
        
				TIM_SetCompare3(TIM1,(5998-(int16_t)PIDVel_X.control[1]));
    }
    /*********************************************************************************************************
    ************** PID POS
    *********************************************************************************************************/
    // PID Position
    if (PIDPos_X.Enable)    // X axis
    {
        if (PIDPos_X.pre_set_point != PIDPos_X.set_point)
        {
            distance = abs(PIDPos_X.cur_value - PIDPos_X.set_point);
            PIDPos_X.pre_set_point = PIDPos_X.set_point;
            max_resX = 0;
        }
        
        PIDPos_X.e[2] = PIDPos_X.set_point - PIDPos_X.cur_value;
        
        PposX_part = PIDPos_X.Kp*(PIDPos_X.e[2])*0.001;
        IposX_part = IposX_part + PIDPos_X.Ki*(PIDPos_X.e[2] + PIDPos_X.e[1])*0.001;
        //DposX_part = PIDPos_X.Kd*(PIDVel_Y.e[2] - PIDVel_Y.e[1])*0.001;
        
        PIDPos_X.control[1] = PposX_part + IposX_part + DposX_part;
        
        if (abs(PIDPos_X.e[2]) < 200)
        {
            max_resX = response_stepx * abs(PIDPos_X.e[2]) / 200;
            
            if (max_resX < 50)  max_resX = 50;
        }
        else 
        {
            max_resX += 2;
            response_stepx = max_resX;
            
            if (max_resX > PIDPos_X.MAX_Control)
                max_resX = PIDPos_X.MAX_Control;
        }
        
        if (IposX_part > 200) IposX_part = 200;
        if (IposX_part < -200) IposX_part = -200;
        
        if (abs(PIDPos_X.e[2]) <= 1)   
        {
            IposX_part = 0;
            PIDPos_X.control[1] = 0;
        }
        
        if (PIDPos_X.control[1] > max_resX) PIDPos_X.control[1] = max_resX;
        if (PIDPos_X.control[1] < -max_resX) PIDPos_X.control[1] = -max_resX;
        
        // Backup
        PIDPos_X.control[0] = PIDPos_X.control[1];
        PIDPos_X.e[1] = PIDPos_X.e[2];
        PIDPos_X.e[0] = PIDPos_X.e[1];
        
        PID_(&PIDVel_X, 1, PIDPos_X.control[1]);
    }
    // Reset encoder
	  Reset_Encoder();
}

void PID_(K_PID *PID, uint8_t enable, int32_t set_point)
{
    if (enable) // PID Control
    {
        (*PID).set_point = set_point;
        (*PID).Enable = 1;
    }
    else        // PID release
    {
        (*PID).Enable = 0;
    }
}

        
void PID_Profile(int32_t t1, int32_t Vr, int32_t t2, int32_t s)
{
	int32_t Vc = 0;
	Vc = 2*s/(t1+t2);
	if(Vc>=Vr)
	{
		a1=Vr/t1;
		a2=-Vr/t2;
		if(Vc>Vr)
			Timer_V=(uint32_t)((s-Vr*(t1+t2)/2)/Vr/0.05);
		else
			Timer_V=0;
		Vmax=Vr;
	}
	else
	{
		a1=Vc/t1;
		a2=-Vc/t2;
		Timer_V=0;
		Vmax=Vc;
	}
	Timer_Rise = (int32_t)(t1/0.05);
	Timer_Fall = (int32_t)(t2/0.05);
	mode=1;
}
void PID_ViTri(void)
{
		/*********************************************************************************************************
    ************** PID POS
    *********************************************************************************************************/
		// PID Position
		// Calculate positon
		speed=Get_Encoder();
    PIDPos_X.cur_value += speed;
		
		PIDPos_X.e[2] = PIDPos_X.set_point - PIDPos_X.cur_value;
		
		P_part = (float)PIDPos_X.Kp*PIDPos_X.e[2];
		I_part += (float)PIDPos_X.Ki*PIDPos_X.e[2];// + PIDPos_X.e[1]);
		D_part = (float)PIDPos_X.Kd*(PIDPos_X.e[2] - PIDPos_X.e[1]);//+ PIDPos_X.e[0]);
		
		if (I_part > 4000) I_part = 4000;
		if (I_part < -4000) I_part = -4000;
		
		PIDPos_X.control[1] = P_part + I_part + D_part;
		//=========================================
		if (PIDPos_X.control[1] >= 0) 
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_9);
			if (PIDPos_X.control[1] > PIDPos_X.MAX_Control) 
				PIDPos_X.control[1] = PIDPos_X.MAX_Control;
			TIM_SetCompare3(TIM1,(5998-(int16_t)PIDPos_X.control[1]));
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_9);
			if (PIDPos_X.control[1] < -PIDPos_X.MAX_Control) 
				PIDPos_X.control[1] = -PIDPos_X.MAX_Control;
			TIM_SetCompare3(TIM1,-(int16_t)PIDPos_X.control[1]);
			
		}
		
		// Backup
		PIDPos_X.control[0] = PIDPos_X.control[1];
		PIDPos_X.e[1] = PIDPos_X.e[2];
		PIDPos_X.e[0] = PIDPos_X.e[1];
		
    // Reset encoder
	  Reset_Encoder();
}

/**
		P_part = Kp*(Error - pre_Error);
		I_part = 0.5*Ki*T*(Error + pre_Error);
		D_part = Kd/T*( Error - 2*pre_Error+ pre_pre_Error);
		Out = pre_out + P_part + I_part + D_part ;
		pre_pre_Error = pre_Error
		pre_Error = Error
		pre_Out = Out
*/
/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/
