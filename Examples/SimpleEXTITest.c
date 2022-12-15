#include "EXTI.h"
#include <stdio.h>

void some_func(void);

void some_func(void){
	printf("EXTI interupt handling funkcija me pozvala");
}

int main(void){
	
	int prioritet = 0;
	int broj_linija = 15;
	
	int i;
	
	for(i=0; i < broj_linija;i++){
		Init_EXTI(i,prioritet,some_func);
	}

}
