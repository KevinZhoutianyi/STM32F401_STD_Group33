#define __ADC_GLOBAL

#include "adc.h"
#include "delay.h"		
#include "usart.h"

													   
void  Adc_Init(int mode)
{  
	if(mode == SINGLEMODE)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef       ADC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);  
	 
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	 
	 
		
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInit(&ADC_CommonInitStructure);
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC_Init(ADC1, &ADC_InitStructure);
		ADC1->CR2 &= ~(1<<8); //dma fail to turn down by the library function, here to turn it down forcely

		
	 
		ADC_Cmd(ADC1, ENABLE);
	}
	else if(mode == SCANMODE)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef       ADC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
				
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
		
	 
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	 
	 
		
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;            //????????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;         //???????
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //???????TIM2
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//?????
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;         //12????
    
    ADC_Init(ADC1, &ADC_InitStructure);
		
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC1->CR2 &= ~(1<<8); //dma fail to turn down by the library function, here to turn it down forcely
		ADC_Init(ADC1, &ADC_InitStructure);
		
	 
		ADC_Cmd(ADC1, ENABLE);
	}
}				  
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת���������

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

void MySingleAdcRead()
{
	u16 adcx;
	float temp;

	adcx=Get_Adc_Average(0,20);//???? 5 ????,20 ????

	temp=(float)adcx*(3.3/4096); //???????????????,?? 3.1111
	printf("%f \n",temp);
	delay_ms(250); 
		
}
	 









