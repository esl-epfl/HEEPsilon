#include <stdint.h>
#include "cgra.h"

typedef long int    kcom_time_t;
typedef kcom_time_t kcom_param_t;

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
    kcom_time_diff_t    input;
    kcom_time_diff_t    output;
    kcom_time_diff_t    reprogramCols;
    kcom_time_diff_t    bitstream;
} kcom_timing_t;

typedef struct
{
    kcom_col_perf_t    cols[CGRA_N_COLS];
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
} kcom_run_t;

typedef struct
{
   kcom_run_t   avg;
   uint32_t     n;
   uint32_t     errors;
   uint8_t      *name;
} kcom_stats_t;

#define HART_ID 0

void timerInit();
uint64_t getTime_cy( );
void timeStop( kcom_time_diff_t *perf );
void timeStart( kcom_time_diff_t *perf );
void kcom_perfRecordStop( kcom_time_diff_t *perf );
void kcom_perfRecordStart( kcom_time_diff_t *perf );