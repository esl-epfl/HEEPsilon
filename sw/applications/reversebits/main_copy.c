#include <stdio.h>
#include <stdlib.h>

#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "heepocrates.h"
#include "cgra.h"
#include "cgra_bitstream.h"

// Use PRINTF instead of PRINTF to remove print by default
#ifdef DEBUG
  #define PRINTF(fmt, ...)    PRINTF(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif

// Input parameters
uint32_t input = 0xab;
uint32_t num_bits = 8;

// cgra variables
int32_t cgra_input[2] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;
volatile int32_t cgra_res __attribute__ ((aligned (4))) = {0};
int32_t sw_res = {0};

// Interrupt controller variables
dif_plic_params_t rv_plic_params;
dif_plic_t rv_plic;
dif_plic_result_t plic_res;
dif_plic_irq_id_t intr_num;

/* 
  CGRA IRQ Handler
*/
void handler_irq_external(void) {
    // Claim/clear interrupt
    plic_res = dif_plic_irq_claim(&rv_plic, 0, &intr_num);
    if (plic_res == kDifPlicOk && intr_num == CGRA_INTR) {
      cgra_intr_flag = 1;
    }
}

/* 
  SW reverse bits

  Revert a specified number of bits (starting from lsb) of the variable index
*/
unsigned ReverseBits ( unsigned index, unsigned NumBits )
{
    unsigned i, rev;

    for ( i=rev=0; i < NumBits; i++ )
    {
        rev = (rev << 1) | (index & 1);
        index >>= 1;
    }

    return rev;
}

int main(void) {

  PRINTF("Init CGRA context memory...\n");
  cgra_cmem_init(cgra_imem_bistream, cgra_kem_bitstream);
  PRINTF("\rdone\n");

  // Init the PLIC
  rv_plic_params.base_addr = mmio_region_from_addr((uintptr_t)PLIC_START_ADDRESS);
  plic_res = dif_plic_init(rv_plic_params, &rv_plic);

  if (plic_res != kDifPlicOk) {
    printf("PLIC init failed\n;");
    return EXIT_FAILURE;
  }

  // Set CGRA priority to 1 (target threshold is by default 0) to trigger an interrupt to the target (the processor)
  plic_res = dif_plic_irq_set_priority(&rv_plic, CGRA_INTR, 1);
  if (plic_res != kDifPlicOk) {
    printf("Set CGRA interrupt priority to 1 failed\n");
    return EXIT_FAILURE;
  }

  plic_res = dif_plic_irq_set_enabled(&rv_plic, CGRA_INTR, 0, kDifPlicToggleEnabled);
  if (plic_res != kDifPlicOk) {
    printf("Enable CGRA interrupt failed\n");
    return EXIT_FAILURE;
  }

  // Enable interrupt on processor side
  // Enable global interrupt for machine-level interrupts
  CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // Set mie.MEIE bit to one to enable machine-level external interrupts
  const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  CSR_SET_BITS(CSR_REG_MIE, mask);
  cgra_intr_flag = 0;

  cgra_t cgra;
  cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  // Variable for CGRA call anc check
  uint8_t cgra_slot;
  cgra_input[0] = input;
  cgra_input[1] = num_bits;
  printf("Addr: %08x\n", cgra_input);

  /*
    -- REVERSE BIT EXAMPLE --
  */                                                                                             

  /*
    SW
  */  
  PRINTF("Run Bit_Reverse on cpu...\n");
  sw_res = ReverseBits( input , num_bits );
  PRINTF("\rdone\n");

  /*
    CGRA
  */  

  // Select request slot of CGRA (2 slots)
  cgra_slot = cgra_get_slot(&cgra);

  cgra_perf_cnt_enable(&cgra, 1);

  // Set CGRA kernel L/S pointers
  for(int8_t column_idx = 0 ; column_idx < 4 ; column_idx++){
    cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t)cgra_input, column_idx);
    cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t)&cgra_res, column_idx);
  }

  // Launch CGRA kernel
  printf("Run Bit_Reverse operation on CGRA...\n");
  cgra_set_kernel(&cgra, cgra_slot, REVERSEBITS_KER_ID);

  // Wait CGRA is done
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  printf("Done!\n");
  // Complete the interrupt
  plic_res = dif_plic_irq_complete(&rv_plic, 0, &intr_num);
  if (plic_res != kDifPlicOk || intr_num != CGRA_INTR) {
    printf("CGRA interrupt complete failed\n");
    return EXIT_FAILURE;
  }

  printf("Results:\n");
  printf("[SRC] : %08x\n", input);
  printf("[CPU] : %08x\n", sw_res);
  printf("[CGRA]: %08x\n", cgra_res);

  // Check the cgra values are correct
  if (cgra_res != sw_res) {
    printf("CGRA MISMATCH!\n");
  }

  int8_t column_idx;
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

  return EXIT_SUCCESS;
}
