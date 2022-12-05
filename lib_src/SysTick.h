#include<stm32f4xx.h>
#include<stdio.h>

volatile uint32_t DelayPeriod = 0;
volatile int delay = 0;

// ticks - no. of clock cycles after which we generate an interupt (value range 1 - 16.777.215)
// clock - 0 for external clock, 1 for cpu clock
// value - represents current value of timer (default)
// priority - defines SysTick priority
// event - pokazivac na funkciju koju želimo da izvršimo na SysTick underflow

void SysTickIRQ(uint32_t ticks, uint32_t clock, uint32_t priority, uint32_t curr_val);

void Init_SysTick(uint32_t ticks, uint32_t clock, uint32_t priority, uint32_t curr_val,void (*event)());
void SysTick_Delay(uint32_t period);
void SysTick_Handler(void);
void SysTick_Scheduling(void);

void (*func)();

void Init_SysTick(uint32_t ticks,uint32_t clock, uint32_t priority, uint32_t curr_val, void (*event)()){
	DelayPeriod = 0;
	SysTick->CTRL = 0;
	SysTick->LOAD = ticks - 1;
	NVIC_SetPriority(SysTick_IRQn, priority);
	SysTick->VAL = curr_val;
	SysTick->CTRL |= clock;
	NVIC_EnableIRQ(SysTick_IRQn);
	func = event;	
}

void SysTick_Handler(void){
	if(delay == 1){//if we use SysTick for delay only
		if(DelayPeriod > 0)
			DelayPeriod--;
	}
	else{	//if we use SysTick for handeling event
		(*func)();
	}
}

void SysTick_Delay(uint32_t period){
	delay = 1;
	DelayPeriod = period;
	while(DelayPeriod != 0);
}

//REQUIRES MORE PRECISION WITH DELAY AND INTERUPT LENGTH TIME



