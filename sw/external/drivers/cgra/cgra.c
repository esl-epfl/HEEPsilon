// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stddef.h>
#include <stdint.h>

#include "cgra_x_heep.h"
#include "core_v_mini_mcu.h"
#include "cgra.h"
#include "cgra_regs.h" // generated

void cgra_cmem_init(uint32_t cgra_imem_bistream[], uint32_t cgra_kem_bitstream[])
{
  int32_t *cgra_cmem_ptr = (int32_t*) (CGRA_START_ADDRESS);

  printf("Loading Instruction memory in %08x\n", cgra_cmem_ptr);
  for (int i=0; i<CGRA_IMEM_SIZE; i++) {
    printf("value was: %u",*cgra_cmem_ptr);
    printf("I[%u]=%08x\n",i, cgra_imem_bistream[i]);
    *cgra_cmem_ptr++ = cgra_imem_bistream[i];
  }

  printf("Loading Kernel memory \n");
  for (int i=0; i<CGRA_KMEM_SIZE; i++) {
    printf("K[%u]=%08x\n",i, cgra_kem_bitstream[i]);
    *cgra_cmem_ptr++ = cgra_kem_bitstream[i];
  }
}

void cgra_set_read_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t read_ptr, uint8_t column) {
  if (slot_id == 0) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_PTR_IN_C0_REG_OFFSET+0x8*column), read_ptr);
  } else if (slot_id == 1) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT1_PTR_IN_C0_REG_OFFSET+0x8*column), read_ptr);
  }
}

void cgra_set_write_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t write_ptr, uint8_t column) {
  if (slot_id == 0) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_PTR_OUT_C0_REG_OFFSET+0x8*column), write_ptr);
  } else if (slot_id == 1) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT1_PTR_OUT_C0_REG_OFFSET+0x8*column), write_ptr);
  }
}

uint32_t cgra_get_slot(const cgra_t *cgra) {
  uint32_t slot0, slot1;

  // Wait until one slot is available
  do {
    slot0 = mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_KER_ID_REG_OFFSET));
    slot1 = mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_KER_ID_REG_OFFSET));
  } while (slot0 == 1 && slot1 == 1);
  // return the available slot
  return slot0 == 0 ? 0 : 1;
}

void cgra_set_kernel(const cgra_t *cgra, uint8_t slot_id, uint32_t kernel_id) {
  // Only slot 0 and 1 work for now
  if (slot_id == 0) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_KER_ID_REG_OFFSET), kernel_id);
  } else if (slot_id == 1) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT1_KER_ID_REG_OFFSET), kernel_id);
  }
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
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_C0_ACTIVE_CYCLES_REG_OFFSET+column_idx*0x8));
}

uint32_t cgra_perf_cnt_get_col_stall(const cgra_t *cgra, uint8_t column_idx) {
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_PERF_CNT_C0_STALL_CYCLES_REG_OFFSET+column_idx*0x8));
}

uint32_t cgra_get_status(const cgra_t *cgra) {
  return mmio_region_read32(cgra->base_addr, (ptrdiff_t)(CGRA_COL_STATUS_REG_OFFSET));
}
