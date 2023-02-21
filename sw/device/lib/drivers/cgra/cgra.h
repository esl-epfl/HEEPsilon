////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                 Name Surname - email (affiliation if not ESL)              //
//                                                                            //
// File Name:      cgra.h                                                     //
//                                                                            //
// Project Name:   CGRA_ESL                                                   //
//                                                                            //
// Language:       C                                                          //
//                                                                            //
// Description:    CGRA parameters and control functions.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _CGRA_H_
#define _CGRA_H_

#include <stdint.h>

#include "mmio.h"
#include "core_v_mini_mcu.h"

#define CGRA_INTR EXT_INTR_0

#define CGRA_KMEM_SIZE 16
#define CGRA_IMEM_SIZE 512
#define CGRA_N_SLOTS   2
#define CGRA_N_COLS    4
#define CGRA_N_ROWS    4

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write the CGRA bistream to its memory
 */
void cgra_cmem_init(uint32_t cgra_imem_bistream[], uint32_t cgra_kem_bitstream[]);

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
 * @param slot_id Slot ID to which write the read_ptr address
 * @param read_ptr Any valid memory address.
 * @param column Column number to which write the read_ptr address
 */
void cgra_set_read_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t read_ptr, uint8_t column);

/**
 * Write to write_ptr register of the CGRA control registers.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param slot_id Slot ID to which write the write_ptr address
 * @param write_ptr Any valid memory address.
 * @param column Column number to which write the write_ptr address
 */
void cgra_set_write_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t write_ptr, uint8_t column);

/**
 * Get the first available slot from the CGRA.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @return Available slot number.
 */
uint32_t cgra_get_slot(const cgra_t *cgra);

/**
 * Write to kernel_id register of the CGRA control registers.
 * @param cgra Pointer to cgra_t represting the target CGRA peripheral.
 * @param slot_id Slot ID to which request the kernel_id
 * @param kernel_id Kernel ID to execute
 */
void cgra_set_kernel(const cgra_t *cgra, uint8_t slot_id, uint32_t kernel_id);

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