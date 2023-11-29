// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stddef.h>
#include <stdint.h>

#include "heepsilon.h"
#include "core_v_mini_mcu.h"
#include "cgra.h"
#include "cgra_regs.h" // generated

void cgra_cmem_init(uint32_t cgra_cmem_bitstream[], uint32_t cgra_kmem_bitstream[])
{
  int32_t *cgra_cmem_ptr;// = (int32_t*) (CGRA_START_ADDRESS);

  for (int i=0; i<CGRA_N_ROWS; i++) {
    // Update the pointer to the nexkt memory bank
    cgra_cmem_ptr = (int32_t*) (CGRA_START_ADDRESS) + i*((uint32_t)1<<CGRA_CMEM_BK_DEPTH_LOG2);
    for (int j=0; j<CGRA_CMEM_BK_DEPTH; j++) {
      *cgra_cmem_ptr++ = cgra_cmem_bitstream[j+i*CGRA_CMEM_BK_DEPTH];
    }
  }

  cgra_cmem_ptr = (int32_t*) (CGRA_START_ADDRESS) + CGRA_N_ROWS*((uint32_t)1<<CGRA_CMEM_BK_DEPTH_LOG2);
  for (int i=0; i<CGRA_KMEM_DEPTH; i++) {
    *cgra_cmem_ptr++ = cgra_kmem_bitstream[i];
  }
}

void cgra_set_read_ptr(const cgra_t *cgra, uint32_t read_ptr, uint8_t column_idx) {
  // Each column has 2 pointers so increment the address by 0x8 (i.e., 2 x 32 bit addresses) multiply by the column index
  mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_PTR_IN_COL_0_REG_OFFSET+0x8*column_idx), read_ptr);
}

void cgra_set_write_ptr(const cgra_t *cgra, uint32_t write_ptr, uint8_t column_idx) {
  // Each column has 2 pointers so increment the address by 0x8 (i.e., 2 x 32 bit addresses) multiply by the column index
  mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_PTR_OUT_COL_0_REG_OFFSET+0x8*column_idx), write_ptr);
}

void cgra_wait_ready(const cgra_t *cgra) {
  uint32_t cgra_req_free;
  // Wait until the CGRA can accept a new request
  do {
    cgra_req_free = mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_KERNEL_ID_REG_OFFSET));
  } while (cgra_req_free != 0);
}

void cgra_set_kernel(const cgra_t *cgra, uint32_t kernel_id) {
  mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_KERNEL_ID_REG_OFFSET), kernel_id);
}

void cgra_perf_cnt_enable(const cgra_t *cgra, bool enable) {
  mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_ENABLE_REG_OFFSET), enable);
}

void cgra_perf_cnt_reset(const cgra_t *cgra) {
  mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_RESET_REG_OFFSET), 1);
}

uint32_t cgra_perf_cnt_get_kernel(const cgra_t *cgra) {
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_TOTAL_KERNELS_REG_OFFSET));
}

uint32_t cgra_perf_cnt_get_col_active(const cgra_t *cgra, uint8_t column_idx) {
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_COL_0_ACTIVE_CYCLES_REG_OFFSET+column_idx*0x8));
}

uint32_t cgra_perf_cnt_get_col_stall(const cgra_t *cgra, uint8_t column_idx) {
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_COL_0_STALL_CYCLES_REG_OFFSET+column_idx*0x8));
}

uint32_t cgra_get_status(const cgra_t *cgra) {
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_COL_STATUS_REG_OFFSET));
}
