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
#define CGRA_OUT_LEN 1

// one dim slot x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_COLS][CGRA_IN_LEN] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;

int32_t stimuli_1[1] = {  155   };
int32_t stimuli_2[1] = {  187   };

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

  // Prepare input data for the CGRA
  cgra_input[0][0] = (int32_t)&stimuli_1[0];
  cgra_input[1][0] = (int32_t)&stimuli_1[0];
  cgra_input[2][0] = (int32_t)&stimuli_1[0];
  cgra_input[3][0] = (int32_t)&stimuli_1[0];
  cgra_input[0][1] = (int32_t)&stimuli_2[0];
  cgra_input[1][1] = (int32_t)&stimuli_2[0];
  cgra_input[2][1] = (int32_t)&stimuli_2[0];
  cgra_input[3][1] = (int32_t)&stimuli_2[0];

  PRINTF("Running functionality check on CGRA...");
  // Check the CGRA can accept a new request
  cgra_wait_ready(&cgra);
  // Enable performance counters
  cgra_perf_cnt_enable(&cgra, 1);
  int8_t column_idx;
  // Set CGRA kernel pointers
  column_idx = 0;
  cgra_set_read_ptr(&cgra, (uint32_t) cgra_input[0], column_idx);
  // Set CGRA kernel pointers column 1
  column_idx = 1;
  cgra_set_read_ptr(&cgra, (uint32_t) cgra_input[1], column_idx);
  // Set CGRA kernel pointers column 2
  column_idx = 2;
  cgra_set_read_ptr(&cgra, (uint32_t) cgra_input[2], column_idx);
  // Set CGRA kernel pointers column 3
  column_idx = 3;
  cgra_set_read_ptr(&cgra, (uint32_t) cgra_input[3], column_idx);

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, CGRA_FUNC_TEST);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  PRINTF("done\n");

  // Check the cgra values are correct
  int32_t errors=0;
  if (stimuli_1[0] != 5) {
    printf("stimuli_1[0] = %d\n", stimuli_1[0]);
    errors++;
  }
  if (stimuli_2[0] != 188) {
    printf("stimuli_2[0] = %d\n", stimuli_2[0]);
    errors++;
  }

  printf("CGRA functionality check finished with %d errors\n", errors);

  return errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
