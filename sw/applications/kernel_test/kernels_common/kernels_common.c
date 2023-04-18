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

#define PERF_PLOT_SCALE_DOWN    5 

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

static uint32_t z1 = RANDOM_SEED, \
                z2 = RANDOM_SEED, \
                z3 = RANDOM_SEED, \
                z4 = RANDOM_SEED;

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED FUNCTIONS                             */
/**                                                                        **/
/****************************************************************************/

/* MISCELANEOUS */

uint32_t kcom_getRand()
{
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

void kcom_resetRand()
{
    z1 = z2 = z3 = z4 = RANDOM_SEED;
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

void kcom_subtractDead( kcom_time_t *time, kcom_time_t dead )
{
    *time -= dead;
}

/* COMPUTATIONS */

void kcom_populateRun( kcom_run_t *run, kcom_perf_t *perf, uint32_t it_idx )
{
    // These times have already been subtracted the dead time.
    (run[ it_idx ]).sw          = perf->time.sw.spent_cy;     
    (run[ it_idx ]).conf        = 0;     
    (run[ it_idx ]).cgra        = perf->time.cgra.spent_cy;  
    (run[ it_idx ]).repo        = perf->cols_max.cyc_act + perf->cols_max.cyc_stl;
    (run[ it_idx ]).repo_conf   = 0;   
    (run[ it_idx ]).cyc_ratio   = perf->cyc_ratio; 
    PRINTDBG("sw: %03d \t conf: %03d \t cgra: %03d \t repo: %03d \t repo_conf: %03d \t cyc_ratio: %03d\n", 
    (run[ it_idx ]).sw, (run[ it_idx ]).conf, (run[ it_idx ]).cgra, (run[ it_idx ]).repo, (run[ it_idx ]).repo_conf, (run[ it_idx ]).cyc_ratio );
}

void kcom_extractConfTime( kcom_run_t *run, uint32_t it_n )
{
#if REPEAT_FIRST_INPUT
    PRINTDBG("-----\n");
    run[0].conf         = run[0].cgra - run[1].cgra;
    run[0].repo_conf    = run[0].repo - run[1].repo;

    for( uint32_t i = 0; i < it_n; i++ )
    {
        run[i].cgra         = run[i].cgra - run[0].conf;
        run[i].conf         = run[0].conf;
        run[i].repo_conf    = run[0].repo_conf; 
        PRINTDBG("sw: %03d \t conf: %03d \t cgra: %03d \t repo: %03d \t repo_conf: %03d \t cyc_ratio: %03d\n", 
        (run[ i ]).sw, (run[ i ]).conf, (run[ i ]).cgra, (run[ i ]).repo, (run[ i ]).repo_conf, (run[ i ]).cyc_ratio );
    }
#else
    kcom_param_t avg        = 0;
    kcom_param_t repo_avg   = 0;
    for( uint32_t i = 1; i < it_n; i++ )
    {
        run[i].conf         = run[0].cgra - run[i].cgra;
        run[i].repo_conf    = run[0].repo - run[i].repo;
        avg         += run[i].conf;
        repo_avg    += run[i].repo_conf;
    }
    avg         /= it_n;
    repo_avg    /= it_n;

    run[0].conf         = avg;
    run[0].repo_conf    = repo_avg;

    run[0].cgra         = run[0].cgra - avg;  
    run[0].repo         = run[0].repo - repo_avg; 
#endif //REPEAT_FIRST_INPUT

}


void kcom_getKernelStats( kcom_run_t *run, kcom_stats_t *stats )
{
    kcom_run_t *avg = &( stats->avg );
    kcom_run_t *var = &( stats->var );
    uint32_t iterations = stats->n - 1; // Because the first iteration is discarded

    
    uint8_t paramCount = sizeof( kcom_run_t ) / sizeof( kcom_param_t );

    for( uint8_t p_idx = 0; p_idx < paramCount; p_idx++ )
    {
        /* Get the population average. */
        ((kcom_param_t*)avg)[p_idx] = 0;
        for( uint32_t it_idx = 0; it_idx < iterations; it_idx++ )
        {
            /* Get the sum of values. */
            ((kcom_param_t*)avg)[p_idx] += ((kcom_param_t*)(&(run[it_idx])))[p_idx];
        }
        /* Divide by the population size. */
        ((kcom_param_t*)avg)[p_idx] /= iterations;
    
        /* Get the variance (sigma^2). */
        ((kcom_param_t*)var)[p_idx] = 0; 
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
    perf->cols_max.cyc_act = 0;
    perf->cols_max.cyc_stl = 0;
    for(int8_t col_idx = 0 ; col_idx < CGRA_MAX_COLS ; col_idx++)
    {
        perf->cols[col_idx].cyc_act    = cgra_perf_cnt_get_col_active(cgra, col_idx);
        perf->cols[col_idx].cyc_stl    = cgra_perf_cnt_get_col_stall (cgra, col_idx);
        if( perf->cols[col_idx].cyc_act > perf->cols_max.cyc_act )
        {
            perf->cols_max.cyc_act       = perf->cols[col_idx].cyc_act;
        }
        if( perf->cols[col_idx].cyc_stl > perf->cols_max.cyc_stl )
        {
            perf->cols_max.cyc_stl       += perf->cols[col_idx].cyc_stl;
        }
    }
    perf->cyc_ratio = perf->cols_max.cyc_stl*CGRA_STAT_PERCENT_MULTIPLIER*10 / perf->cols_max.cyc_act;
}

/* PRESENTING THE RESULTS */

void kcom_printPerf( cgra_t *cgra, kcom_perf_t *perf )
{

#if PRINT_PLOT
    for( uint16_t i = 0; i < perf->time.cgra.spent_cy; i += PERF_PLOT_SCALE_DOWN )
    {
        PRINTF(" ");
    }
    PRINTF("*\n");
#endif

#if PRINT_COLUMN_STATS
    PRINTF("\n===========\n COLUMN STATS BELOW \n===========\n");
    PRINTF("Col\tAct\tStl\n");
    for(int8_t col_idx = 0 ; col_idx < CGRA_MAX_COLS ; col_idx++)
    {
        PRINTF("%01d\t%03d\t%03d\n", col_idx, perf->cols[col_idx].cyc_act, perf->cols[col_idx].cyc_stl );
    }
    PRINTF("Max:\t%03d\t%03d\n", perf->cols_max.cyc_act, perf->cols_max.cyc_stl );
#endif //PRINT_COLUMN_STATS

#if PRINT_LATEX
#if PRINT_TABBED
    PRINTF("\n===========\n LATEX VERSION BELOW \n===========\n");
#endif // PRINT_TABBED
    PRINTF("\\begin{table}[h!] \n");
    PRINTF("\t\\centering \n");
    PRINTF("\t\\caption{\\small Some table caption} \n");
    PRINTF("\t\\begin{tabular}{lll} \n");
    PRINTF("\t\t\\toprule \n");

    PRINTF("\t\t\\textbf{Parameter}&\\textbf{Value}&\\textbf{Unit}\\\\\n");

    PRINTF("\t\t\\midrule \n");

    PRINTF("\t\tActive&%03d&cycles\\\\\n", perf->cols_max.cyc_act + perf->cols_max.cyc_stl );
    PRINTF("\t\tAct/Stl&%d.%01d\\%%&- \\\\\n", perf->cyc_ratio / 10, perf->cyc_ratio % 10  ); 
    PRINTF("\t\tSoftware&%d&sec\\\\\n",  perf->time.sw.spent_cy );
    PRINTF("\t\tCGRA&%d&sec\\\\\n", perf->time.cgra.spent_cy );

    PRINTF("\t\t\\bottomrule \n");
	PRINTF("\t\\end{tabular} \n");
    PRINTF("\\end{table} \n");
#endif //PRINT_LATEX

#if PRINT_TABBED
#if PRINT_LATEX
    PRINTF("\n===========\n TABBED VERSION BELOW \n===========\n");
#endif //PRINT_LATEX
    PRINTF("Param\tValue\tUnit \n");
    PRINTF("Total\t%d+%d=%03d\tcycles \n", perf->cols_max.cyc_act, perf->cols_max.cyc_stl,perf->cols_max.cyc_act + perf->cols_max.cyc_stl );
    PRINTF("Act/Stl\t%d.%01d\t%% \n", perf->cyc_ratio / 10, perf->cyc_ratio % 10  ); 
    PRINTF("Sw\t%d\tcy\n",  perf->time.sw.spent_cy );
    PRINTF("CGRA\t%d\tcy\n", perf->time.cgra.spent_cy );
    PRINTF("Active\t%d\tcy\n", perf->time.cgra.spent_cy * ( 1000 - perf->cyc_ratio )/1000 );
#endif // PRINT_TABBED

}

void kcom_printKernelStats( kcom_stats_t *stats  )
{
    PRINTF("\n===================\n %s \n", stats->name);
    PRINTF("PARA\tAVG(cy)\tVAR\n");
    PRINTF("SOFT\t%d\t%0d.%01d\n", stats->avg.sw, stats->var.sw/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.sw%CGRA_STAT_PERCENT_MULTIPLIER );
    PRINTF("CONF\t%d\t%0d.%01d\n", stats->avg.conf, stats->var.conf/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.conf%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("REPO\t%d\t%0d.%01d\n", stats->avg.repo, stats->var.repo/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.repo%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("CGRA\t%d\t%0d.%01d\n", stats->avg.cgra, stats->var.cgra/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.cgra%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("St/A\t%0d.%01d%%\t%0d.%01d\n", stats->avg.cyc_ratio/10,stats->avg.cyc_ratio%10, stats->var.cyc_ratio/CGRA_STAT_PERCENT_MULTIPLIER,stats->var.cyc_ratio%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("CG/S\t%0d%%\t-\n",(stats->avg.cgra*CGRA_STAT_PERCENT_MULTIPLIER/stats->avg.sw));
    PRINTF("Errs\t%d\n", stats->errors);
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