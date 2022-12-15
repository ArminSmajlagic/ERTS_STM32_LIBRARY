#include "SysTick.h"
#include <stdio.h>

void some_func(void);

void some_func(void){
		printf("SysTick interupt handling funkcija me pozvala");
}

int main(void){
	uint32_t ticks = 100000;
	uint32_t clock = 0x00000007;
	uint32_t prioritet = 3;
	uint32_t vrijednost = 0;
	Init_SysTick(ticks, clock, prioritet, vrijednost, some_func);
	SysTick_Delay(500);
	
}