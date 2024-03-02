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

// For GPIO managing
#include "gpio.h"
#include "pad_control.h"
#include "pad_control_regs.h"

// For interrupt handling
#include "csr.h"
#include "handler.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"

// For the timer
#include "rv_timer.h"
#include "soc_ctrl.h"
#include "core_v_mini_mcu.h"

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

inline __attribute__((always_inline)) void pinHigh( uint8_t pin );
inline __attribute__((always_inline)) void pinLow(  uint8_t pin );

uint64_t    getTime_cy();
void        timeStart(    kcom_time_diff_t    *perf );
void        timeStop(     kcom_time_diff_t    *perf );

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

static uint32_t freq_hz;

static uint32_t z1 = RANDOM_SEED, \
                z2 = RANDOM_SEED, \
                z3 = RANDOM_SEED, \
                z4 = RANDOM_SEED;

// Controlling a pin
static gpio_t  gpio;

// Timer
static rv_timer_t          timer;

// Plic controller variables
volatile bool               cgra_intr_flag;

static cgra_t               cgra;
static uint8_t              cgra_slot;
// To stop the counter inside the interupt handler
static kcom_time_diff_t     *cgraPerf;

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED FUNCTIONS                             */
/**                                                                        **/
/****************************************************************************/

void plic_interrupt_init() {
    // Init the PLIC
    plic_Init();
    plic_irq_set_priority(CGRA_INTR, 1);
    plic_irq_set_enabled(CGRA_INTR, kPlicToggleEnabled);

    // Enable interrupt on processor side
    // Enable global interrupt for machine-level interrupts
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
    // Set mie.MEIE bit to one to enable machine-level external interrupts
    const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
    CSR_SET_BITS(CSR_REG_MIE, mask);
    cgra_intr_flag = 0;
}

// Interrupt controller variables
void handler_irq_ext(uint32_t id) {
  if( id == CGRA_INTR) {
    kcom_perfRecordStop( cgraPerf );
    cgra_intr_flag = 1;
  }
}

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

void kcom_subtractDead( kcom_time_t *time, kcom_time_t dead )
{
#if ENABLE_TIME_MEASURE
    *time -= dead;
#endif //ENABLE_TIME_MEASURE
}

void kcom_newVCDfile()
{
#if CTRL_VCD_W_PIN
    pinHigh( PIN_TO_NEW_VCD );
    pinLow(  PIN_TO_NEW_VCD );
#endif
}

void kcom_perfRecordStart( kcom_time_diff_t *perf )
{
#if CTRL_VCD_W_PIN
    pinHigh( PIN_TO_CTRL_VCD );
#endif //CTRL_VCD_W_PIN

#if ENABLE_TIME_MEASURE
    timeStart( perf );
#endif //ENABLE_TIME_MEASURE
}

void kcom_perfRecordStop( kcom_time_diff_t *perf )
{
#if CTRL_VCD_W_PIN
    pinLow( PIN_TO_CTRL_VCD );
#endif //CTRL_VCD_W_PIN

#if ENABLE_TIME_MEASURE
    timeStop( perf );
#endif //ENABLE_TIME_MEASURE
}

void kcom_perfRecordIntrSet( kcom_time_diff_t *perf )
{
    cgraPerf = perf;
    cgra_intr_flag = 0;
}

/* COMPUTATIONS */

void kcom_populateRun( kcom_run_t *run, kcom_perf_t *perf, uint32_t it_idx )
{
#if ENABLE_TIME_MEASURE
    // These times have already been subtracted the dead time.
#if EXECUTE_SOFTWARE
    (run[ it_idx ]).sw          = perf->time.sw.spent_cy;
#endif //EXECUTE_SOFTWARE
    (run[ it_idx ]).conf        = 0;
    (run[ it_idx ]).cgra        = perf->time.cgra.spent_cy;
    (run[ it_idx ]).loading_result = perf->time.loading_result.spent_cy;
    (run[ it_idx ]).repo        = perf->cols_max.cyc_act + perf->cols_max.cyc_stl;
#if MEASURE_RATIO
    (run[ it_idx ]).repo_conf   = 0;
    (run[ it_idx ]).cyc_ratio   = perf->cyc_ratio;
#endif //MEASURE_RATIO

#if PRINT_ITERATION_VALUES
    PRINTF("i%d\tSW:%d\tCG:%d\n",it_idx, (run[ it_idx ]).sw, (run[ it_idx ]).cgra );
#endif //PRINT_ITERATION_VALUES
#endif //ENABLE_TIME_MEASURE
}

void kcom_extractConfTime( kcom_run_t *run, uint32_t it_n )
{
#if ENABLE_TIME_MEASURE
#if REPEAT_FIRST_INPUT
    run[0].conf         = run[0].cgra - run[1].cgra;
    // run[0].repo_conf    = run[0].repo - run[1].repo;

    for( uint32_t i = 0; i < it_n; i++ )
    {
        run[i].conf         = run[0].conf;
        // run[i].repo_conf    = run[0].repo_conf;
    }
#endif //REPEAT_FIRST_INPUT
    kcom_param_t avg        = 0;
    kcom_param_t repo_avg   = 0;
    for( uint32_t i = 1; i < it_n; i++ )
    {
        run[i].conf         = run[0].cgra - run[i].cgra;
        // run[i].repo_conf    = run[0].repo - run[i].repo;
        avg         += run[i].conf;
        // repo_avg    += run[i].repo_conf;
    }
    avg         /= it_n;
    repo_avg    /= it_n;

    run[0].conf         = avg;
    // run[0].repo_conf    = repo_avg;

    run[0].cgra         = run[0].cgra - avg;
    run[0].repo         = run[0].repo - repo_avg;
#endif //ENABLE_TIME_MEASURE
}

void kcom_getKernelStats( kcom_run_t *run, kcom_stats_t *stats )
{
#if ENABLE_TIME_MEASURE
    kcom_run_t *avg     = &( stats->avg );
#if MEASURE_DEVIATION
    kcom_run_t *stdev   = &( stats->stdev );
#endif //MEASURE_DEVIATION
    uint32_t iterations = stats->n - 1; // Because the first iteration is discarded

    uint8_t paramCount = sizeof( kcom_run_t ) / sizeof( kcom_param_t );

    for( uint8_t p_idx = 0; p_idx < paramCount; p_idx++ )
    {
        /* Get the population average. */
        ((kcom_param_t*)avg)[p_idx] = 0;
        for( uint32_t it_idx = 1; it_idx < iterations+1; it_idx++ )
        {
            /* Get the sum of values. */
            ((kcom_param_t*)avg)[p_idx] += ((kcom_param_t*)(&(run[it_idx])))[p_idx];
        }
        /* Divide by the population size. */
        ((kcom_param_t*)avg)[p_idx] /= iterations;

#if MEASURE_DEVIATION
        /* Get the deviation (sigma). */
        ((kcom_param_t*)stdev)[p_idx] = 0;
        for( uint32_t it_idx = 1; it_idx < iterations+1; it_idx++ )
        {
             /* Get diff with avg. */
            int32_t diff = (int32_t)(((int32_t)((kcom_param_t*)&(run[it_idx]))[p_idx]) -  (int32_t)(((kcom_param_t*)avg)[p_idx]));
            /* Square it. Its multiplied by 100 because otherwise the next division would be < 1 */
            ((kcom_param_t*)stdev)[p_idx] += diff * diff * CGRA_STAT_PERCENT_MULTIPLIER;
        }
        /* Divide by the population size. */
        ((kcom_param_t*)stdev)[p_idx] /= iterations;
        kcom_param_t value = ((kcom_param_t*)stdev)[p_idx];
        if( value != 0 )
        {
            kcom_param_t sqr = value / 2;
            kcom_param_t temp;
            while( ( sqr != temp ) && ( sqr + 1 != temp ) ){
                temp = sqr;
                sqr = ( value/temp + temp) / 2;
            }
            ((kcom_param_t*)stdev)[p_idx] = sqr *10; //To recover a 10 factor we lost during sqrt
        }
#endif //MEASURE_DEVIATION
    }

#endif //ENABLE_TIME_MEASURE
}

void kcom_getPerf( kcom_perf_t *perf )
{
    perf->cols_max.cyc_act = 0;
    perf->cols_max.cyc_stl = 0;
    for(int8_t col_idx = 0 ; col_idx < CGRA_N_COLS ; col_idx++)
    {
        perf->cols[col_idx].cyc_act    = cgra_perf_cnt_get_col_active(&cgra, col_idx);
        perf->cols[col_idx].cyc_stl    = cgra_perf_cnt_get_col_stall (&cgra, col_idx);
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

void kcom_printPerf( kcom_perf_t *perf )
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
#if ENABLE_TIME_MEASURE
    PRINTF("\t\tSoftware&%d&sec\\\\\n",  perf->time.sw.spent_cy );
    PRINTF("\t\tCGRA&%d&sec\\\\\n", perf->time.cgra.spent_cy );
#endif //ENABLE_TIME_MEASURE

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
#if ENABLE_TIME_MEASURE
    // PRINTF("\n===================\n %s \n", stats->name);
    // PRINTF("PARA\tAVG(cy)\tDEV\n");
#if MEASURE_DEVIATION
#if EXECUTE_SOFTWARE
    PRINTF("SOFT\t%d\t%0d.%01d\n", stats->avg.sw, stats->stdev.sw/CGRA_STAT_PERCENT_MULTIPLIER,stats->stdev.sw%CGRA_STAT_PERCENT_MULTIPLIER );
#endif //EXECUTE_SOFTWARE
    PRINTF("CONF\t%d\t%0d.%01d\n", stats->avg.conf, stats->stdev.conf/CGRA_STAT_PERCENT_MULTIPLIER,stats->stdev.conf%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("REPO\t%d\t%0d.%01d\n", stats->avg.repo, stats->stdev.repo/CGRA_STAT_PERCENT_MULTIPLIER,stats->stdev.repo%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("CGRA\t%d\t%0d.%01d\n", stats->avg.cgra, stats->stdev.cgra/CGRA_STAT_PERCENT_MULTIPLIER,stats->stdev.cgra%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("LOADING OUTPUT\t%d\t%0d.%01d\n", stats->avg.loading_result, stats->stdev.loading_result/CGRA_STAT_PERCENT_MULTIPLIER,stats->stdev.loading_result%CGRA_STAT_PERCENT_MULTIPLIER);
#if MEASURE_RATIO
    PRINTF("St/A\t%0d.%01d%%\t%0d.%01d\n", stats->avg.cyc_ratio/10,stats->avg.cyc_ratio%10, stats->stdev.cyc_ratio/CGRA_STAT_PERCENT_MULTIPLIER,stats->stdev.cyc_ratio%CGRA_STAT_PERCENT_MULTIPLIER);
    PRINTF("CG/S\t%0d%%\t-\n",(stats->avg.cgra*CGRA_STAT_PERCENT_MULTIPLIER/stats->avg.sw));
#endif //MEASURE_RATIO
#else
#if EXECUTE_SOFTWARE
    PRINTF("SOFT\t%d\n", stats->avg.sw );
#endif //EXECUTE_SOFTWARE
    PRINTF("CONF\t%d\n", stats->avg.conf );
    PRINTF("REPO\t%d\n", stats->avg.repo );
    PRINTF("CGRA\t%d\n", stats->avg.cgra );
#endif
#endif //ENABLE_TIME_MEASURE
}

void kcom_printSummary( kcom_stats_t *stats  )
{
    PRINTF("E\t%d\n", stats->errors );
}


/*  GENERAL */

void kcom_init()
{
    pinInit();
    timerInit();
    plic_interrupt_init( CGRA_INTR );
}

void kcom_load( kcom_kernel_t *ker )
{
    cgra_cmem_init(ker->imem, ker->kmem );

    cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);
    // Select request slot of CGRA
    cgra_slot = cgra_get_slot(&cgra);
    cgra_perf_cnt_enable(&cgra, 1);
    // Set CGRA kernel L/S pointers
    for(int8_t col_idx = 0 ; col_idx < ker->col_n ; col_idx++){
        cgra_set_read_ptr ( &cgra, cgra_slot, (uint32_t)&((ker->input[0]))  + (col_idx * ker->in_n  * sizeof(uint32_t) ),  col_idx );
        cgra_set_write_ptr( &cgra, cgra_slot, (uint32_t)&((ker->output[0])) + (col_idx * ker->out_n * sizeof(uint32_t) ),  col_idx );
    }
}

void kcom_rstPerfCounter()
{
    cgra_perf_cnt_reset( &cgra );
}

void kcom_launchKernel( uint8_t Id )
{
    cgra_set_kernel( &cgra, cgra_slot, Id );
}

__attribute__((optimize("O0"))) void kcom_waitingForIntr()
{
#if WATCHDOG_CYCLES
    uint32_t cycles = WATCHDOG_CYCLES;
    while( cgra_intr_flag == 0 && cycles > 0){ cycles--; }
#else
    while( cgra_intr_flag == 0 );
#endif //WATCHDOG_CYCLES
}

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

inline __attribute__((always_inline)) void pinHigh( uint8_t pin )
{
    gpio_write(&gpio, pin, true );
}

inline __attribute__((always_inline)) void pinLow( uint8_t pin )
{
    gpio_write(&gpio, pin, false );
}

void pinInit()
{
#if CTRL_VCD_W_PIN
    gpio_result_t gpio_res;
    gpio_params_t gpio_params;
    pad_control_t pad_control;

    pad_control.base_addr = mmio_region_from_addr((uintptr_t)PAD_CONTROL_START_ADDRESS);
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_START_ADDRESS);

    gpio_init(gpio_params, &gpio);

    gpio_write(&gpio, PIN_TO_NEW_VCD, false);
    gpio_write(&gpio, PIN_TO_CTRL_VCD, false);

    gpio_output_set_enabled(&gpio, PIN_TO_NEW_VCD, true);
    gpio_output_set_enabled(&gpio, PIN_TO_CTRL_VCD, true);

    pad_control_set_mux(&pad_control, (ptrdiff_t)(PAD_CONTROL_PAD_MUX_I2C_SDA_REG_OFFSET), 1);
#endif
}

void timerInit()
{
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr  = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
    freq_hz             = soc_ctrl_get_frequency(&soc_ctrl);

    mmio_region_t timer_0_reg = mmio_region_from_addr(RV_TIMER_AO_START_ADDRESS);

    rv_timer_init( timer_0_reg, (rv_timer_config_t) { .hart_count = 2, .comparator_count = 1 }, &timer );

    rv_timer_tick_params_t tick_params;

    // The same frequency is provaided to get one tick per cycle.
    rv_timer_approximate_tick_params( freq_hz, freq_hz, &tick_params );
    rv_timer_set_tick_params(&timer, HART_ID, tick_params);

    // Juan: see if i cannot remove this!
    rv_timer_irq_enable(&timer, HART_ID, 0, kRvTimerEnabled);
    rv_timer_arm(&timer, HART_ID, 0, 1);

    rv_timer_counter_set_enabled(&timer, HART_ID, kRvTimerEnabled);

}

uint64_t getTime_cy( )
{
    static uint64_t out;
    rv_timer_counter_read( &timer, HART_ID, &out );
    return out;
}

void timeStart( kcom_time_diff_t *perf )
{
#if ENABLE_TIME_MEASURE
    perf->start_cy = getTime_cy();
#endif //ENABLE_TIME_MEASURE
}

void timeStop( kcom_time_diff_t *perf )
{
#if ENABLE_TIME_MEASURE
    perf->end_cy = getTime_cy();
    perf->spent_cy = perf->end_cy - perf->start_cy;
#endif //ENABLE_TIME_MEASURE
}

/****************************************************************************/
/**                                                                        **/
/*                                 EOF                                      */
/**                                                                        **/
/****************************************************************************/