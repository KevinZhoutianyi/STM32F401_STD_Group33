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
	Adc_Init(); 
	
	while(1)
	{
		
		
		u16 adcx;
		float temp;

	adcx=Get_Adc_Average(0,20);//???? 5 ????,20 ????

	temp=(float)adcx*(3.3/4096); //???????????????,?? 3.1111
	printf("%f \n",temp);
	delay_ms(250); 
		
	
	}		
}


