/*
    FreeRTOS V8.0.1 - Copyright (C) 2014 Real Time Engineers Ltd. 
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/prcm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "utils/cpu_usage.h"


#if (configGENERATE_RUN_TIME_STATS)

/* This demo uses WTimer5 to generate the time base for gathering run time
statistics information.  Run time statistics show much processing time has
been allocated to each task since the application booted. */

/* In this port, the time base for the run time stats is generated by a 64 bit timer.
 * FreeRTOS requires a 32 bit counter to get track of the statistics. This counter should be at least 10 times faster than
 * configTICK_RATE_HZ (the RTOS Tick)
 * In this implementation, the value of the 64 bit wtimer is divided by 1024 and casted to 32 bits in order
 * to compute the statistics. Thus the counter speed is CPU_CLOCK/1024, which is still much faster than  configTICK_RATE_HZ
 *
 * "cycles" printed in the statistics are cycles of this counter ticks (each corresponding to 1024 CPU clock cycles).
 *
 */

static uint32_t g_TimerStats=0;

void RunTimeStatsTimerHandler (void)
{
	g_TimerStats++;
	TimerIntClear(TIMERA2_BASE,TIMER_TIMA_TIMEOUT);
}






/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{

    	MAP_PRCMPeripheralClkEnable  ( PRCM_TIMERA2,PRCM_RUN_MODE_CLK|PRCM_SLP_MODE_CLK); //JMCG clock gating en sleep


	    MAP_TimerControlStall(TIMERA2_BASE,TIMER_BOTH,1);	//Esto es para que no cuente en modo debug

	    //
	    // Configure the timer for 32-bit periodic operation.
	    //
	    MAP_TimerConfigure(TIMERA2_BASE,TIMER_CFG_PERIODIC);

	    //
	    // Set the load value for the timer to the maximum value.
	    //
	    MAP_TimerLoadSet(TIMERA2_BASE, TIMER_A, 0xFFFFFFFF);

	    g_TimerStats=0;


	    //Register the interrupt
	    IntRegister(INT_TIMERA2A,RunTimeStatsTimerHandler);
	    TimerIntEnable(TIMERA2_BASE,TIMER_TIMA_TIMEOUT);
	    IntEnable(INT_TIMERA2A);

	    //
	    // Enable the timer.  It will now count the number of system clocks
	    // during which the processor is executing code.
	    //
	    TimerEnable(TIMERA2_BASE, TIMER_A);

}
/*-----------------------------------------------------------*/

inline uint32_t GetOverflowCounts()
{
	//Divide el numero de ticks por 1024 (y trunca a 32 bits).
	return (((0xFFFFFFFF-MAP_TimerValueGet(TIMERA2_BASE,TIMER_A))>>10)|(g_TimerStats<<22));
}

/*-----------------------------------------------------------*/


#endif
