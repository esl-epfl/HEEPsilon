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
#include "defines.h"
#include "fft_data.h"

#ifdef CPLX_FFT
  #if FFT_SIZE==512
    #include "fft_factors_512_32b_int.h"
  #endif
  #if FFT_SIZE==1024
    #include "fft_factors_1024_32b_int.h"
  #endif
  #if FFT_SIZE==2048
    #include "fft_factors_2048_32b_int.h"
  #endif
#endif // CPLX_FFT
#ifdef REAL_FFT
  #if FFT_SIZE==512
    #include "fft_factors_256_32b_int.h"
  #endif
  #if FFT_SIZE==1024
    #include "fft_factors_512_32b_int.h"
  #endif
  #if FFT_SIZE==2048
    #include "fft_factors_1024_32b_int.h"
  #endif
#endif // REAL_FFT

#define DEBUG

// Use PRINTF instead of PRINTF to remove print by default
#ifdef DEBUG
  #define PRINTF(fmt, ...)    printf(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif

/* --------------------------------------------------------------------------
 *                     Functions declaration
 * --------------------------------------------------------------------------*/
uint16_t ReverseBits ( uint16_t index, uint16_t numBits );
uint16_t NumberOfBitsNeeded ( uint16_t powerOfTwo );

/* --------------------------------------------------------------------------
 *                     Global variables
 * --------------------------------------------------------------------------*/

// FFT radix-2 variables
fxp RealOut_fxp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };
fxp ImagOut_fxp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };

fxp RealOut_fxp_exp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };
fxp ImagOut_fxp_exp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };

#ifdef REAL_FFT
  fxp re_tmp[FFT_SIZE/2+1] __attribute__ ((aligned (4))) = { 0 };
  fxp im_tmp[FFT_SIZE/2+1] __attribute__ ((aligned (4))) = { 0 };
#endif // REAL_FFT

// one dim per core x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_COLS][CGRA_N_SLOTS][10] __attribute__ ((aligned (4))) = { 0 };
int8_t cgra_intr_flag;
// Nothing should be write here by the FFT kernel
int32_t cgra_output[CGRA_N_COLS][CGRA_N_ROWS][10] __attribute__ ((aligned (4))) = { 0 };

/*----------------------------------------------------------------------------
                        INTERRUPTS
-----------------------------------------------------------------------------*/

// Interrupt controller variables
void handler_irq_ext(uint32_t id) {
  if( id == CGRA_INTR) {
    cgra_intr_flag = 1;
  }
}

/* --------------------------------------------------------------------------
 *                     main
 * --------------------------------------------------------------------------*/
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

  //////////////////////////////////////////////////////////
  //
  // COMPLEX FFT radix-2 (Butterfy) implementation
  //
  //////////////////////////////////////////////////////////
#ifdef CPLX_FFT

  printf("Run a complex FFT of %d points on CGRA...\n", FFT_SIZE);
  
  cgra_perf_cnt_enable(&cgra, 1);
  uint16_t numBits = NumberOfBitsNeeded ( FFT_SIZE );
  int8_t column_idx;

  // STEP 1: bit reverse
  // Select request slot of CGRA (2 slots)
  uint32_t cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[0]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE/2; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = 0; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[FFT_SIZE/2+1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[FFT_SIZE/2]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = FFT_SIZE/2; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  // THIS MIGHT BE WRONG BECAUSE FIRST COLUMN TO FINISH MAKE THIS LOOP EXIT BUT THE SECOND ONE COULD STILL BE RUNNING
  // // SECOND WHILE THAT SHOULD ALSO BE EXITED, FIND A NICER SOLUTION ==> NOT WORKING
  // cgra_intr_flag=0;
  // while(cgra_intr_flag==0) {
  //   wait_for_interrupt();
  // }

  printf("Run a complex FFT of %d points on CGRA...\n", FFT_SIZE);

  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_output[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&RealOut_fxp[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&f_real[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE;

  column_idx = 1;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_output[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&f_imag[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&ImagOut_fxp[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)numBits;

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_ID);

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }

  // Check the cgra values are correct

  // Bitrev check
  // int32_t errors=0;
  // for (int i=0; i<FFT_SIZE; i++) {
  //   uint32_t revBit = ReverseBits(i, numBits);
  //   if(RealOut_fxp[i] != input_signal[2*revBit] ||
  //       ImagOut_fxp[i] != input_signal[2*revBit+1]) {
  //         printf("Real[%d->%d] (out/expected) %08x != %08x)\n", i, revBit, RealOut_fxp[i], input_signal[2*revBit]);
  //         printf("Imag[%d->%d] (out/expected) %08x != %08x)\n", i, revBit, ImagOut_fxp[i], input_signal[2*revBit+1]);
  //       errors++;
  //     }
  // }

  int32_t errors=0;
  for (int i=0; i<FFT_SIZE; i++) {
    if(RealOut_fxp[i] != exp_output_real[i] ||
        ImagOut_fxp[i] != exp_output_imag[i]) {
          printf("Real[%d] (out/expected) %08x != %08x)\n", i, RealOut_fxp[i], exp_output_real[i]);
          printf("Imag[%d] (out/expected) %08x != %08x)\n", i, ImagOut_fxp[i], exp_output_imag[i]);
        errors++;
      }
  }

  printf("CGRA FFT computation finished with %d errors\n", errors);

#endif // CPLX_FFT


  // printf("Run an FFT of %d points on CGRA...\n", FFT_SIZE);
  // cgra_perf_cnt_enable(&cgra, 1);
  // int8_t column_idx;
  // // Set CGRA kernel pointers
  // column_idx = 0;
  // cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[0][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[0][cgra_slot], column_idx);
  // // Set CGRA kernel pointers column 1
  // column_idx = 1;
  // cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[1][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[1][cgra_slot], column_idx);
  // // Set CGRA kernel pointers column 2
  // column_idx = 2;
  // cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[2][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[2][cgra_slot], column_idx);
  // // Set CGRA kernel pointers column 3
  // column_idx = 3;
  // cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[3][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res[3][cgra_slot], column_idx);

  // // Launch CGRA kernel
  // cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

  // // Wait CGRA is done
  // cgra_intr_flag=0;
  // while(cgra_intr_flag==0) {
  //   wait_for_interrupt();
  // }

  // // Check the cgra values are correct
  // int32_t errors=0;
  // for (int k=0; k<CGRA_N_COLS; k++) {
  //   for (int i=0; i<CGRA_N_ROWS; i++) {
  //     for (int j=0; j<OUTPUT_LENGTH; j++) {
  //       if (cgra_res[k][i][j] != exp_rc_c0[i][j]) {
  //         printf("[%d][%d][%d]: %d != %d\n", k, i, j, cgra_res[k][i][j], exp_rc_c0[i][j]);
  //         printf("[%d][%d][%d]: %08x != %08x\n", k, i, j, cgra_res[k][i][j], exp_rc_c0[i][j]);
  //         errors++;
  //       }
  //     }
  //   }
  // }

  // printf("CGRA functionality check finished with %d errors\n", errors);

  // // Performance counter display
  // printf("CGRA kernel executed: %d\n", cgra_perf_cnt_get_kernel(&cgra));
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
  // printf("CGRA kernel executed (after counter reset): %d\n", cgra_perf_cnt_get_kernel(&cgra));
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

  return EXIT_SUCCESS;
}

uint16_t ReverseBits ( uint16_t index, uint16_t numBits )
{
  uint16_t i, rev;

  for ( i=rev=0; i < numBits; i++ )
  {
    rev = (rev << 1) | (index & 1);
    index >>= 1;
  }

  return rev;
}

uint16_t NumberOfBitsNeeded ( uint16_t powerOfTwo )
{
  uint16_t i;

  if ( powerOfTwo < 2 )
  {
   return 0; // should not happen
  }

  for ( i=0; ; i++ )
  {
    if ( powerOfTwo & (1 << i) )
      return i;
  }
}
