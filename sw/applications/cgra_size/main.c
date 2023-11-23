#include <stdio.h>
#include <stdlib.h>

#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "cgra_x_heep.h"
#include "cgra.h"

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

#define CGRA_IN_LEN  8
#define CGRA_OUT_LEN 5

// one dim slot x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_COLS][CGRA_IN_LEN] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;
int32_t cgra_res[CGRA_N_COLS][CGRA_N_ROWS][CGRA_OUT_LEN] = {0};

int32_t stimuli[CGRA_N_ROWS][CGRA_MAX_COLS] = { 0 };
int32_t exp_res[CGRA_N_ROWS][CGRA_MAX_COLS] = { 0 };

uint32_t cgra_kmem_bitstream[CGRA_KMEM_DEPTH] = { 0 };
uint32_t cgra_cmem_bitstream[CGRA_CMEM_TOT_DEPTH] = { 0 };

// Interrupt controller variables
void handler_irq_ext(uint32_t id) {
  if (id == CGRA_INTR) {
    cgra_intr_flag = 1;
  }
}

int main(void) {

  // Generate the bitstream on file to match the specific CGRA size
  // num_cols + cmem_bk_address + num_instr (per RC)
  uint32_t kmem_conf_word = 0;
  // First add the number of instructions
  // 1 LWD + 2 DATA MVT + 1 SDW + 1 EXIT
  uint32_t num_instr = 5;
  // The maximum number of columns are used and the kernel always starts at address 0
  // First create the one-hot encoding for the number of columns used
  uint32_t onehot_max_cols = 0
  for (int i=0; i<CGRA_MAX_COLS*CGRA_N_ROWS; i++) {
    onehot_max_cols = onehot_max_cols << 1 + 1;
  }
  kmem_conf_word = num_instr + (uint32_t) onehot_max_cols << (CGRA_CMEM_BK_DEPTH_LOG2+CGRA_RCS_NUM_CREG_LOG2);

  printf("Kernel configuration word: 0x%x\n", kmem_conf_word);


  // PRINTF("Init CGRA context memory...");
  // cgra_cmem_init(cgra_cmem_bitstream, cgra_kmem_bitstream);
  // PRINTF("done\n");

  // // Init the PLIC
  // plic_Init();
  // plic_irq_set_priority(CGRA_INTR, 1);
  // plic_irq_set_enabled(CGRA_INTR, kPlicToggleEnabled);

  // // Enable interrupt on processor side
  // // Enable global interrupt for machine-level interrupts
  // CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // // Set mie.MEIE bit to one to enable machine-level external interrupts
  // const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  // CSR_SET_BITS(CSR_REG_MIE, mask);
  // cgra_intr_flag = 0;

  // cgra_t cgra;
  // cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  // // Generate the input data for the specific CGRA size
  // // Example for a 2x5 CGRA:
  // // 
  // // RC(0,0) -- RC(0,1) --  RC(0,2) --  RC(0,3) --  RC(0,4)
  // //    |          |           |           |           |
  // // RC(1,0) -- RC(1,1) --  RC(1,2) --  RC(1,3) --  RC(1,4)
  // //
  // // Each RC reads the corresponding data:
  // //
  // // 0 -- 1 -- 2 -- 3 -- 4
  // // |    |    |    |    | 
  // // 5 -- 6 -- 7 -- 8 -- 9
  // //
  // int32_t* ptr_stimuli = stimuli;
  // for (int i=0; i<CGRA_MAX_COLS*CGRA_N_ROWS; i++) {
  //   *ptr_stimuli++ = i;    
  // }

  // // Now the expected results is generated
  // // After reading the data, each RC reads the data of their top RC, then from their left RC
  // for (int i=1; i<CGRA_N_ROWS; i++) {
  //   for (int j=1; j<CGRA_MAX_COLS; j++) {
  //     exp_res[i][j] = stimuli[i-1][j-1];
  //   }
  // }
  // // Handle the edges
  // for (int j=1; j<CGRA_MAX_COLS; j++) {
  //   exp_res[0][j] = stimuli[CGRA_N_ROWS-1][j-1];
  // }
  // for (int i=1; i<CGRA_N_ROWS; i++) {
  //   exp_res[i][0] = stimuli[i-1][CGRA_MAX_COLS-1];
  // }
  // exp_res[0][0] = stimuli[CGRA_N_ROWS-1][CGRA_MAX_COLS-1];

  // PRINTF("Running check on CGRA...");

  // // Check the CGRA can accept a new request
  // cgra_wait_ready(&cgra);
  // // Enable performance counters
  // cgra_perf_cnt_enable(&cgra, 1);

  // // Set CGRA kernel pointers
  // for (int col_idx=0; j<CGRA_MAX_COLS; j++) {
  //   cgra_set_read_ptr(&cgra, (uint32_t) &stimuli[0][col_idx], col_idx);
  // }

  // // Launch CGRA kernel
  // cgra_set_kernel(&cgra, 1);

  // // Wait CGRA is done
  // cgra_intr_flag=0;
  // while(cgra_intr_flag==0) {
  //   wait_for_interrupt();
  // }
  // PRINTF("done\n");

  // // Check the cgra values are correct
  // int32_t errors=0;
  // for (int i=0; i<CGRA_N_ROWS; i++) {
  //   for (int j=0; j<CGRA_MAX_COLS; j++) {
  //     if (cgra_res[i][j] != exp_res[i][j]) {
  //       PRINTF("[%d][%d]: %d != %d\n", i, j, cgra_res[i][j], exp_res[i][j]);
  //       PRINTF("[%d][%d]: %08x != %08x\n", i, j, cgra_res[i][j], exp_res[i][j]);
  //       errors++;
  //     }
  //   }
  // }

  // printf("CGRA functionality check finished with %d errors\n", errors);

  // // Performance counter display
  // PRINTF("CGRA kernel executed: %d\n", cgra_perf_cnt_get_kernel(&cgra));
  // column_idx = 0;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // column_idx = 1;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // column_idx = 2;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // column_idx = 3;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // cgra_perf_cnt_reset(&cgra);
  // PRINTF("CGRA kernel executed (after counter reset): %d\n", cgra_perf_cnt_get_kernel(&cgra));
  // column_idx = 0;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // column_idx = 1;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // column_idx = 2;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  // column_idx = 3;
  // PRINTF("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  // PRINTF("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));

  // return errors ? EXIT_FAILURE : EXIT_SUCCESS;
  return 0;
}
