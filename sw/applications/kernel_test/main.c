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

// For interrupt handling
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"

//Include kernels!
#include "bitcount/bitcount.h"
#include "gsm/gsm.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

/* Configuration definitions */
#define PRINT_ITERATION_VALUES  1
#define PRINT_KERNEL_STATS      1   


#define ITERATIONS_PER_KERNEL   2



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

extern int32_t cgra_input;

/****************************************************************************/
/**                                                                        **/
/*                            GLOBAL VARIABLES                              */
/**                                                                        **/
/****************************************************************************/

static kcom_kernel_t *kernels[] = { 
    //&bitc_kernel,
    &gsm_kernel, 
    &bitc_kernel, 
    &gsm_kernel,
    //&bitc_kernel, 
    //&gsm_kernel,    
    //&bitc_kernel, 
    // Add all other kernels here
    };


static uint8_t      cgra_slot;
static cgra_t       cgra;
static kcom_perf_t  kperf;
static kcom_run_t   run[ ITERATIONS_PER_KERNEL ];
static kcom_stats_t stats;

// Plic controller variables
dif_plic_params_t           rv_plic_params;
dif_plic_t                  rv_plic;
dif_plic_result_t           plic_res;
dif_plic_irq_id_t           intr_num;
int8_t                      cgra_intr_flag;

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


void cgra_config( kcom_kernel_t *ker )
{
    cgra_cmem_init(ker->imem, ker->kmem );
    cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);
    // Select request slot of CGRA
    cgra_slot = cgra_get_slot(&cgra);
    cgra_perf_cnt_enable(&cgra, 1);
    // Set CGRA kernel L/S pointers
    for(int8_t col_idx = 0 ; col_idx < ker->col_n ; col_idx++){
        cgra_set_read_ptr ( &cgra, cgra_slot, &((ker->input[col_idx])),  col_idx );
        cgra_set_write_ptr( &cgra, cgra_slot, &((ker->output[col_idx])),  col_idx );
    }
}

void plic_interrupt_init(dif_plic_irq_id_t irq) {
    // Init the PLIC
    rv_plic_params.base_addr = mmio_region_from_addr((uintptr_t)RV_PLIC_START_ADDRESS);
    plic_res = dif_plic_init(rv_plic_params, &rv_plic);
    if (plic_res != kDifPlicOk) {
        PRINTF("PLIC init failed\n;");
    }

    // Set CGRA priority to 1 (target threshold is by default 0) to trigger an interrupt to the target (the processor)
    plic_res = dif_plic_irq_set_priority(&rv_plic, irq, 1);
    if (plic_res != kDifPlicOk) {
        PRINTF("Set interrupt priority to 1 failed\n");
    }

    plic_res = dif_plic_irq_set_enabled(&rv_plic, irq, 0, kDifPlicToggleEnabled);
    if (plic_res != kDifPlicOk) {
        PRINTF("Enable interrupt failed\n");
    }

    // Enable interrupt on processor side
    // Enable global interrupt for machine-level interrupts
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
    // Set mie.MEIE bit to one to enable machine-level external interrupts
    const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
    CSR_SET_BITS(CSR_REG_MIE, mask);
}

void handler_irq_external(void) {
    // Claim/clear interrupt
    plic_res = dif_plic_irq_claim(&rv_plic, 0, &intr_num);
    if (plic_res == kDifPlicOk && intr_num == CGRA_INTR) {
      kcom_timeStop( &(kperf.time.cgra), &(kperf.time.timer) );  
      cgra_intr_flag = 1;
    }


    // Complete the interrupt
    plic_res = dif_plic_irq_complete(&rv_plic, 0, &intr_num);
    if (plic_res != kDifPlicOk || intr_num != CGRA_INTR) {
        PRINTF("CGRA interrupt complete failed\n");
    }
}


void main()
{

    uint8_t kernels_n = sizeof( kernels ) / sizeof( kcom_kernel_t * );
    
    PRINTF("Will execute %d kernels %d times each! \n",kernels_n, ITERATIONS_PER_KERNEL );

    kcom_timerInit( &(kperf.time.timer) );
    plic_interrupt_init(CGRA_INTR);


    kcom_func_ret_t errors;
    kcom_kernel_t* kernel;

    for( uint8_t ker_idx = 0; ker_idx < kernels_n; ker_idx++ )
    {
        
        kernel = kernels[ ker_idx ];
        PRINTF("=================================================\n");
        PRINTF(" EXECUTING KERNEL: %s\n", kernel->name );
        

        PRINTF("KMEM: %08x\t IMEM: %08x\t Input: %08x\t Output: %08x\n", kernel->kmem, kernel->imem, kernel->input, kernel->output);
        PRINTF("cONFIG: %08x\t func: %08x\t check: %08x\n",kernel->config, kernel->func, kernel->check);


        for( uint8_t it_idx = 0; it_idx < ITERATIONS_PER_KERNEL; it_idx++ )
        {
            cgra_intr_flag = 0;
            
            /* Reset the CGRA performance counters */
            cgra_perf_cnt_reset( &cgra );

            /* Configuration (of inputs). */
            kernel->config();

            /* Obtention of dead-zone-time */
            kcom_timeStart( &(kperf.time.dead), &(kperf.time.timer) );
            kcom_timeStop(  &(kperf.time.dead), &(kperf.time.timer) );

            /* Software */
            kcom_timeStart( &(kperf.time.sw), &(kperf.time.timer) );
                kernel->func();
            kcom_timeStop(  &(kperf.time.sw), &(kperf.time.timer) );

            /* CGRA Configuration */
            kcom_timeStart( &(kperf.time.config), &(kperf.time.timer) );
                cgra_config( kernel );
            kcom_timeStop(  &(kperf.time.config), &(kperf.time.timer) );
            
            /* CGRA Execution */
            cgra_perf_cnt_reset( &cgra );
            kcom_timeStart( &(kperf.time.cgra), &(kperf.time.timer) );
                cgra_set_kernel(&cgra, cgra_slot, 1 );
                while(cgra_intr_flag==0) wait_for_interrupt();
            // Time is stopped inside the interrupt handler to make it as fast as possible

            /* Result comparison */
            errors = kernel->check();

            PRINTF("Errors: %d", errors);

            /* Subtract the dead times from the obtained values */
            kcom_subtractDead( &(kperf.time.sw), &(kperf.time.dead) );
            kcom_subtractDead( &(kperf.time.config), &(kperf.time.dead) );
            kcom_subtractDead( &(kperf.time.cgra), &(kperf.time.dead) );

            /* Performance report */
            kcom_getPerf(&cgra, &kperf );
#if PRINT_ITERATION_VALUES
            kcom_printPerf( &cgra, &kperf );
#endif //PRINT_ITERATION_VALUES
            
            /* Add this iteration to the runs vector */
            kcom_populateRun( &run, &kperf, it_idx );

        }
        /* Get statistical values from the whole set of runs for this kernel. */
        stats.n = ITERATIONS_PER_KERNEL;
        kcom_getKernelStats( &run, &stats );
        kcom_printKernelStats( &stats );
    }

    kcom_printSummary( &cgra );
}




/****************************************************************************/
/**                                                                        **/
/*                                 EOF                                      */
/**                                                                        **/
/****************************************************************************/
