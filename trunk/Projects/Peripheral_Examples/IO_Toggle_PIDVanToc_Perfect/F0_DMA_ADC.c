#include "F0_DMA_ADC.h"

volatile uint16_t ADCConvertedValues[1];

void ADCgo(void)
{
    ADC_InitTypeDef         ADC_InitStructure;
    GPIO_InitTypeDef        GPIO_InitStructure;
    DMA_InitTypeDef         DMA_InitStructure;
 
    /* DMA1 clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* TIM15 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15 , ENABLE);
    /* GPIOC Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
 
    /* Configure ADC Channel0 and 1 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* DMA1 Channel1 Config */
    DMA_DeInit(DMA1_Channel1);
		
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValues;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		
    /* DMA1 Channel1 enable */
    DMA_Cmd(DMA1_Channel1, ENABLE);
		/* ADC DMA request in circular mode */
		ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
		/* Enable ADC_DMA */
		ADC_DMACmd(ADC1, ENABLE);
		
    /* Enable DMA1 Channel1 Transfer Complete interrupt */
    //DMA_ITConfig(DMA1_Channel1, DMA_IT_TC);
//===================================================================
		/* ADCs DeInit */
    ADC_DeInit(ADC1);

    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);
 
    /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);
 
    /* Convert the ADC1 Channel 0 with 13.5 Cycles as sampling time */
    ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_13_5Cycles);     // microphone
 
    //ADC_DiscModeCmd(ADC1, DISABLE);
    /* Set synchronous clock mode */
    //ADC_JitterCmd(ADC1, ADC_JitterOff_PCLKDiv2, ENABLE);
 
    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);
 
		/* Enable DMA request after last transfer (Single-ADC mode) */
		//ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
		/* Wait the ADCEN flag */
//		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
//		{
//			
//		}
    
		/* Enable ADCperipheral */
    ADC_Cmd(ADC1, ENABLE);
		
		/* ADC1 regular Software Start Conv */
		ADC_StartOfConversion(ADC1);
    
// 
//    /* ADC1 regular Software Start Conv */
//    ADC_StartOfConversion(ADC1);
}

/**
  * @brief  ADC1 channel with DMA configuration
  * @param  None
  * @retval None
  */
void ADC_CH_DMA_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
  DMA_InitTypeDef     DMA_InitStructure;
  
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValues;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 
 
  /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_55_5Cycles);  
  ADC_TempSensorCmd(ENABLE);
  
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
}
