#ifndef __INPUTS_H
#define __INPUTS_H

#include "bitmap.h"
#include "stm32f10x_adc.h"
#include "define.h"
void inputs_init(void) ;
void inputs_scan(void) ;

 
 
//extern vu16 AD_Value[MAX_AI_CHANNEL]; 
 
//#if (defined T322AI) || (T38AI8AO6DO)	
//extern uint16_t data_change[MAX_AI_CHANNEL] ;

// void pulse_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) ;
// void pulse_set(uint8_t channel) ;
//#endif




#endif































