#ifndef __ADC_H
#define __ADC_H


#include "sys.h"
#define SINGLEMODE 1
#define SCANMODE 2

void  Adc_Init(int mode);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
void MySingleAdcRead();

#ifdef  __ADC_GLOBAL
#define __ADC_EXT  
#else
#define __ADC_EXT extern
#endif


__ADC_EXT uint16_t sample_1[128];
__ADC_EXT uint16_t sample_2[128];
__ADC_EXT uint16_t sample_3[128];
__ADC_EXT uint16_t sample_4[128];
__ADC_EXT uint16_t sample_5[128];
__ADC_EXT uint16_t sample_6[128];
__ADC_EXT uint16_t adc_dma_tab[6];
__ADC_EXT uint8_t sample_finish;
__ADC_EXT uint8_t sample_index;



#endif