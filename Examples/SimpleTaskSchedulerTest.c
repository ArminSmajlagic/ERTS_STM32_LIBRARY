#include "TaskScheduler.h"
//#include "EXTI.h"


////MAP MEMMORY TO R/W/E - range : 0x20000000, 0x200FFFFF - this is range for task table
void task1(void);
void task2(void);
void task3(void);
void task4(void);
void task5(void);


__attribute__((noreturn)) 
void task1(void){
	//this is task 1
	while(1);
}
__attribute__((noreturn)) 
void task2(void){
	//this is task 2
	while(1);
}
__attribute__((noreturn)) 
void task3(void){
	//this is task 3
	while(1);
}
__attribute__((noreturn)) 
void task4(void){
	//this is task 4
	while(1);
}

int main(void)
{
	Scheduler_Init(task1,task2,task3,task4);
	
	//this never gets executed
	for(;;);
}



