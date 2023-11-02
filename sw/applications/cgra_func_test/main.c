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
#include "cgra_bitstream.h"
#include "fxp.h"

#define DEBUG

// Use PRINTF instead of PRINTF to remove print by default
#ifdef DEBUG
  #define PRINTF(fmt, ...)    printf(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif

#define INPUT_LENGTH 4
#define OUTPUT_LENGTH 5

// one dim slot x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_COLS][CGRA_N_SLOTS][10] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;
int32_t cgra_res[CGRA_N_COLS][CGRA_N_ROWS][OUTPUT_LENGTH] = {0};

int32_t stimuli[CGRA_N_ROWS][INPUT_LENGTH] = {
  144, 4, 5, -23463,
  -12, 16, 5, 0,
  1033, 8, 5, 0,
  -199, 128, 5, 1,
};

int32_t exp_rc_c0[CGRA_N_ROWS][OUTPUT_LENGTH] = {0};

// Interrupt controller variables
void handler_irq_ext(uint32_t id) {
  if( id == CGRA_INTR) {
    cgra_intr_flag = 1;
  }
}

int main(void) {

  PRINTF("Init CGRA context memory...\n");
  cgra_cmem_init(cgra_imem_bitstream, cgra_kmem_bitstream);
  PRINTF("\rdone\n");

  // Init the PLIC
  plic_Init();
  plic_irq_set_priority(CGRA_INTR, 1);
  plic_irq_set_enabled(CGRA_INTR, kPlicToggleEnabled);

  // Enable interrupt on processor side
  // Enable global interrupt for machine-level interrupts
  CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // Set mie.MEIE bit to one to enable machine-level external interrupts
  const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  CSR_SET_BITS(CSR_REG_MIE, mask);
  cgra_intr_flag = 0;

  cgra_t cgra;
  cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  int32_t tmp0, tmp1, tmp2, tmp3;

  // SADD AND SSUB TEST
  tmp0 = stimuli[0][0]+stimuli[1][0];
  tmp1 = stimuli[1][0]+stimuli[2][0];
  tmp2 = stimuli[2][0]+stimuli[3][0];
  tmp3 = stimuli[3][0]+stimuli[0][0];
  exp_rc_c0[0][0] = tmp0-tmp3; // 187
  exp_rc_c0[1][0] = tmp1-tmp0; // 889
  exp_rc_c0[2][0] = tmp2-tmp1; // -187
  exp_rc_c0[3][0] = tmp3-tmp2; // -889
  // SMUL AND FXPMUL TEST
  tmp0 = stimuli[0][1]* 144;
  tmp1 = stimuli[1][1]*-12;
  tmp2 = stimuli[2][1]* 1033;
  tmp3 = stimuli[3][1]*-199;
  exp_rc_c0[0][1] = fxp_mult(tmp3, tmp0); // -448
  exp_rc_c0[1][1] = fxp_mult(tmp0, tmp1); // -4
  exp_rc_c0[2][1] = fxp_mult(tmp1, tmp2); // -49
  exp_rc_c0[3][1] = fxp_mult(tmp2, tmp3); // -6424
  // SLT, SRT, AND SRA TEST
  tmp0 = (uint32_t)exp_rc_c0[0][1]>>(uint32_t)stimuli[0][2]; // 134217714
  tmp1 = (uint32_t)exp_rc_c0[1][1]>>(uint32_t)stimuli[1][2]; // 0
  tmp2 = (uint32_t)exp_rc_c0[2][1]>>(uint32_t)stimuli[2][2]; // 131071
  tmp3 = (uint32_t)exp_rc_c0[3][1]>>(uint32_t)stimuli[3][2]; // 131071
  exp_rc_c0[0][2] = tmp0<<2; // 536870856
  exp_rc_c0[1][2] = tmp1<<2; // 0
  exp_rc_c0[2][2] = tmp2<<2; // 524284
  exp_rc_c0[3][2] = tmp3<<2; // 524284
  exp_rc_c0[0][3] = -16/8;   // -2
  exp_rc_c0[1][3] = 32/4;    // 8
  exp_rc_c0[2][3] = 1854/1;  // 1854
  exp_rc_c0[3][3] = -4096/2; // -2048
  // BSFA AND BZFA TEST
  tmp0 = stimuli[0][3] <0 ? 0 : -1; // 0
  tmp1 = stimuli[1][3]==0 ? 0 : -1; // 0
  tmp2 = stimuli[2][3] <0 ? 0 : -1; // -1
  tmp3 = stimuli[3][3]==0 ? 0 : -1; // -1
  exp_rc_c0[0][4] = tmp1==0 ? exp_rc_c0[0][2] : 1; // 536870856
  exp_rc_c0[1][4] = tmp2 <0 ? exp_rc_c0[1][2] : 1; // 0
  exp_rc_c0[2][4] = tmp3==0 ? exp_rc_c0[2][2] : 1; // 1
  exp_rc_c0[3][4] = tmp0 <0 ? exp_rc_c0[3][2] : 1; // 1

  // Select request slot of CGRA (2 slots)
  uint32_t cgra_slot = cgra_get_slot(&cgra);
  // input data ptr column 0
  cgra_input[0][cgra_slot][0] = (int32_t)&stimuli[0][0];
  cgra_input[0][cgra_slot][1] = (int32_t)&stimuli[1][0];
  cgra_input[0][cgra_slot][2] = (int32_t)&stimuli[2][0];
  cgra_input[0][cgra_slot][3] = (int32_t)&stimuli[3][0];
  cgra_input[0][cgra_slot][4] = (int32_t)&cgra_res[0][0][0];
  cgra_input[0][cgra_slot][5] = (int32_t)&cgra_res[0][1][0];
  cgra_input[0][cgra_slot][6] = (int32_t)&cgra_res[0][2][0];
  cgra_input[0][cgra_slot][7] = (int32_t)&cgra_res[0][3][0];

  // input data ptr column 1
  cgra_input[1][cgra_slot][0] = (int32_t)&stimuli[0][0];
  cgra_input[1][cgra_slot][1] = (int32_t)&stimuli[1][0];
  cgra_input[1][cgra_slot][2] = (int32_t)&stimuli[2][0];
  cgra_input[1][cgra_slot][3] = (int32_t)&stimuli[3][0];
  cgra_input[1][cgra_slot][4] = (int32_t)&cgra_res[1][0][0];
  cgra_input[1][cgra_slot][5] = (int32_t)&cgra_res[1][1][0];
  cgra_input[1][cgra_slot][6] = (int32_t)&cgra_res[1][2][0];
  cgra_input[1][cgra_slot][7] = (int32_t)&cgra_res[1][3][0];

  // input data ptr column 2
  cgra_input[2][cgra_slot][0] = (int32_t)&stimuli[0][0];
  cgra_input[2][cgra_slot][1] = (int32_t)&stimuli[1][0];
  cgra_input[2][cgra_slot][2] = (int32_t)&stimuli[2][0];
  cgra_input[2][cgra_slot][3] = (int32_t)&stimuli[3][0];
  cgra_input[2][cgra_slot][4] = (int32_t)&cgra_res[2][0][0];
  cgra_input[2][cgra_slot][5] = (int32_t)&cgra_res[2][1][0];
  cgra_input[2][cgra_slot][6] = (int32_t)&cgra_res[2][2][0];
  cgra_input[2][cgra_slot][7] = (int32_t)&cgra_res[2][3][0];

  // input data ptr column 3
  cgra_input[3][cgra_slot][0] = (int32_t)&stimuli[0][0];
  cgra_input[3][cgra_slot][1] = (int32_t)&stimuli[1][0];
  cgra_input[3][cgra_slot][2] = (int32_t)&stimuli[2][0];
  cgra_input[3][cgra_slot][3] = (int32_t)&stimuli[3][0];
  cgra_input[3][cgra_slot][4] = (int32_t)&cgra_res[3][0][0];
  cgra_input[3][cgra_slot][5] = (int32_t)&cgra_res[3][1][0];
  cgra_input[3][cgra_slot][6] = (int32_t)&cgra_res[3][2][0];
  cgra_input[3][cgra_slot][7] = (int32_t)&cgra_res[3][3][0];

  printf("Run functionality check on CGRA...\n");
  cgra_perf_cnt_enable(&cgra, 1);
  int8_t column_idx;
  // Set CGRA kernel pointers
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[0][cgra_slot], column_idx);
  cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[0][cgra_slot], column_idx);
  // Set CGRA kernel pointers column 1
  column_idx = 1;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[1][cgra_slot], column_idx);
  cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[1][cgra_slot], column_idx);
  // Set CGRA kernel pointers column 2
  column_idx = 2;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[2][cgra_slot], column_idx);
  cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[2][cgra_slot], column_idx);
  // Set CGRA kernel pointers column 3
  column_idx = 3;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[3][cgra_slot], column_idx);
  cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[3][cgra_slot], column_idx);

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FUNC_TEST);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }

  // Check the cgra values are correct
  int32_t errors=0;
  for (int k=0; k<CGRA_N_COLS; k++) {
    for (int i=0; i<CGRA_N_ROWS; i++) {
      for (int j=0; j<OUTPUT_LENGTH; j++) {
        if (cgra_res[k][i][j] != exp_rc_c0[i][j]) {
          printf("[%d][%d][%d]: %d != %d\n", k, i, j, cgra_res[k][i][j], exp_rc_c0[i][j]);
          printf("[%d][%d][%d]: %08x != %08x\n", k, i, j, cgra_res[k][i][j], exp_rc_c0[i][j]);
          errors++;
        }
      }
    }
  }

  printf("CGRA functionality check finished with %d errors\n", errors);

  // Performance counter display
  printf("CGRA kernel executed: %d\n", cgra_perf_cnt_get_kernel(&cgra));
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
  cgra_perf_cnt_reset(&cgra);
  printf("CGRA kernel executed (after counter reset): %d\n", cgra_perf_cnt_get_kernel(&cgra));
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

  return EXIT_SUCCESS;
}
