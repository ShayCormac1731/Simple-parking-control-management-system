#include "ldr.h"


void Clock_Config(void)
{
/* RCC system reset */
RCC_DeInit();
/* HCLK = SYSCLK */
RCC_HCLKConfig(RCC_SYSCLK_Div1);
/* PCLK2 = HCLK */
RCC_PCLK2Config(RCC_HCLK_Div1);
/* PCLK1 = HCLK/2 */
RCC_PCLK1Config(RCC_HCLK_Div1);
/*enable HSI source clock*/
RCC_HSICmd(ENABLE);
/* Wait till PLL is ready */
while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
/* Select HSI as system clock source */
RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
/* Wait till HSI is used as system clock source */
while(RCC_GetSYSCLKSource() != 0x00) {}
}

void ADC_configuration()
{
	
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
	
//cau hinh cho chan GPIO va bo ADC hoat dong
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
RCC_ADCCLKConfig (RCC_PCLK2_Div8);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

/*cau hinh chan Input cua bo ADC1 la chan PA0*/
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
//cau hinh ADC
ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
ADC_InitStructure.ADC_ScanConvMode = DISABLE;
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfChannel = 1;
ADC_Init(ADC1, &ADC_InitStructure);
/* Cau hinh chanel, rank, thoi gian lay mau */
ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
/* Cho phep bo ADC1 hoat dong */
ADC_Cmd(ADC1, ENABLE);
/* Bat dau chuyen doi ADC */

ADC_ResetCalibration(ADC1); // Reset hieu Chuan ADC
while(ADC_GetResetCalibrationStatus(ADC1));

ADC_StartCalibration(ADC1); // Hieu chuan lai
while(ADC_GetCalibrationStatus(ADC1));

ADC_Cmd (ADC1,ENABLE);
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint16_t ldr_read(void){
	uint16_t raw_value, sum_adc, num_percent;
	uint8_t i;
				sum_adc = 0;
				//doc gia tri ADC 10 lan roi lay ket qua
				for(i=0;i<10;i++)
				{
				raw_value = ADC_GetConversionValue(ADC1);
				sum_adc += raw_value;
				delay_ms(10);
				}
		sum_adc /= 10;
		num_percent = (sum_adc*5)/4096;
		
				return num_percent;
}
