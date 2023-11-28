// This application works with any reasonable CGRA size.
// It does a simple check by using the maximum number of columns.
// Each RC loads a data, move it around and store a data back.
// It is not an extensive test but it checks basic features,
// so if it gives no error the system should work properly
// with this CGRA configuration.

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

// #define DEBUG

// Use PRINTF instead of printf to remove print by default
#ifdef DEBUG
  #define PRINTF(fmt, ...)    printf(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif

// Global variables
int8_t cgra_intr_flag;

int32_t stimuli[CGRA_N_ROWS][CGRA_MAX_COLS] = { 0 };
int32_t exp_res[CGRA_N_ROWS][CGRA_MAX_COLS] = { 0 };
int32_t cgra_res[CGRA_N_ROWS][CGRA_MAX_COLS] = { 0 };

uint32_t cgra_kmem_bitstream[CGRA_KMEM_DEPTH] = { 0 };
uint32_t cgra_cmem_bitstream[CGRA_CMEM_TOT_DEPTH] = { 0 };

// First add the number of instructions
// 1 LWD + 2 DATA MVT + 1 SDW + 1 EXIT
#define INSTR_PER_RC 5

// List of instructions used to generate the bitstream
uint32_t instr_list[INSTR_PER_RC] = {
  0x00a80000 + (4*CGRA_MAX_COLS), // lwd  self (address offset equals stimuli array 2nd dimension)
  0x40080000, // mv   self, rct
  0x20080000, // mv   self, rcl
  0x10b00000 + (4*CGRA_MAX_COLS), // swd  self (address offset equals stimuli array 2nd dimension)
  0x00000000  // nop
};
  
uint32_t exit_instr = 0x00c80000;


// Interrupt controller variables
void handler_irq_cgra(uint32_t id) {
    cgra_intr_flag = 1;
}

int main(void) {

  // Generate the bitstream on file to match the specific CGRA size
  uint32_t kmem_conf_word = 0;
  // The maximum number of columns are used and the kernel always starts at address 0
  // First create the one-hot encoding for the number of columns used
  uint32_t onehot_max_cols = 0;
  for (int i=0; i<CGRA_MAX_COLS; i++) {
    onehot_max_cols = (onehot_max_cols << 1) + 1;
  }
  // num_cols + cmem_bk_address (0) + num_instr (per RC)
  kmem_conf_word = ((uint32_t) (onehot_max_cols << (CGRA_CMEM_BK_DEPTH_LOG2+CGRA_RCS_NUM_CREG_LOG2))) | (uint32_t)(INSTR_PER_RC-1);

  cgra_kmem_bitstream[1] = kmem_conf_word;

  // printf("Kernel configuration word: 0x%x\n", kmem_conf_word);

  for (int i=0; i<CGRA_N_ROWS; i++) {
    for (int j=0; j<CGRA_MAX_COLS; j++) {
      for (int k=0; k<INSTR_PER_RC; k++) {
        cgra_cmem_bitstream[i*CGRA_CMEM_BK_DEPTH+j*INSTR_PER_RC+k] = instr_list[k]; 
      }
    }
  }
  // Add the exit instruction to RC(0,0)
  cgra_cmem_bitstream[INSTR_PER_RC-1] = exit_instr;

  PRINTF("Init CGRA context memory...");
  cgra_cmem_init(cgra_cmem_bitstream, cgra_kmem_bitstream);
  PRINTF("done\n");

  // Init the PLIC
  plic_Init();
  plic_irq_set_priority(CGRA_INTR, 1);
  plic_irq_set_enabled(CGRA_INTR, kPlicToggleEnabled);
  plic_assign_external_irq_handler(CGRA_INTR, &handler_irq_cgra);

  // Enable interrupt on processor side
  // Enable global interrupt for machine-level interrupts
  CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // Set mie.MEIE bit to one to enable machine-level external interrupts
  const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  CSR_SET_BITS(CSR_REG_MIE, mask);
  cgra_intr_flag = 0;

  cgra_t cgra;
  cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  // Generate the input data for the specific CGRA size
  // Example for a 2x5 CGRA:
  // 
  // RC(0,0) -- RC(0,1) --  RC(0,2) --  RC(0,3) --  RC(0,4)
  //    |          |           |           |           |
  // RC(1,0) -- RC(1,1) --  RC(1,2) --  RC(1,3) --  RC(1,4)
  //
  // Each RC reads the corresponding data:
  //
  // 0 -- 1 -- 2 -- 3 -- 4
  // |    |    |    |    | 
  // 5 -- 6 -- 7 -- 8 -- 9
  //
  int32_t* ptr_stimuli = stimuli;
  for (int i=0; i<CGRA_MAX_COLS*CGRA_N_ROWS; i++) {
    *ptr_stimuli++ = i;
  }

  // Now the expected results is generated
  // After reading the data, each RC reads the data of their top RC, then from their left RC
  for (int i=1; i<CGRA_N_ROWS; i++) {
    for (int j=1; j<CGRA_MAX_COLS; j++) {
      exp_res[i][j] = stimuli[i-1][j-1];
    }
  }
  // Handle the edges
  for (int j=1; j<CGRA_MAX_COLS; j++) {
    exp_res[0][j] = stimuli[CGRA_N_ROWS-1][j-1];
  }
  for (int i=1; i<CGRA_N_ROWS; i++) {
    exp_res[i][0] = stimuli[i-1][CGRA_MAX_COLS-1];
  }
  exp_res[0][0] = stimuli[CGRA_N_ROWS-1][CGRA_MAX_COLS-1];

  PRINTF("Running configuration check on %dx%d CGRA...", CGRA_N_ROWS, CGRA_N_COLS);

  // Check the CGRA can accept a new request
  cgra_wait_ready(&cgra);
  // Enable performance counters
  cgra_perf_cnt_enable(&cgra, 1);

  // Set CGRA kernel pointers
  for (int col_idx=0; col_idx<CGRA_MAX_COLS; col_idx++) {
    cgra_set_read_ptr(&cgra, (uint32_t) &stimuli[0][col_idx], col_idx);
    cgra_set_write_ptr(&cgra, (uint32_t) &cgra_res[0][col_idx], col_idx);
  }

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, 1);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  PRINTF("done\n");

  // Check the cgra values are correct
  int32_t errors=0;
  for (int i=0; i<CGRA_N_ROWS; i++) {
    for (int j=0; j<CGRA_MAX_COLS; j++) {
      if (cgra_res[i][j] != exp_res[i][j]) {
        PRINTF("[%d][%d]: %d != %d\n", i, j, cgra_res[i][j], exp_res[i][j]);
        PRINTF("[%d][%d]: %08x != %08x\n", i, j, cgra_res[i][j], exp_res[i][j]);
        errors++;
      }
    }
  }

  PRINTF("CGRA configuration check finished with %d errors\n", errors);

  return errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
