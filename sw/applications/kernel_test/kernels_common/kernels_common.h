/*
                              *******************
******************************* H HEADER FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : kernels_common.h                                             **
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
* @file   kernel_common.h
* @date   05/04/23
* @brief  The declarations for the common kernel module.
*
*/

#ifndef _KERNEL_COMMON_H
#define _KERNEL_COMMON_H

/****************************************************************************/
/**                                                                        **/
/**                            MODULES USED                                **/
/**                                                                        **/
/****************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "cgra.h"
#include "cgra_x_heep.h"


// For the timer
#include "rv_timer.h"
#include "soc_ctrl.h"
#include "core_v_mini_mcu.h"

/****************************************************************************/
/**                                                                        **/
/**                       DEFINITIONS AND MACROS                           **/
/**                                                                        **/
/****************************************************************************/

/* Operation Configurations */
#define ENABLE_PRINTF           0
#define ENABLE_DEBUG_PRINTF     0

#define PRINT_ITERATION_VALUES  0
#define PRINT_KERNEL_STATS      0  
#define PRINT_COLUMN_STATS      0
#define PRINT_LATEX             0
#define PRINT_TABBED            0
#define PRINT_PLOT              0

#define ENABLE_PIN_TOGGLE       1
#define ENABLE_TIME_MEASURE     1
#define REPEAT_FIRST_INPUT      1


#define PIN_TO_TOGGLE           30
#define ITERATIONS_PER_KERNEL   3
#define PERF_PLOT_SCALE_DOWN    5 


/* Constants */
#define CGRA_STAT_PERCENT_MULTIPLIER    100
#define CGRA_MAX_COLS                   4
#define HART_ID                         0
#define KCOM_FUNC_RET_OK                0
#define TICK_FREQ_HZ                    20 * 1000 * 1000 // 20 MHz
#define KERNEL_NAME_LENGTH_MAX          20
#define RANDOM_SEED                     12346

#define CGRA_ACCESS_FLAT_COST_CYCLES    55  // Measured in Questasim, do not change

/* Macros */

#if ENABLE_PRINTF
    #define PRINTF(fmt, ...)    printf(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif //ENABLE_PRINTF
#if ENABLE_DEBUG_PRINTF
    #define PRINTDBG(fmt, ...)  printf(fmt, ## __VA_ARGS__)
#else
    #define PRINTDBG(...)
#endif //ENABLE_DEBUG_PRINTF
#define PRINTF_ALWAYS(fmt, ...) printf(fmt, ## __VA_ARGS__)


/****************************************************************************/
/**                                                                        **/
/**                       TYPEDEFS AND STRUCTURES                          **/
/**                                                                        **/
/****************************************************************************/

typedef uint32_t    kcom_func_ret_t;

typedef uint32_t    kcom_time_t;

typedef uint32_t*   kcom_mem_t;

typedef int32_t*    kcom_io_t;

typedef kcom_time_t kcom_param_t;

typedef struct 
{
    kcom_mem_t  kmem;
    kcom_mem_t  imem;
    kcom_io_t   input;
    kcom_io_t   output;
    uint8_t     col_n;
    void        ( *config ) (void);
    void        ( *func )   (void);
    uint32_t    ( *check )  (void);
    int8_t      name[ KERNEL_NAME_LENGTH_MAX ];
} kcom_kernel_t;


typedef struct
{
    uint32_t cyc_act;
    uint32_t cyc_stl;
} kcom_col_perf_t;

typedef struct
{
    kcom_time_t start_cy;
    kcom_time_t end_cy;
    kcom_time_t spent_cy;
} kcom_time_diff_t;

typedef struct 
{
    kcom_time_diff_t    sw;
    kcom_time_diff_t    cgra;
    kcom_time_diff_t    load;
    kcom_time_diff_t    conf;
    kcom_time_diff_t    dead;
    rv_timer_t          timer;
} kcom_timing_t;

typedef struct
{
    kcom_col_perf_t    cols[CGRA_MAX_COLS];
    kcom_col_perf_t    cols_max;
    uint32_t           cyc_ratio; // Stored *CGRA_STAT_PERCENT_MULTIPLIER 
    kcom_timing_t      time;
} kcom_perf_t;

typedef struct 
{
    kcom_param_t sw;
    kcom_param_t conf;
    kcom_param_t cgra;
    kcom_param_t repo;
    kcom_param_t repo_conf;
    kcom_param_t cyc_ratio;
} kcom_run_t;

typedef struct
{
   kcom_run_t   avg;
   kcom_run_t   stdev;
   uint32_t     n;
   uint32_t     errors;
   uint8_t      *name;
} kcom_stats_t;

/****************************************************************************/
/**                                                                        **/
/**                          EXPORTED VARIABLES                            **/
/**                                                                        **/
/****************************************************************************/



/****************************************************************************/
/**                                                                        **/
/**                          EXPORTED FUNCTIONS                            **/
/**                                                                        **/
/****************************************************************************/
uint32_t kcom_getRand();
void kcom_resetRand();

void kcom_timerInit(    rv_timer_t *timer);
uint64_t kcom_getTime(  rv_timer_t *timer );
void kcom_timeStart(    kcom_time_diff_t    *perf, rv_timer_t   *timer );
void kcom_timeStop(     kcom_time_diff_t    *perf, rv_timer_t   *timer );
void kcom_subtractDead( kcom_time_t         *time, kcom_time_t  dead );

void kcom_getPerf(          cgra_t      *cgra,  kcom_perf_t     *perf );
void kcom_populateRun(      kcom_run_t  *run,   kcom_perf_t     *perf, uint32_t it_idx );
void kcom_extractConfTime(  kcom_run_t  *run,   uint32_t        it_n );
void kcom_getKernelStats(   kcom_run_t  *run,   kcom_stats_t    *stats );

void kcom_printPerf(        cgra_t          *cgra, kcom_perf_t *perf );
void kcom_printKernelStats( kcom_stats_t    *stats );
void kcom_printSummary(     cgra_t          *cgra );

/****************************************************************************/
/**                                                                        **/
/**                          INLINE FUNCTIONS                              **/
/**                                                                        **/
/****************************************************************************/



#endif /* _KERNEL_COMMON_H*/
/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/























