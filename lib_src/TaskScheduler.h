#include <stdint.h>
#include <stdio.h>
#include "SysTick.h"
#include "CPUFaults.h"

//definition
/* Stack Memory Address Calculation */
#define SIZE_TASK_STACK  		1024U
#define SIZE_SCHED_STACK 		1024U

#define SRAM_START				0X20000000U
#define SRAM_SIZE				((128) * (1024))
#define SRAM_END				((SRAM_START) + (SRAM_SIZE))

#define T1_STACK_START			SRAM_END
#define T2_STACK_START			((SRAM_END) - (1 * SIZE_TASK_STACK))
#define T3_STACK_START			((SRAM_END) - (2 * SIZE_TASK_STACK))
#define T4_STACK_START			((SRAM_END) - (3 * SIZE_TASK_STACK))
#define SCHED_STACK_START		((SRAM_END) - (4 * SIZE_TASK_STACK))

#define MAX_USER_TASKS 	   4
#define TASK_READY_STATE   0
#define TASK_RUNNING_STATE   1
#define TASK_BLOCKED_STATE 2

#define INTERRUPT_DISABLE() __asm volatile("MOV R0, #0x1"); \
		__asm("MSR PRIMASK, R0")
#define INTERRUPT_ENABLE() __asm volatile("MOV R0, #0x0"); \
		__asm("MSR PRIMASK, R0")
		
		
//Task Control Block
typedef struct
{
	uint32_t task_frame;
	uint8_t  task_state;
	void (*task_handler)(void);
}TCB_t;

int current_task = 0; 
TCB_t volatile tasks[MAX_USER_TASKS];

void init_user_tasks(void);

void save_psp_value(uint32_t current_psp_value);
uint32_t get_psp_value(void);

void Task1_Handler(void);
void Task2_Handler(void);
void Task3_Handler(void);
void Task4_Handler(void);

void decide_next_task(void);

void (*Task1)(void);
void (*Task2)(void);
void (*Task3)(void);
void (*Task4)(void);


//void SysTick_PendSV_Trigger(void);

__attribute__((naked)) void Context_Switch(void);

void PendSV_Handler(void);

void SysTick_Init(void);

__attribute__((naked)) void init_scheduler_task_stack(void);

__attribute__((naked)) void switch_sp_to_psp(void);

void Scheduler_Init(void (*task1)(void),void (*task2)(void),void (*task3)(void),void (*task4)(void));

void init_user_tasks_stack(void);

void unblock_user_tasks(void);

void unblock_user_tasks(void){
	
	int i;
	
	for(i=0;i<MAX_USER_TASKS;i++){
		tasks[i].task_state = TASK_READY_STATE;
	}
	
}
uint32_t get_LR(void);
void set_LR(uint32_t value);

// implementation

void init_user_tasks_stack(void)
{
	int i;
	uint32_t *pPSP;

	tasks[0].task_state = TASK_READY_STATE;
	tasks[1].task_state = TASK_READY_STATE;
	tasks[2].task_state = TASK_READY_STATE;
	tasks[3].task_state = TASK_READY_STATE;

	tasks[0].task_handler = Task1_Handler;
	tasks[1].task_handler = Task2_Handler;
	tasks[2].task_handler = Task3_Handler;
	tasks[3].task_handler = Task4_Handler;
	
	tasks[0].task_frame = T1_STACK_START;
	tasks[1].task_frame = T2_STACK_START;
	tasks[2].task_frame = T3_STACK_START;
	tasks[3].task_frame = T4_STACK_START;
	
	//...
	
	for(i=0; i<MAX_USER_TASKS ; i++)
	{
		int j;

		pPSP = (uint32_t*)tasks[i].task_frame;	

		pPSP--; // xPSR
		*pPSP = (uint32_t)0x01000000; // DUMMY xPSR Should be 0x01000000 because of the T bit

		pPSP--; // PC
		*pPSP = (uint32_t)tasks[i].task_handler;

		pPSP--; // LR
		*pPSP = (uint32_t)0xFFFFFFFD; // Special Return Value
		
		for(j = 0; j<13; j++)
		{
			pPSP--; // General Purpuse Registers R12-R0
			*pPSP = (uint32_t)0x00;
		}

		tasks[i].task_frame = (uint32_t)pPSP; // Saving the final value of the PSP
	}
}

uint32_t get_psp_value(void)
{
	return tasks[current_task].task_frame;
}

void save_psp_value(uint32_t current_psp_value)
{
	tasks[current_task].task_frame = current_psp_value;
}

void SysTick_Init(void){
	uint32_t ticks = 10000; //after 100000 CPU ticks PendSV context switch will be triggerd
	uint32_t clock = 0x00000007; //CPU clock
	uint32_t prioritet = 1; // setting highest priority
	uint32_t vrijednost = 0; // when tick goes from 1 to 0 (vrijednost) underflow will happen and PendSV will be tiggered

	Init_SysTick(ticks, clock, prioritet, vrijednost); //this will init SysTick to trigger PendSV context switch in fixed time intervals
}

__attribute__((naked)) 
void init_scheduler_task_stack(void)
{
	__asm volatile("MSR MSP,%0" : : "r"((SRAM_END) - (5 * SIZE_TASK_STACK)) : );
	__asm volatile("BX LR");
}

__attribute__((naked)) 
void switch_sp_to_psp(void)
{
	// Initialize the PSP with TASK1 starting value
	__asm volatile("PUSH {LR}"); // preserve LR which connects back to main
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP, R0"); // initialize PSP
	__asm volatile("POP {LR}"); // bring LR back

	// Change the MSP to PSP
	__asm volatile("MRS R0, CONTROL"); // Saving CONTROL Register to R0
	__asm volatile("ORR R0, R0, #0x02"); // Setting the 1st bit
	__asm volatile("MSR CONTROL, R0"); // Load back to CONTROL Register
	__asm volatile("BX LR");
}

void Scheduler_Init(void (*task1)(void),void (*task2)(void),void (*task3)(void),void (*task4)(void)){
	
	
	enable_processor_faults();
	
	Task1 = task1;
	Task2 = task2;
	Task3 = task3;
	Task4 = task4;

	init_scheduler_task_stack();
	
	init_user_tasks_stack();
	
	SysTick_Init(); // initializing systick clock
	
	switch_sp_to_psp();
	
	Task1_Handler();	
}

void SysTick_Handler(void){
	
		/* First push the LR, Because we will use BL instruction */
	__asm volatile("PUSH {LR}");
	/* Save  the context of current tasks */
	//1. Get current running task's PSP value
	__asm volatile("MRS R0, PSP");
	//2. Using that PSP value store SF2(R4 to R11)
	__asm volatile("STMDB R0!, {R4-R11}");
	//3. Save the current value of PSP
	__asm volatile("BL save_psp_value");

	/* Retrieve the context of next task */
	//1. Decide next task to run
	__asm volatile("BL decide_next_task");
	//2. Get its past PSP value
	__asm volatile("BL get_psp_value");
	//3. Using that PSP value retrieve SF2(R4 to R11)
	__asm volatile("LDMFD R0!, {R4-R11}");
	//4. Update PSP
	__asm volatile("MSR PSP, R0");
	
	tasks[current_task].task_state = TASK_RUNNING_STATE;

	/* POP the LR and go back */
	__asm volatile("POP {LR}");
	__asm volatile("BX LR");
	
}


//dynamicaly set tasks are going to be executed in their appropriate handlers
void Task1_Handler(void){
	int c;
	while(1){
		c++;
	}
}

void Task2_Handler(void)
{
	int c;
	while(1){
		c++;
	}
}

void Task3_Handler(void)
{
	int c;
	while(1){
		c++;
	}
}
void Task4_Handler(void)
{
	int c;
	while(1){
		c++;
	}
}


//Round-Robin task scheduling
void decide_next_task(void) 
{
		int i;
		for(i=0;i<MAX_USER_TASKS;i++){
			
			tasks[current_task].task_state = TASK_BLOCKED_STATE;
			
			current_task++;
			
			if(current_task == 4){
				current_task = 0;
				unblock_user_tasks();
			}
						
			if(tasks[current_task].task_state == TASK_READY_STATE)
				break;
			
		}
		
}




void PendSV_Handler(void)
{
	//context switch
}



