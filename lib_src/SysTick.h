#include<stm32f4xx.h>
#include<stdio.h>

volatile uint32_t DelayPeriod = 0;
volatile int delay = 0;

// ticks - no. of clock cycles after which we generate an interupt (value range 1 - 16.777.215)
// clock - 0 for external clock, 1 for cpu clock
// value - represents current value of timer (default)
// priority - defines SysTick priority
// event - pokazivac na funkciju koju želimo da izvršimo na SysTick underflow

//,void (*event)()
void Init_SysTick(uint32_t ticks, uint32_t clock, uint32_t priority, uint32_t curr_val,void (*event)());
void SysTick_Delay(uint32_t period);
void SysTick_Handler(void);
void SysTick_Scheduling(void);

void (*func)();

void Init_SysTick(uint32_t ticks,uint32_t clock, uint32_t priority, uint32_t curr_val,void (*event)()){
	
	DelayPeriod = 0;	// po zadanom nema delaya
	delay = 0;
	
	SysTick->CTRL = 0; // onemoguciti SysTick IRQ i sat
	
	SysTick->LOAD = ticks - 1; //postavljanje reaload vrijednosti 
	
	NVIC_SetPriority(SysTick_IRQn, priority); // definisanje prioriteta SysTick_IRQ
	
	SysTick->VAL = curr_val; //resetovanje vrijednosti (trebalo bi biti nula)
	
	SysTick->CTRL |= clock; // odabir sata
	
	NVIC_EnableIRQ(SysTick_IRQn); // postavljanje enable bita na 1 za SysTick_IRQ
	
	func = event;	// definisanje eventa kojeg želimo izvršiti na underflow
}

void SysTick_Handler(void){
	//koristimo sys tick samo sa delay
	if(delay == 1){
		if(DelayPeriod > 0)
			DelayPeriod--;
	}
	//koristimo SysTick za vršenje neke radnje u fiksnim vremenskim intervalima
	else{	
		(*func)();
	}
}

void SysTick_Delay(uint32_t period){
	delay = 1; // 1 je za delay enabled
	DelayPeriod = period; // definisanje perioda delaya
	while(DelayPeriod != 0); // delay = DelayPeriod * (ticks-1)
}

//REQUIRES MORE PRECISION WITH DELAY AND INTERUPT LENGTH TIME



