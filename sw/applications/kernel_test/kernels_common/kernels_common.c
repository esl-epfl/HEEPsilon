


#include "kernels_common.h"




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

void kcom_timerInit( rv_timer_t *timer)
{
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
    uint32_t freq_hz = soc_ctrl_get_frequency(&soc_ctrl);

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

void kcom_timeStart( kcom_time_diff_t *stats, rv_timer_t *timer )
{
    stats->start = kcom_getTime( timer );
}

void kcom_timeStop( kcom_time_diff_t *stats,  rv_timer_t *timer )
{
    stats->end = kcom_getTime( timer );
    stats->spent = stats->end - stats->start;
}

/* 
    CGRA performance results
*/
void kcom_printPerf( cgra_t *cgra, kcom_cgra_stats_t *stats ){
    PRINTF("CGRA kernels executed: %d\n", cgra_perf_cnt_get_kernel(cgra));
    PRINTF("Col\tAct\tStl\n");
    for(int8_t col_idx = 0 ; col_idx < CGRA_MAX_COLS ; col_idx++){
        stats->cols[col_idx].cyc_act    = cgra_perf_cnt_get_col_active(cgra, col_idx);
        stats->cols[col_idx].cyc_stl    = cgra_perf_cnt_get_col_stall (cgra, col_idx);
        stats->total.cyc_act            += stats->cols[col_idx].cyc_act;
        stats->total.cyc_stl            += stats->cols[col_idx].cyc_stl;
        
        PRINTF("%01d\t%03d\t%03d\n", col_idx, stats->cols[col_idx].cyc_act, stats->cols[col_idx].cyc_stl );
    }
    stats->cyc_ratio = stats->total.cyc_stl*CGRA_STAT_RATIO_MULTIPLIER*10 / stats->total.cyc_act;

    PRINTF("SUM\t%03d\t%03d\n",     stats->total.cyc_act, stats->total.cyc_stl );
    PRINTF("RATIO:\t%d.%01d%%\n",   stats->cyc_ratio / 10, stats->cyc_ratio % 10  ); 
    PRINTF("SW time:\t%d\n",        stats->time.sw.spent );
    PRINTF("CGRA time:\t%d\n",      stats->time.cgra.spent );
}
