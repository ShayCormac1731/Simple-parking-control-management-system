#ifndef	__LDR_H
#define __LDR_H

#include "stm32f10x.h"
#include "delay.h"
#include "stm32f10x_adc.h"

void Clock_Config(void);
void ADC_configuration(void);
uint16_t ldr_read(void);

#endif
