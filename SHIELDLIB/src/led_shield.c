#include "led_shield.h" 
#include "delay.h"

void LED_Shield_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

}

void Blink(void)
{
	static int temp  = 0;
	
	temp++;
	if((temp)%3 == 0)
		LEDR = !LEDR;
	else if((temp)%3 == 1)
		LEDG = !LEDG;
	else if((temp)%3 == 2)
		LEDB = !LEDB;
	
	delay_ms(35);
	
	
}






