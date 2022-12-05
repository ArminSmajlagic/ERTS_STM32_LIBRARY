#include "EXTI.h"
#include <stdio.h>

void some_func(void);

void some_func(void){
	printf("EXTI interupt handling funkcija me pozvala");
}

int main(void){
	uint32_t linija = 0;
	uint32_t prioritet = 0;

	Init_EXTI(linija,prioritet,some_func);
}