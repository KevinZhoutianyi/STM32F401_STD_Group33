#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "led_shield.h"
#include "speaker_shield.h"
int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(84);		
	LED_Init();
	LED_Shield_Init();
	uart_init(9600,2);
	Speaker_Shield_Init(2000);
	
	while(1)
	{
		


	        
	}		    
}         
          

