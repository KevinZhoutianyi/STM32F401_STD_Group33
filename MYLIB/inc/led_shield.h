#ifndef __LED_SHIELD_H
#define __LED_SHIELD_H
#include "sys.h"

#define LEDR PBout(4)	
#define LEDG PCout(7)	 
#define LEDB PAout(9)	

void LED_Shield_Init(void); 	
void Blink(void);
#endif
