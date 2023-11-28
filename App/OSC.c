#include "ff.h"
#include "fatfs.h"
#include "main.h"
#include "usb_device.h"
#include "math.h"
#include "usbd_cdc_if.h"
#include "stm32f3xx_hal_dma.h"
#include "stm32f3xx_hal.h"
#include "Recursos.h"
#include "ILI9341_Driver.h"
#include "Panel Tactil.h"
#include "USB_device.h"

unsigned char BufferRX[100];
unsigned char Canal;

uint32_t OSCCanal[4100];
uint32_t* OSCCanal1;
uint32_t* OSCCanal2;
uint32_t* OSCCanal3;
uint32_t* OSCCanal4;

uint32_t m_FV[5];
uint32_t m_PosV[5];

uint32_t m_PosH;
uint32_t m_BTO;
uint32_t m_BTG;

uint32_t m_Disparo;
uint32_t m_NivelDisparoL;
uint32_t m_NivelDisparoH;
uint32_t m_Flanco;
uint32_t m_Demora;
uint32_t m_NroTrama;
uint32_t m_AcopAC;

void SetBT(void);
void TomarCanales(void);



void TomarCanales(void)
{
	unsigned int i;

	//uint8_t CAN_data[8]={0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    //Can_TxMessage(0,0x555,8,CAN_data);

	HAL_ADC_Start_DMA(&hadc1,OSCCanal1,1024);
	HAL_ADC_Start_DMA(&hadc2,OSCCanal2,1024);
    HAL_ADC_Start_DMA(&hadc3,OSCCanal3,1024);
    HAL_ADC_Start_DMA(&hadc4,OSCCanal4,1024);

    while(hadc2.Instance->DR>1500);
    while(hadc2.Instance->DR<1500);

    //__disable_irq();

    (hadc1.DMA_Handle)->Instance->CCR &= ~DMA_CCR_EN; // disable
	(hadc2.DMA_Handle)->Instance->CCR &= ~DMA_CCR_EN; // disable
	(hadc3.DMA_Handle)->Instance->CCR &= ~DMA_CCR_EN; // disable
	(hadc4.DMA_Handle)->Instance->CCR &= ~DMA_CCR_EN; // disable

	(hadc1.DMA_Handle)->Instance->CNDTR=1024;
	(hadc2.DMA_Handle)->Instance->CNDTR=1024;
	(hadc3.DMA_Handle)->Instance->CNDTR=1024;
	(hadc4.DMA_Handle)->Instance->CNDTR=1024;

	(hadc1.DMA_Handle)->Instance->CCR |= DMA_CCR_EN; // re-enable
	(hadc2.DMA_Handle)->Instance->CCR |= DMA_CCR_EN; // re-enable
	(hadc3.DMA_Handle)->Instance->CCR |= DMA_CCR_EN; // re-enable
	(hadc4.DMA_Handle)->Instance->CCR |= DMA_CCR_EN; // re-enable

	//__enable_irq();

	do
	{
		i=__HAL_DMA_GET_COUNTER(hadc4.DMA_Handle);

 	}while(i>1);
 	HAL_Delay(10);

 	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
 	HAL_ADC_Stop_DMA(&hadc3);
 	HAL_ADC_Stop_DMA(&hadc4);

 	if(m_BTO<3)
 	for(i=0;i<1000;i++)
    {
		OSCCanal1[i]=OSCCanal1[(int)(i*1.024)];
		OSCCanal2[i]=OSCCanal2[(int)(i*1.024)];
	    OSCCanal3[i]=OSCCanal3[(int)(i*1.024)];
	    OSCCanal4[i]=OSCCanal4[(int)(i*1.024)];
    }
}

void SetBT()
{
	switch(m_BTO)
  	{
		  	  		case 0://5us
			  		hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
			  		hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
			  		hadc1.Init.ExternalTrigConv= ADC_SOFTWARE_START;
			  		hadc1.Init.ContinuousConvMode = ENABLE;
			  		hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc1);
		  	  		hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  		hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc2.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc2.Init.ContinuousConvMode = ENABLE;
		  	  		hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc2);
		  	  		hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  		hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc3.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc3.Init.ContinuousConvMode = ENABLE;
		  	  		hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc3);
		  	  		hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  		hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc4.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc4.Init.ContinuousConvMode = ENABLE;
		  	  		hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  		case 1://10us
			  		hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV2;
			  		hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
			  		hadc1.Init.ExternalTrigConv= ADC_SOFTWARE_START;
			  		hadc1.Init.ContinuousConvMode = ENABLE;
			  		hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc1);
		  	  		hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV2;
		  	  		hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc2.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc2.Init.ContinuousConvMode = ENABLE;
		  	  		hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc2);
		  	  		hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV2;
		  	  		hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc3.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc3.Init.ContinuousConvMode = ENABLE;
		  	  		hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc3);
		  	  		hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV2;
		  	  		hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc4.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc4.Init.ContinuousConvMode = ENABLE;
		  	  		hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  		case 2://20us
			  		hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV4;
			  		hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
			  		hadc1.Init.ExternalTrigConv= ADC_SOFTWARE_START;
			  		hadc1.Init.ContinuousConvMode = ENABLE;
			  		hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc1);
		  	  		hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV4;
		  	  		hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc2.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc2.Init.ContinuousConvMode = ENABLE;
		  	  		hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc2);
		  	  		hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV4;
		  	  		hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc3.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc3.Init.ContinuousConvMode = ENABLE;
		  	  		hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc3);
		  	  		hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV4;
		  	  		hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  	  		hadc4.Init.ExternalTrigConv= ADC_SOFTWARE_START;
		  	  		hadc4.Init.ContinuousConvMode = ENABLE;
		  	  		hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  		HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 3://50us
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 0;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 72-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);
		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 4://100us
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 0;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 144-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests =ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 5://200us
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 0;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 288-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests =ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests =ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 6://.5ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 10-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 7://1ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 20-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 8://2ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 40-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 9://5ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 100-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);

		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);

		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);

		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 10://10ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 200-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 11://20ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 400-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 12://50ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 1000-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 13://100ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 2000-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

		  	  	    case 14://200ms
	    	  	   	htim3.Instance = TIM3;
	    	  	    htim3.Init.Prescaler = 72-1;
	    	  	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  	  	    htim3.Init.Period = 4000-1;
		  	  	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  	  	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		  	  	    HAL_TIM_Base_Init(&htim3);

		  	  	    hadc1.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc1.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc1.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc1);
		  	  	    hadc2.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc2.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc2.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc2);
		  	  	    hadc3.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc3.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc3.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc3);
		  	  	    hadc4.Init.ClockPrescaler  = ADC_CLOCK_SYNC_PCLK_DIV1;
		  	  	    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
		  	  	    hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
		  	  	    hadc4.Init.ContinuousConvMode = DISABLE;
		  	  	    hadc4.Init.DMAContinuousRequests = ENABLE;
		  	  	    HAL_ADC_Init(&hadc4);
		  	  	    break;

  	}
}


