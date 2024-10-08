#include <stdio.h>
#include <stdlib.h>

#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "heepsilon.h"
#include "cgra.h"
#include "cgra_bitstream.h"
#include "stimuli.h"

// This application only works with a 4x4 CGRA
#if CGRA_N_COLS != 4 | CGRA_N_ROWS != 4
  #error The CGRA must have a 4x4 size to run this example
#endif

// #define DEBUG

// Use PRINTF instead of printf to remove print by default
#ifdef DEBUG
  #define PRINTF(fmt, ...)    printf(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif

#define CGRA_IN_LEN  2
#define CGRA_OUT_LEN 4

// one dim slot x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_IN_LEN] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;
volatile int32_t cgra_res[CGRA_OUT_LEN] = {0};
int32_t exp_res[CGRA_OUT_LEN] = {0};

// Interrupt controller variables
void handler_irq_cgra(uint32_t id) {
    cgra_intr_flag = 1;
}

int main(void) {

  PRINTF("Init CGRA context memory...");
  cgra_cmem_init(cgra_cmem_bitstream, cgra_kmem_bitstream);
  PRINTF("done\n");

  // Init the PLIC
  plic_Init();
  plic_irq_set_priority(CGRA_INTR, 1);
  plic_irq_set_enabled(CGRA_INTR, kPlicToggleEnabled);
  plic_assign_external_irq_handler( CGRA_INTR, (void *) &handler_irq_cgra);

  // Enable interrupt on processor side
  // Enable global interrupt for machine-level interrupts
  CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // Set mie.MEIE bit to one to enable machine-level external interrupts
  const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  CSR_SET_BITS(CSR_REG_MIE, mask);
  cgra_intr_flag = 0;

  cgra_t cgra;
  cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  // Variable for CGRA call and check
  int8_t column_idx;
  int32_t errors_min, errors_max;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  _____   ____  _    _ ____  _      ______   __  __ _____ _   _    _____ ______          _____   _____ _    _  //
  // |  __ \ / __ \| |  | |  _ \| |    |  ____| |  \/  |_   _| \ | |  / ____|  ____|   /\   |  __ \ / ____| |  | | //
  // | |  | | |  | | |  | | |_) | |    | |__    | \  / | | | |  \| | | (___ | |__     /  \  | |__) | |    | |__| | //
  // | |  | | |  | | |  | |  _ <| |    |  __|   | |\/| | | | | . ` |  \___ \|  __|   / /\ \ |  _  /| |    |  __  | //
  // | |__| | |__| | |__| | |_) | |____| |____  | |  | |_| |_| |\  |  ____) | |____ / ____ \| | \ \| |____| |  | | //
  // |_____/ \____/ \____/|____/|______|______| |_|  |_|_____|_| \_| |_____/|______/_/    \_\_|  \_\\_____|_|  |_| //
  //                                                                                                               //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  exp_res[0] = stimuli[0];
  exp_res[1] = INT32_MAX;
  exp_res[2] = 0;
  exp_res[3] = -1;

  PRINTF("Run double min search on cpu...");
  for(int32_t i=1; i<INPUT_LENGTH; i++) {
    if (stimuli[i] < exp_res[0]) {
      exp_res[1] = exp_res[0];
      exp_res[0] = stimuli[i] ;
      exp_res[3] = exp_res[2];
      exp_res[2] = i;
    } else if (stimuli[i] < exp_res[1]) {
      exp_res[1] = stimuli[i];
      exp_res[3] = i;
    }
  }
  PRINTF("done\n");

  // Check the CGRA can accept a new request
  cgra_wait_ready(&cgra);
  // input data ptr
  cgra_input[0] = (int32_t)&stimuli[0];
  // input size
  cgra_input[1] = INPUT_LENGTH-1;

  PRINTF("Run double min search on CGRA...");
  cgra_perf_cnt_enable(&cgra, 1);
  // Set CGRA kernel pointers
  column_idx = 0;
  cgra_set_read_ptr(&cgra, (uint32_t) cgra_input, column_idx);
  cgra_set_write_ptr(&cgra, (uint32_t) cgra_res, column_idx);
  // Launch CGRA kernel
  cgra_set_kernel(&cgra, DBL_MIN_KER_ID);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  PRINTF("done\n");

  // Check the cgra values are correct
  errors_min=0;
  for (int i=0; i<CGRA_OUT_LEN; i++) {
    if (cgra_res[i] != exp_res[i]) {
      PRINTF("[%d]: %d != %d\n", i, cgra_res[i], exp_res[i]);
      PRINTF("[%d]: %08x != %08x\n", i, cgra_res[i], exp_res[i]);
      errors_min++;
    }
  }

  printf("CGRA double minimum check finished with %d errors\n", errors_min);

  // Performance counter display
  PRINTF("CGRA kernel executed: %d\n", cgra_perf_cnt_get_kernel(&cgra));

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  _____   ____  _    _ ____  _      ______   __  __          __   __   _____ ______          _____   _____ _    _  //
  // |  __ \ / __ \| |  | |  _ \| |    |  ____| |  \/  |   /\    \ \ / /  / ____|  ____|   /\   |  __ \ / ____| |  | | //
  // | |  | | |  | | |  | | |_) | |    | |__    | \  / |  /  \    \ V /  | (___ | |__     /  \  | |__) | |    | |__| | //
  // | |  | | |  | | |  | |  _ <| |    |  __|   | |\/| | / /\ \    > <    \___ \|  __|   / /\ \ |  _  /| |    |  __  | //
  // | |__| | |__| | |__| | |_) | |____| |____  | |  | |/ ____ \  / . \   ____) | |____ / ____ \| | \ \| |____| |  | | //
  // |_____/ \____/ \____/|____/|______|______| |_|  |_/_/    \_\/_/ \_\ |_____/|______/_/    \_\_|  \_\\_____|_|  |_| //
  //                                                                                                                   //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  exp_res[0] = stimuli[0];
  exp_res[1] = INT32_MIN;
  exp_res[2] = 0;
  exp_res[3] = -1;

  PRINTF("Run double max search on cpu...");
  for(int32_t i=1; i<INPUT_LENGTH; i++) {
    if (stimuli[i] > exp_res[0]) {
      exp_res[1] = exp_res[0];
      exp_res[0] = stimuli[i] ;
      exp_res[3] = exp_res[2];
      exp_res[2] = i;
    } else if (stimuli[i] > exp_res[1]) {
      exp_res[1] = stimuli[i];
      exp_res[3] = i;
    }
  }
  PRINTF("done\n");

  // Check the CGRA can accept a new request
  cgra_wait_ready(&cgra);
  // input data ptr
  cgra_input[0] = (int32_t)&stimuli[0];
  // input size
  cgra_input[1] = INPUT_LENGTH-1;

  PRINTF("Run double max search on CGRA...");
  cgra_perf_cnt_enable(&cgra, 1);
  // Set CGRA kernel pointers
  column_idx = 0;
  cgra_set_read_ptr(&cgra, (uint32_t) cgra_input, column_idx);
  cgra_set_write_ptr(&cgra, (uint32_t) cgra_res, column_idx);
  // Launch CGRA kernel
  cgra_set_kernel(&cgra, DBL_MAX_KER_ID);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  PRINTF("done\n");

  // Check the cgra values are correct
  errors_max=0;
  for (int i=0; i<CGRA_OUT_LEN; i++) {
    if (cgra_res[i] != exp_res[i]) {
      PRINTF("[%d]: %d != %d\n", i, cgra_res[i], exp_res[i]);
      PRINTF("[%d]: %08x != %08x\n", i, cgra_res[i], exp_res[i]);
      errors_max++;
    }
  }

  printf("CGRA double maximum check finished with %d errors\n", errors_max);

  // Performance counter display
  PRINTF("CGRA kernel executed: %d\n", cgra_perf_cnt_get_kernel(&cgra));
  column_idx = 0;
  PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  column_idx = 1;
  PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  column_idx = 2;
  PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  column_idx = 3;
  PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));

  return (errors_min+errors_max) ? EXIT_FAILURE : EXIT_SUCCESS;
}
