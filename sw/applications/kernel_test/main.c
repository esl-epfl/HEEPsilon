/*
                              *******************
******************************* C SOURCE FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : main.c                                                       **
** version  : 1                                                            **
** date     : 05/04/23                                                     **
**                                                                         **
*****************************************************************************
**                                                                         **
** Copyright (c) EPFL                                                      **
** All rights reserved.                                                    **
**                                                                         **
*****************************************************************************
*/

/***************************************************************************/
/***************************************************************************/

/**
* @file   main.c
* @date   05/04/23
* @brief  An application to run a number of kernels under a same given 
* structure.
*
*/

#define _KERNELS_C

/****************************************************************************/
/**                                                                        **/
/*                             MODULES USED                                 */
/**                                                                        **/
/****************************************************************************/

//Include kernels!
#include "kernels/bitcount/bitcount.h"
#include "kernels/reversebits/reversebits.h"
#include "kernels/gsm/gsm.h"
#include "kernels/sha2/sha2.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                        TYPEDEFS AND STRUCTURES                           */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                      PROTOTYPES OF LOCAL FUNCTIONS                       */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED VARIABLES                             */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                            GLOBAL VARIABLES                              */
/**                                                                        **/
/****************************************************************************/

static kcom_kernel_t *kernels[] = { 
        &bitc_kernel,
        &reve_kernel,
        &gsm_kernel,
        &sha2_kernel, 
        // Add all other kernels here
    };

static kcom_perf_t  kperf;
static kcom_run_t   run[ ITERATIONS_PER_KERNEL ];
static kcom_stats_t stats;

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED FUNCTIONS                             */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

void main()
{
    uint8_t kernels_n = sizeof( kernels ) / sizeof( kcom_kernel_t * );
    kcom_kernel_t* kernel;

    PRINTF("Will execute %d kernels %d times each! \n",kernels_n, ITERATIONS_PER_KERNEL );
    kcom_init();
    
    for( uint8_t ker_idx = 0; ker_idx < kernels_n; ker_idx++ )
    {
        kernel = kernels[ ker_idx ];
        stats.name = kernel->name;
        stats.n = ITERATIONS_PER_KERNEL;
                
        /* Set the kernel ID */
        uint8_t kernel_id = ( ker_idx % (CGRA_KMEM_SIZE - 1) ) + 1; // Must be between 1 and (KMEM_SIZE - 1). 
        kernel->kmem[ kernel_id ] = kernel->kmem[1]; // By default the kernels come located with id = 1.
        // The kernel = 1 is kept, so we can always take it from there. 

        /* CGRA load */
        kcom_perfRecordStart( &(kperf.time.load) );
            kcom_load( kernel );
        kcom_perfRecordStop( &(kperf.time.load) );

        for( uint8_t it_idx = 0; it_idx < ITERATIONS_PER_KERNEL; it_idx++ )
        {
            /* Reset the CGRA performance counters */
            kcom_rstPerfCounter();

            /* Load (of inputs). */
#if REPEAT_FIRST_INPUT
            if( it_idx < 2 ) kcom_resetRand(); 
#endif //REPEAT_FIRST_INPUT
            kernel->config();

            /* Obtention of dead-zone-time */
            kcom_perfRecordStart(   &(kperf.time.dead) );
            kcom_perfRecordStop(    &(kperf.time.dead) );

            /* Software */
            kcom_perfRecordStart(   &(kperf.time.sw) );
                kernel->func();
            kcom_perfRecordStop(    &(kperf.time.sw) );    

            /* CGRA Execution */
            kcom_perfRecordIntrSet( &(kperf.time.cgra) );
            kcom_perfRecordStart(   &(kperf.time.cgra) );
                kcom_launchKernel( kernel_id );
                kcom_waitingForIntr();
            // Time is stopped inside the interrupt handler to make it as fast as possible
  
            /* Result comparison */
            stats.errors += kernel->check();

            /* Subtract the dead times from the obtained values */
            kcom_subtractDead( &(kperf.time.sw.spent_cy),    kperf.time.dead.spent_cy );
            kcom_subtractDead( &(kperf.time.load.spent_cy),  kperf.time.dead.spent_cy );
            kcom_subtractDead( &(kperf.time.cgra.spent_cy),  kperf.time.dead.spent_cy + CGRA_ACCESS_FLAT_COST_CYCLES );

            /* Performance report */
            kcom_getPerf(   &kperf );
#if PRINT_ITERATION_VALUES
            kcom_printPerf( &kperf );
#endif //PRINT_ITERATION_VALUES
            
            /* Add this iteration to the runs vector */
            kcom_populateRun( &run, &kperf, it_idx );
        }

        // Compute the conf time for each iteration. The iteration 0 is assigned the average, and its is subtracted to the cgra operation.
        kcom_extractConfTime( &run, stats.n );

        /* Get statistical values from the whole set of runs for this kernel. */
        kcom_getKernelStats( &run, &stats );
        kcom_printKernelStats( &stats );

    }
    kcom_printSummary( );
}




/****************************************************************************/
/**                                                                        **/
/*                                 EOF                                      */
/**                                                                        **/
/****************************************************************************/
