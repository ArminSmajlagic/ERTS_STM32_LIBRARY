# ERTS_STM32_LIBRARY

This project is collection of modules which simplify bare metal interaction with ARM Cortex-Mx platform. It contains functions for interupt handeling, system timers &amp; alarms, task scheduling and examples in simulated envirement. 

## Library/Driver modules

- Round-Robin SysTick Timer Task Scheduler
- External (IRQ routine) Interupts Request Handler for (GPIO) general purpose I/O pins 
- SysTick timer delay and timer IRQ routine (for tick speed of EXT Clock or CPU Clock)
- System Faults (IRQ routine) Interupt Request Handler (MemManagFault, BusFault, HardFault, UsageFault)

Previously mentioned modules offer ease of bare metal interaction with most STM32 platforms.

Author: Armin Smajlagic
