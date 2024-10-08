// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef _CGRA_H_
#define _CGRA_H_

#include <stdint.h>

#include "mmio.h"
#include "core_v_mini_mcu.h"

#define CGRA_INTR EXT_INTR_0

#define CGRA_N_COLS   ${cgra_num_columns}
#define CGRA_N_ROWS   ${cgra_num_rows}
#define CGRA_MAX_COLS ${cgra_max_columns}

#define CGRA_KMEM_DEPTH     ${cgra_kmem_depth}
#define CGRA_KMEM_WIDTH     ${cgra_kmem_width}

#define CGRA_CMEM_BK_DEPTH      ${cgra_cmem_bk_depth}
#define CGRA_CMEM_BK_DEPTH_LOG2 ${cgra_cmem_bk_depth_log2}
#define CGRA_CMEM_TOT_DEPTH     (CGRA_CMEM_BK_DEPTH*CGRA_N_ROWS)

#define CGRA_RCS_NUM_CREG      ${cgra_rcs_num_instr}
#define CGRA_RCS_NUM_CREG_LOG2 ${cgra_rcs_num_instr_log2}

// Some of these checks are already done during the bitstream generation but better double check them
#if CGRA_CMEM_TOT_DEPTH < ${cgra_num_rows*cgra_max_columns*cgra_rcs_num_instr}
  #warning Context memory cannot hold the maximum kernel size
#endif
#if CGRA_MAX_COLS > CGRA_N_COLS
  #error CGRA configuration: more columns than possible can be used
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write the CGRA bistream to its memory
 */
void cgra_cmem_init(uint32_t cgra_cmem_bitstream[], uint32_t cgra_kmem_bitstream[]);

/**
 * Initialization parameters for CGRA peripheral control registers..
 *
 */
typedef struct cgra {
  /**
   * The base address for the soc_ctrl hardware registers.
   */
  mmio_region_t base_addr;
} cgra_t;

/**
 * Write to read_ptr register of the CGRA control registers.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param read_ptr Any valid memory address.
 * @param column_idx Column number to which write the read_ptr address
 */
void cgra_set_read_ptr(const cgra_t *cgra, uint32_t read_ptr, uint8_t column_idx);

/**
 * Write to write_ptr register of the CGRA control registers.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param write_ptr Any valid memory address.
 * @param column_idx Column number to which write the write_ptr address
 */
void cgra_set_write_ptr(const cgra_t *cgra, uint32_t write_ptr, uint8_t column_idx);

/**
 * Wait until the CGRA is ready to accept a new request.
 * @param cgra Pointer to cgra_t representing the target CGRA peripheral.
 */
void cgra_wait_ready(const cgra_t *cgra);

/**
 * Write to kernel_id register of the CGRA control registers.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param kernel_id Kernel ID to execute
 */
void cgra_set_kernel(const cgra_t *cgra, uint32_t kernel_id);

/**
 * Enable/disable CGRA performance counters.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param enable (0: disable, 1:enable)
 */
void cgra_perf_cnt_enable(const cgra_t *cgra, bool enable);

/**
 * Reset CGRA performance counters.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 */
void cgra_perf_cnt_reset(const cgra_t *cgra);

/**
 * Get the number of executed kernels from performance counter.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @return Performance counter measuring the number of executed kernels.
 */
uint32_t cgra_perf_cnt_get_kernel(const cgra_t *cgra);

/**
 * Get the number of active cycles (configuration + execution) from performance counter.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param column_idx Column index from which get the performance counter.
 * @return Performance counter measuring the number of active cycles.
 */
uint32_t cgra_perf_cnt_get_col_active(const cgra_t *cgra, uint8_t column_idx);

/**
 * Get the number of stall cycles because data accesses from performance counter.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param column_idx Column index from which get the performance counter.
 * @return Performance counter measuring the number of stall cycles.
 */
uint32_t cgra_perf_cnt_get_col_stall(const cgra_t *cgra, uint8_t column_idx);

/**
 * Get CGRA columns' status (1 bit per column: 0=free, 1=used).
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param column_idx Column index from which get the performance counter.
 * @return CGRA columns' status.
 */
uint32_t cgra_get_status(const cgra_t *cgra);

#ifdef __cplusplus
}
#endif

#endif // _CGRA_H_
