#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>

void enable_processor_faults(void);
__attribute__((noreturn)) void MemManage_Handler(void);
__attribute__((noreturn)) void BusFault_Handler(void);
__attribute__((noreturn)) void UsageFault_Handler(void);
__attribute__((noreturn)) void HardFault_Handler(void);

void enable_processor_faults(void)
{
	//	uint32_t volatile *SHCSR = (uint32_t *)0xE000ED24;
	//	*SHCSR = (1 << 18); // Enabling the Usage Fault - setting 18th bit to 1
	//	*SHCSR = (1 << 17); // Enabling the Bus Fault - setting 17th bit to 1
	//	*SHCSR = (1 << 16); // Enabling the MemManage Fault - setting 16th bit to 1
	
	// OR
	
	SCB->SHCSR |=(1 << 18);
	SCB->SHCSR |=(1 << 17);
	SCB->SHCSR |=(1 << 16);
}

//Gets triggered when user program performes unappropriate memmory management
__attribute__((noreturn)) void MemManage_Handler(void)
{
	printf("Exception: MemManage Fault\n");
	while(1);
}

//Gets triggered when user program trys to access memory in a unappropriate way
__attribute__((noreturn)) void BusFault_Handler(void)
{
	printf("Exception: BusFault\n");
	while(1);
}

//Gets triggered when user program causes unappropriate memmory management
__attribute__((noreturn)) void UsageFault_Handler(void)
{
	printf("Exception: Usage Fault\n");
	while(1);
}

// Gets triggered when there is no other appropriate fault handler 
__attribute__((noreturn)) void HardFault_Handler(void)
{
	printf("Exception: HardFault\n");
	while(1);
}
