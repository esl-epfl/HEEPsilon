#include "performance.h"

// For the timer
#include "rv_timer.h"
#include "soc_ctrl.h"
#include "core_v_mini_mcu.h"

// Timer
static rv_timer_t          timer;

void kcom_perfRecordStart( kcom_time_diff_t *perf )
{
    timeStart( perf );
}

void kcom_perfRecordStop( kcom_time_diff_t *perf )
{
    timeStop( perf );
}

void timeStart( kcom_time_diff_t *perf )
{
    perf->start_cy = getTime_cy();
}

void timeStop( kcom_time_diff_t *perf )
{
    perf->end_cy = getTime_cy();
    perf->spent_cy += perf->end_cy - perf->start_cy;
}

uint64_t getTime_cy( )
{
    static uint64_t out;
    rv_timer_counter_read( &timer, HART_ID, &out );
    return out;
}

//Initialize the timer
void timerInit()
{
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr  = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
    uint32_t freq_hz  = soc_ctrl_get_frequency(&soc_ctrl);

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