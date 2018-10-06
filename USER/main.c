#include "sys.h"
#include "delay.h"
#include "led.h"



int main(void)
{ 
 
	delay_init(84);		
	LED_Init();

	
	while(1)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);

	delay_ms(500);  		
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	 
	
	delay_ms(500);  
	}		
}


