/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

/* Priorities at which the tasks are created. */
#define mainTask1_PRIORITY  ( tskIDLE_PRIORITY + 2 )
#define mainTask2_PRIORITY  ( tskIDLE_PRIORITY + 1 )

#define mainTIMER_TEST_PERIOD           ( 50 )


/*
 * Prototypes for the standard FreeRTOS callback/hook functions implemented
 * within this file.
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

/*-----------------------------------------------------------*/

unsigned int matrix_task_loop_tick_count = 0;
unsigned int communication_task_loop_tick_count = 0;
unsigned int matrix_task_average_tick_count = 0;
unsigned int communication_task_average_tick_count = 0;

/*-----------------------------------------------------------*/
#define SIZE    10
#define ROW     SIZE
#define COL     SIZE

static void matrix_task ( void *pvParameters )
{
    TickType_t t1, t2;

    (void)pvParameters;

    int i;
    double **a = (double **) pvPortMalloc(ROW * sizeof(double *));
    for (i=0; i< ROW; i++) {
        a[i] = (double *)pvPortMalloc(COL * sizeof(double));
    }
    double **b = (double **) pvPortMalloc(ROW * sizeof(double *));
    for (i=0; i< ROW; i++) {
        b[i] = (double *)pvPortMalloc(COL * sizeof(double));
    }
    double **c = (double **) pvPortMalloc(ROW * sizeof(double *));
    for (i=0; i< ROW; i++) {
        c[i] = (double *)pvPortMalloc(COL * sizeof(double));
    }

    double sum = 0;
    int j, k, l;

    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            a[i][j] = 1.5;
            b[i][j] = 2.6;
        }
    }

    while (1) {
        long simulationdelay;

        t1 = xTaskGetTickCount(); 
        printf("%u : Starts matrix task...\n", t1);
        fflush(stdout);

        for (simulationdelay=0; simulationdelay<1000000000; simulationdelay++); /* make sure to turn off optimzation with -O0 */

        for (i=0; i<SIZE; i++) {
            for (j=0; j<SIZE; j++) {
                c[i][j] = 0.0;
            }
        }

        for (i=0; i<SIZE; i++) {
            for (j=0; j<SIZE; j++) {
                sum = 0.0;
                for (k=0; k<SIZE; k++) {
                    for (l=0; l<10; l++) {
                        sum = sum + a[i][k]*b[k][j];
                    }
                }
                c[i][j] = sum;
            }
        }
        t2 = xTaskGetTickCount(); 
        matrix_task_loop_tick_count = t2 - t1;
        if (matrix_task_average_tick_count != 0) {
            matrix_task_average_tick_count = (matrix_task_average_tick_count + matrix_task_loop_tick_count)/2;
        }
        else {
            matrix_task_average_tick_count = matrix_task_loop_tick_count;
        }
        printf("%u : Matrix task done!\n", t2);
        fflush(stdout);
        vTaskDelay(100);
    }
}

/*-----------------------------------------------------------*/
static void communication_task ( void *pvParameters )
{
    TickType_t t1, t2;
    (void)pvParameters;

    while (1) {
        t1 = xTaskGetTickCount(); 
        printf("%u : Sending data...\n", t1); 
        fflush(stdout);
        vTaskDelay(100);
        t2 = xTaskGetTickCount(); 
        communication_task_loop_tick_count = t2 - t1;
        if (communication_task_average_tick_count != 0) {
            communication_task_average_tick_count = (communication_task_average_tick_count + communication_task_loop_tick_count)/2;
        }
        else {
            communication_task_average_tick_count = communication_task_loop_tick_count;
        }
        printf("%u : Data send!\n", t2);
        fflush(stdout);
        vTaskDelay(100);
    }
}

/*-----------------------------------------------------------*/
static void measure_task ( void *pvParameters )
{
    TickType_t last_time;

    (void)pvParameters;

    last_time = xTaskGetTickCount();

    while(1) {
        printf("%u : matrix task average = %d, communication task average = %d\n", 
            xTaskGetTickCount(), matrix_task_average_tick_count, communication_task_average_tick_count);
        fflush(stdout);

        vTaskDelayUntil(&last_time, 1000);
    }
}

/*-----------------------------------------------------------*/

int main ( void )
{

    xTaskCreate( matrix_task, "Matrix", 1000, NULL, 3, NULL);
    xTaskCreate( communication_task, "Communication", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
    xTaskCreate( measure_task, "Measure", configMINIMAL_STACK_SIZE, NULL, 5, NULL );

    /* Start the scheduler itself. */
    vTaskStartScheduler();

    /* Should never get here unless there was not enough heap space to create
    the idle and other system tasks. */
    return 0;
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */

        /* Call the idle task processing used by the full demo.  The simple
           blinky demo does not use the idle task hook. */
           //vFullDemoIdleFunction();
}

/*-----------------------------------------------------------*/


void vApplicationTickHook( void )
{
    
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */
}

/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
    taskENTER_CRITICAL();
    {
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
    exit(-1);
}
/*-----------------------------------------------------------*/
