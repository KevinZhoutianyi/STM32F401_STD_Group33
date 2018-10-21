#define __ADC_GLOBAL

#include "adc.h"
#include "delay.h"		
#include "usart.h"




													   
void  Adc_Init(int mode)
{  
	
	

	sample_finish = 0;
	sample_index = 0;
	
	if(mode == SINGLEMODE)
	{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 

  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC1->CR2 = ((uint32_t)0x0D0D0001) ;
	
	}
	else if(mode == SCANMODE)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		DMA_InitTypeDef dma_init_structure;
    NVIC_InitTypeDef nvic_init_structure;
 

		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
		
//----------------------------------------------------------------------------------------------------------------------------

				
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
		
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	 
	 /*
		ADC_DeInit();
		
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

		
		*/
		/*
		    ADC_InitTypeDef adc_init_structure;
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);            //??ADC??
 
    ADC_DeInit(ADC1);                                               //??ADC
    ADC_StructInit(&adc_init_structure);                            //???ADC???
 
    adc_init_structure.ADC_ContinuousConvMode = DISABLE;            //????????
    adc_init_structure.ADC_DataAlign = ADC_DataAlign_Right;         //???????
    adc_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //???????TIM2
    adc_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//?????
    adc_init_structure.ADC_Resolution = ADC_Resolution_12b;         //12????
    adc_init_structure.ADC_ScanDirection = ADC_ScanDirection_Upward;//????0-18??
    ADC_Init(ADC1, &adc_init_structure);
 
    ADC_OverrunModeCmd(ADC1, ENABLE);                               //????????
    ADC_ChannelConfig(ADC1, ADC_Channel_0 | ADC_Channel_1 | ADC_Channel_2
                          | ADC_Channel_8 | ADC_Channel_14 | ADC_Channel_15,
                          ADC_SampleTime_13_5Cycles);               //??????,????125nS
    ADC_GetCalibrationFactor(ADC1);                                 //?????ADC
    ADC_Cmd(ADC1, ENABLE);                                          //??ADC1
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);         //??ADC1????
 
    ADC_DMACmd(ADC1, ENABLE);                                       //??ADC_DMA
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);           //??DMA?????????
    ADC_StartOfConversion(ADC1);                                    //??????(??)

		
		*/

		
		ADC->CCR = ((uint32_t)0x00010000);
		ADC1->CR1 = ((uint32_t)0x0400A13F);
		ADC1->CR2 = ((uint32_t)0x0D0D0103);//bit 9 
		
		ADC1->SQR3 = ((uint32_t)0x14B41020);//     01010  01011  01000   00100   00001   00000
		ADC1->SMPR2 = ((uint32_t)0x001fffff);
		
		
//----------------------------------------------------------------------------------------------------------------------------
		
		

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 , ENABLE);              //??DMA??
 
    nvic_init_structure.NVIC_IRQChannel = DMA1_Stream7_IRQn;       //??DMA1????
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //????
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0; 
		nvic_init_structure.NVIC_IRQChannelSubPriority = 0;		//?????0
    NVIC_Init(&nvic_init_structure);
 
    DMA_DeInit(DMA1_Stream7);                                      //??DMA1_channel1
    DMA_StructInit(&dma_init_structure);                            //???DMA???
 
    dma_init_structure.DMA_BufferSize = 6;            //DMA????????
    dma_init_structure.DMA_DIR = DMA_DIR_PeripheralToMemory;             //DMA??:???????                          //???????
    dma_init_structure.DMA_Memory0BaseAddr = (uint32_t)&adc_dma_tab[0];//??????????
    dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//???????Halfword
    dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;        //??????
    dma_init_structure.DMA_Mode = DMA_Mode_Circular;                //DMA????,??????????
    dma_init_structure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1->DR);//???????
    dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//?????????Halfword
    dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//????????
    dma_init_structure.DMA_Priority = DMA_Priority_High;             //DMA???????
    DMA_Init(DMA1_Stream7, &dma_init_structure);
 
    DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE);                  //??DMA??
    DMA_ClearITPendingBit(DMA1_Stream7,DMA_IT_TC);                        //????DMA????
    DMA_Cmd(DMA1_Stream7, ENABLE);                                  //??DMA1
//---------------------------------------------------------------------------------------------------------------


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


void DMA1_Channel1_IRQHandler()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
    if(DMA_GetITStatus(DMA1_Stream7,DMA_IT_TC))                      //??DMA??????
    {
        if(sample_finish == 0)
        {
            sample_1[sample_index] = adc_dma_tab[0];
            sample_2[sample_index] = adc_dma_tab[1];
            sample_3[sample_index] = adc_dma_tab[2];
            sample_4[sample_index] = adc_dma_tab[3];
            sample_5[sample_index] = adc_dma_tab[5];
            sample_6[sample_index] = adc_dma_tab[4];
            sample_index++;
        }
        if(sample_index >= 128)                         //??????????????
        {
            sample_index = 0;                   //??????,?????
            DMA_Cmd(DMA1_Stream7, DISABLE);            //??????,??DMA
            sample_finish = 1;                          //????????
        }
    }
    DMA_ClearITPendingBit(DMA1_Stream7,DMA_IT_TC);                   //??DMA?????
}


	 









