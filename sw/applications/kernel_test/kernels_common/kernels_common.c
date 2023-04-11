/*
                              *******************
******************************* C SOURCE FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : kernels_common.c                                             **
** version  : 1                                                            **
** date     : 06/04/23                                                     **
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
* @file   kernels_common.c
* @date   06/04/23
* @brief  Common functions and types to be used in the characterization of 
* different kernels. 
*
*/

#define _KERNELS_COMMON_C

/****************************************************************************/
/**                                                                        **/
/*                             MODULES USED                                 */
/**                                                                        **/
/****************************************************************************/

#include "kernels_common.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

#define PERF_PRINT_COLUMN_STATS 0
#define PERF_PRINT_LATEX        0
#define PERF_PRINT_TABBED       0

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

uint32_t freq_hz; 

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED FUNCTIONS                             */
/**                                                                        **/
/****************************************************************************/

/* MISCELANEOUS */

uint32_t kcom_getRand()
{
   static uint32_t z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
   uint32_t b;
   b  = ((z1 << 6) ^ z1) >> 13;
   z1 = ((z1 & 4294967294U) << 18) ^ b;
   b  = ((z2 << 2) ^ z2) >> 27; 
   z2 = ((z2 & 4294967288U) << 2) ^ b;
   b  = ((z3 << 13) ^ z3) >> 21;
   z3 = ((z3 & 4294967280U) << 7) ^ b;
   b  = ((z4 << 3) ^ z4) >> 12;
   z4 = ((z4 & 4294967168U) << 13) ^ b;
   return (z1 ^ z2 ^ z3 ^ z4);
}

/* TIME OPERATIONS */

void kcom_timerInit( rv_timer_t *timer)
{
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr  = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
    freq_hz             = soc_ctrl_get_frequency(&soc_ctrl);

    mmio_region_t timer_0_reg = mmio_region_from_addr(RV_TIMER_AO_START_ADDRESS);
    
    rv_timer_init( timer_0_reg, (rv_timer_config_t) { .hart_count = 2, .comparator_count = 1 }, timer );

    rv_timer_tick_params_t tick_params;

    rv_timer_approximate_tick_params( freq_hz, (uint64_t) (TICK_FREQ_HZ), &tick_params );
    rv_timer_set_tick_params(timer, HART_ID, tick_params); 

    // Juan: see if i cannot remove this!
    rv_timer_irq_enable(timer, HART_ID, 0, kRvTimerEnabled);
    rv_timer_arm(timer, HART_ID, 0, 1);

    rv_timer_counter_set_enabled(timer, HART_ID, kRvTimerEnabled);
    
}

uint64_t kcom_getTime( rv_timer_t *timer )
{
    static uint64_t out;
    rv_timer_counter_read( timer, HART_ID, &out );
    return out;
}

void kcom_timeStart( kcom_time_diff_t *perf, rv_timer_t *timer )
{
    perf->start_cy = kcom_getTime( timer );
}

void kcom_timeStop( kcom_time_diff_t *perf,  rv_timer_t *timer )
{
    perf->end_cy = kcom_getTime( timer );
    perf->spent_cy = perf->end_cy - perf->start_cy;
}

void kcom_subtractDead( kcom_time_t *time, kcom_time_t *dead )
{
    *time -= *dead;
}

/* COMPUTATIONS */

void kcom_populateRun( kcom_run_t *run, kcom_perf_t *perf, uint32_t it_idx )
{
    // These times have already been subtracted the dead time.
    (run[ it_idx ]).sw        = perf->time.sw.spent_cy;     
    (run[ it_idx ]).config    = perf->time.config.spent_cy;     
    (run[ it_idx ]).cgra      = perf->time.cgra.spent_cy;   
    (run[ it_idx ]).cyc_ratio = perf->cyc_ratio; 
}

void kcom_getKernelStats( kcom_run_t *run, kcom_stats_t *stats )
{
    kcom_run_t *avg = &( stats->avg );
    kcom_run_t *var = &( stats->var );
    uint32_t iterations = stats->n;

    
    uint8_t paramCount = sizeof( kcom_run_t ) / sizeof( kcom_param_t );

    for( uint8_t p_idx = 0; p_idx < paramCount; p_idx++ )
    {
        /* Get the population average. */
        for( uint32_t it_idx = 0; it_idx < iterations; it_idx++ )
        {
            /* Get the sum of values. */
            ((kcom_param_t*)avg)[p_idx] += ((kcom_param_t*)(&(run[it_idx])))[p_idx];
        }
        /* Divide by the population size. */
        ((kcom_param_t*)avg)[p_idx] /= iterations;
    
        /* Get the variance (sigma^2). */
        for( uint32_t it_idx = 0; it_idx < iterations; it_idx++ )
        {
             /* Get diff with avg. */
            int32_t diff = (int32_t)(((int32_t)((kcom_param_t*)&(run[it_idx]))[p_idx]) -  (int32_t)(((kcom_param_t*)avg)[p_idx]));
            /* Square it. Its multiplied by 100 because otherwise the next division would be < 1 */
            ((kcom_param_t*)var)[p_idx] += diff * diff * CGRA_STAT_PERCENT_MULTIPLIER;
        }
        /* Divide by the population size. */
        ((kcom_param_t*)var)[p_idx] /= iterations;
    }

}

void kcom_getPerf( cgra_t *cgra, kcom_perf_t *perf )
{
    perf->cols_total.cyc_act = 0;
    perf->cols_total.cyc_stl = 0;
    for(int8_t col_idx = 0 ; col_idx < CGRA_MAX_COLS ; col_idx++)
    {
        perf->cols[col_idx].cyc_act    = cgra_perf_cnt_get_col_active(cgra, col_idx);
        perf->cols[col_idx].cyc_stl    = cgra_perf_cnt_get_col_stall (cgra, col_idx);
        perf->cols_total.cyc_act       += perf->cols[col_idx].cyc_act;
        perf->cols_total.cyc_stl       += perf->cols[col_idx].cyc_stl;
    }
    perf->cyc_ratio = perf->cols_total.cyc_stl*CGRA_STAT_PERCENT_MULTIPLIER*10 / perf->cols_total.cyc_act;
}

/* PRESENTING THE RESULTS */

void kcom_printPerf( cgra_t *cgra, kcom_perf_t *perf )
{

#if PERF_PRINT_COLUMN_STATS
    PRINTF("\n===========\n COLUMN STATS BELOW \n===========\n");
    PRINTF("Col\tAct\tStl\n");
    for(int8_t col_idx = 0 ; col_idx < CGRA_MAX_COLS ; col_idx++)
    {
        PRINTF("%01d\t%03d\t%03d\n", col_idx, perf->cols[col_idx].cyc_act, perf->cols[col_idx].cyc_stl );
    }
#endif //PERF_PRINT_COLUMN_STATS

#if PERF_PRINT_LATEX
#if PERF_PRINT_TABBED
    PRINTF("\n===========\n LATEX VERSION BELOW \n===========\n");
#endif // PERF_PRINT_TABBED
    PRINTF("\\begin{table}[h!] \n");
    PRINTF("\t\\centering \n");
    PRINTF("\t\\caption{\\small Some table caption} \n");
    PRINTF("\t\\begin{tabular}{lll} \n");
    PRINTF("\t\t\\toprule \n");

    PRINTF("\t\t\\textbf{Parameter}&\\textbf{Value}&\\textbf{Unit}\\\\\n");

    PRINTF("\t\t\\midrule \n");

    PRINTF("\t\tActive&%03d&cycles\\\\\n", perf->cols_total.cyc_act + perf->cols_total.cyc_stl );
    PRINTF("\t\tAct/Stl&%d.%01d\\%%&- \\\\\n", perf->cyc_ratio / 10, perf->cyc_ratio % 10  ); 
    PRINTF("\t\tSoftware&%d&sec\\\\\n",  perf->time.sw.spent_cy );
    PRINTF("\t\tCGRA&%d&sec\\\\\n", perf->time.cgra.spent_cy );

    PRINTF("\t\t\\bottomrule \n");
	PRINTF("\t\\end{tabular} \n");
    PRINTF("\\end{table} \n");
#endif //PERF_PRINT_LATEX

#if PERF_PRINT_TABBED
#if PERF_PRINT_LATEX
    PRINTF("\n===========\n TABBED VERSION BELOW \n===========\n");
#endif //PERF_PRINT_LATEX
    PRINTF("Param\tValue\tUnit \n");
    PRINTF("Total\t%03d\tcycles \n", perf->cols_total.cyc_act + perf->cols_total.cyc_stl );
    PRINTF("Act/Stl\t%d.%01d\t%% \n", perf->cyc_ratio / 10, perf->cyc_ratio % 10  ); 
    PRINTF("Sw\t%d\tcy\n",  perf->time.sw.spent_cy );
    PRINTF("Conf\t%d\tcy\n", perf->time.config.spent_cy );
    PRINTF("CGRA\t%d\tcy\n", perf->time.cgra.spent_cy );
    PRINTF("Active\t%d\tcy\n", perf->time.cgra.spent_cy * ( 1000 - perf->cyc_ratio )/1000 );
#endif // PERF_PRINT_TABBED

}

void kcom_printKernelStats( kcom_stats_t *stats  )
{
    PRINTF("KERNEL STATS\n");
    PRINTF("PARA\tAVG(cy)\tVAR\n");
    PRINTF("SOFT\t%d\t%0d.%01d\n", stats->avg.sw, stats->var.sw/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.sw%CGRA_STAT_PERCENT_MULTIPLIER );
    PRINTF("CONF\t%d\t%0d.%01d\n", stats->avg.config, stats->var.config/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.config%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("CGRA\t%d\t%0d.%01d\n", stats->avg.cgra, stats->var.cgra/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.cgra%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("A/St\t%0d.%01d%%\t%0d.%01d\n", stats->avg.cyc_ratio/10,stats->avg.cyc_ratio%10, stats->var.cyc_ratio/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.cyc_ratio%CGRA_STAT_PERCENT_MULTIPLIER);
}

void kcom_printSummary( cgra_t *cgra )
{
    PRINTF("\nCGRA kernels executed: %d\n", cgra_perf_cnt_get_kernel(cgra));
    PRINTF("Clock freq: %d MHz\n", freq_hz/1000000);
}



/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                                 EOF                                      */
/**                                                                        **/
/****************************************************************************/

// Juan: make different parameters be accesible without having to repeat N times the same lines!