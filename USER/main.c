#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "adc.h"

int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(84);		
	LED_Init();
	uart_init(9600,2);
	Adc_Init(SINGLEMODE);
	
	while(1)
	{
		
		
		MySingleAdcRead();

	
	}		
}


