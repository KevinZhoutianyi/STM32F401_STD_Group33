#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

u8 send_data = 1;

int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(84);		
	LED_Init();
	uart_init(9600);

	
	while(1)
	{
		
		USART_SendData (USART1,send_data);
		delay_ms(1000);
			

		
	
	}		
}


