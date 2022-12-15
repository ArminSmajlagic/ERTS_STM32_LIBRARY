#include<stm32f4xx.h>
#include<system_stm32f4xx.h>
#include<stdio.h>

void (*func)();

//line - izvor interrupta
//priority - prioritet EXTIx interrupta
//event - pokazivac na funkciju koju želimo izvršiti kada se trigeruje interrupt hanler

void Init_EXTI(int line, int priority, void (*event)());

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

void Init_EXTI(int line, int priority, void (*event)()){
	func = event;
	switch(line){
		case 0:
		{
			NVIC_SetPriority(EXTI0_IRQn, priority);
			NVIC_EnableIRQ(EXTI0_IRQn);
			break;
		}
		case 1:
		{
			NVIC_SetPriority(EXTI1_IRQn, priority);
			NVIC_EnableIRQ(EXTI1_IRQn);
			break;
		}
		//...
		case 2:
		{
			NVIC_SetPriority(EXTI2_IRQn, priority);
			NVIC_EnableIRQ(EXTI2_IRQn);
			break;
		}
		case 3:
		{
			NVIC_SetPriority(EXTI3_IRQn, priority);
			NVIC_EnableIRQ(EXTI3_IRQn);
			break;
		}
		case 4:
		{
			NVIC_SetPriority(EXTI4_IRQn, priority);
			NVIC_EnableIRQ(EXTI4_IRQn);
			break;
		}
		//...
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		{
			NVIC_SetPriority(EXTI9_5_IRQn, priority);
			NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		}
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		{
			NVIC_SetPriority(EXTI15_10_IRQn, priority);
			NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}	
	}
}

void EXTI0_IRQHandler(void){
	if (EXTI->PR & (1<<0)) {
		func();
		EXTI->PR |= (1<<0);  
	}
}


void EXTI1_IRQHandler(void){
	if (EXTI->PR & (1<<1)) {
		func();
		EXTI->PR |= (1<<1);  
	}
}

void EXTI2_IRQHandler(void){
	if (EXTI->PR & (1<<2)) {		
		func();		
		EXTI->PR |= (1<<2);  
	}
}

void EXTI3_IRQHandler(void){
	if (EXTI->PR & (1<<3)) {		
		func();		
		EXTI->PR |= (1<<3);  
	}
}

void EXTI4_IRQHandler(void){
	if (EXTI->PR & (1<<4)) {		
		func();	
		EXTI->PR |= (1<<4);  
	}
}

void EXTI9_5_IRQHandler(void){
	
	if (EXTI->PR & (1<<5)) {	
		func();
		EXTI->PR |= (1<<5);  
	}
	
	if (EXTI->PR & (1<<6)) {	
		func();
		EXTI->PR |= (1<<6);  
	}
		
	if (EXTI->PR & (1<<7)) {
		func();	
		EXTI->PR |= (1<<7);  
	}
		
	if (EXTI->PR & (1<<8)) {
		func();	
		EXTI->PR |= (1<<8);  
	}
		
	if (EXTI->PR & (1<<9)) {
		func();	
		EXTI->PR |= (1<<9);  
	}
}

void EXTI15_10_IRQHandler(void){
	if (EXTI->PR & (1<<10)) {
		func();
		EXTI->PR |= (1<<10);  
	}
	
	if (EXTI->PR & (1<<11)) {
		func();	
		EXTI->PR |= (1<<11);  
	}
		
	if (EXTI->PR & (1<<12)) {
		func();	
		EXTI->PR |= (1<<12);  
	}
		
	if (EXTI->PR & (1<<13)) {	
		func();
		EXTI->PR |= (1<<13);  
	}
		
	if (EXTI->PR & (1<<14)) {
		func();	
		EXTI->PR |= (1<<14);  
	}
}
