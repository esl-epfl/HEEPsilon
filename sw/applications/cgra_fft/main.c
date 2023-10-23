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
fxp RealOut_fft0_fxp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };
fxp ImagOut_fft0_fxp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };

#ifdef CGRA_100_PERCENT
  fxp RealOut_fft1_fxp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };
  fxp ImagOut_fft1_fxp[FFT_SIZE] __attribute__ ((aligned (4))) = { 0 };
#endif

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
// int32_t cgra_output[CGRA_N_COLS][CGRA_N_ROWS][10] __attribute__ ((aligned (4))) = { 0 };

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

  cgra_perf_cnt_enable(&cgra, 1);
  uint16_t numBits = NumberOfBitsNeeded ( FFT_SIZE );
  int8_t column_idx;

  // STEP 1: bit reverse
  PRINTF("Run input bit reverse reordering on %d points on CGRA...\n", FFT_SIZE);
  // Select request slot of CGRA (2 slots)
  uint32_t cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[0]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE/2; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft0_fxp[0];
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
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = FFT_SIZE/2; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

#ifdef CGRA_100_PERCENT
  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[0]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE/2; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft1_fxp[0];
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
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = FFT_SIZE/2; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);
#endif // CGRA_100_PERCENT

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }

  // Step 2: complex-valued FFT computation
  PRINTF("Run a complex FFT of %d points on CGRA...\n", FFT_SIZE);

  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_output[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&RealOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&f_real[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE;

  column_idx = 1;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_output[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&f_imag[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&ImagOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)numBits;

  // Launch CGRA kernel
  #ifdef CGRA_FFT_FOREVER
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_FOREVER_ID);
  #else
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_ID);
  #endif

#ifdef CGRA_100_PERCENT
  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_output[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&RealOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&f_real[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE;

  column_idx = 1;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);
  // cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_output[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&f_imag[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&ImagOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)numBits;

  // Launch CGRA kernel
  #ifdef CGRA_FFT_FOREVER
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_FOREVER_ID);
  #else
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_ID);
  #endif
#endif // CGRA_100_PERCENT

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
#endif // CPLX_FFT

#ifdef REAL_FFT
  printf("REAL FFT KERNEL DEPRECATED FOR CURRENT CGRA ARCHITECTURE")
#endif // REAL_FFT

#ifdef CHECK_ERRORS
  int32_t errors=0;
  for (int i=0; i<FFT_SIZE; i++) {
    if(RealOut_fft0_fxp[i] != exp_output_real[i] ||
        ImagOut_fft0_fxp[i] != exp_output_imag[i]) {
          printf("Real[%d] (out/expected) %08x != %08x)\n", i, RealOut_fft0_fxp[i], exp_output_real[i]);
          printf("Imag[%d] (out/expected) %08x != %08x)\n", i, ImagOut_fft0_fxp[i], exp_output_imag[i]);
        errors++;
      }
  }

#ifdef CGRA_100_PERCENT
  for (int i=0; i<FFT_SIZE; i++) {
    if(RealOut_fft1_fxp[i] != exp_output_real[i] ||
        ImagOut_fft1_fxp[i] != exp_output_imag[i]) {
          printf("Real[%d] (out/expected) %08x != %08x)\n", i, RealOut_fft1_fxp[i], exp_output_real[i]);
          printf("Imag[%d] (out/expected) %08x != %08x)\n", i, ImagOut_fft1_fxp[i], exp_output_imag[i]);
        errors++;
      }
  }
#endif

  printf("CGRA FFT computation finished with %d errors\n", errors);
#endif // CHECK_ERRORS

  return EXIT_SUCCESS;
}

uint16_t ReverseBits (uint16_t index, uint16_t numBits)
{
  uint16_t i, rev;

  for (i=rev=0; i<numBits; i++) {
    rev = (rev << 1) | (index & 1);
    index >>= 1;
  }

  return rev;
}

uint16_t NumberOfBitsNeeded (uint16_t powerOfTwo)
{
  uint16_t i;

  if (powerOfTwo < 2) {
   return 0; // should not happen
  }

  for (i=0;; i++) {
    if (powerOfTwo & (1 << i))
      return i;
  }
}
