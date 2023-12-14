/*
                              *******************
******************************* C SOURCE FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : conv.c                                                 **
** version  : 1                                                            **
** date     : 2023-06-22                                                       **
**                                                                         **
*****************************************************************************
**                                                                         **
** Copyright (c) EPFL                                                      **
** All rights reserved.                                                    **
**                                                                         **
*****************************************************************************
*/

/***************************************************************************/
/***************************************************************************/

/**
* @file   conv.c
* @date   2023-06-22
* @brief  A description of the kernel...
*
*/

#define _CONVOLUTION_C

/****************************************************************************/
/**                                                                        **/
/*                             MODULES USED                                 */
/**                                                                        **/
/****************************************************************************/
#include <stdint.h>

#include "conv.h"
#include "function.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/


#define CGRA_COLS       4
#define IN_VAR_DEPTH    19
#define OUT_VAR_DEPTH   14




/****************************************************************************/
/**                                                                        **/
/*                      PROTOTYPES OF LOCAL FUNCTIONS                       */
/**                                                                        **/
/****************************************************************************/

static void        config  (void);
static void        software(void);
static uint32_t    check   (void);

/****************************************************************************/
/**                                                                        **/
/*                            GLOBAL VARIABLES                              */
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/*                            Juan and Ruben Kernel                         */
/**                                                                        **/
/****************************************************************************/


/* conv 16x3 normal*/

const uint32_t  cgra_imem_conv[CGRA_IMEM_SIZE] = { 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x76180000, 0x7a080000, 0xab0000, 0x76180000, 0x7a080000, 0xab0000, 0x76180000, 0x7a080000, 0x0, 0x0, 0x0, 0xc80000, 0xc80000, 
                                                   0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x76180000, 0x7a080000, 0xab0000, 0x76180000, 0x7a080000, 0xab0000, 0x76180000, 0x7a080000, 0x0, 0x0, 0x0, 0x0, 0x0, 
                                                   0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x76180000, 0x7a080000, 0xab0000, 0x76180000, 0x7a080000, 0xab0000, 0x76180000, 0x7a080000, 0x0, 0x0, 0x0, 0x0, 0x0, 
                                                   0xa09000e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6a110001, 0x6080000a, 0x0, 0x6a110001, 0x6080000a, 0x0, 0x6a110001, 0x6088000b, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x76180000, 0x7a080000, 0x4a0b0000, 0x76180000, 0x7a080000, 0x4a0b0000, 0x76180000, 0x7a080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x76180000, 0x7a080000, 0x4a0b0000, 0x76180000, 0x7a080000, 0x4a0b0000, 0x76180000, 0x7a080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x76180000, 0x7a080000, 0x4a0b0000, 0x76180000, 0x7a080000, 0x4a0b0000, 0x76180000, 0x7a080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x0, 0x76180000, 0x14080000, 0x4a0b0000, 0x76180000, 0x14080000, 0x4a0b0000, 0x76180000, 0x14080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x0, 0x76180000, 0x14080000, 0x4a0b0000, 0x76180000, 0x14080000, 0x4a0b0000, 0x76180000, 0x14080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x0, 0x76180000, 0x14080000, 0x4a0b0000, 0x76180000, 0x14080000, 0x4a0b0000, 0x76180000, 0x14080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5080000, 0x14080000, 0x13080000, 0x5080000, 0x14080000, 0x13080000, 0x5080000, 0x14080000, 0x13080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5080000, 0x14080000, 0x0, 0x5080000, 0x14080000, 0x0, 0x5080000, 0x14080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5080000, 0x14080000, 0x0, 0x5080000, 0x14080000, 0x0, 0x5080000, 0x14080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2a080000, 0x13080000, 0x10b00000, 0x2a080000, 0x13080000, 0x10b00000, 0x2a080000, 0x13080000, 0x10b00000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  };
static uint32_t cgra_kmem_conv[CGRA_KMEM_SIZE] = {  0x0, 0xf012, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, };

static int32_t  __attribute__((section(".xheep_data_interleaved"))) cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
static int32_t  __attribute__((section(".xheep_data_interleaved"))) cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

static Pixel outSW[H_outputs];
static Pixel outCGRA[H_outputs];





    static uint32_t	i_index_soft;
    static uint32_t	i_index_cgra;
    static uint32_t	i_NumBits_soft;
    static uint32_t	i_NumBits_cgra;

    static uint32_t	o_ret_soft;
    static uint32_t	o_ret_cgra;


/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED VARIABLES                             */
/**                                                                        **/
/****************************************************************************/

extern kcom_kernel_t conv_kernel = {
    .kmem   = cgra_kmem_conv,
    .imem   = cgra_imem_conv,
    .col_n  = CGRA_N_COLS,
    .in_n   = IN_VAR_DEPTH,
    .out_n  = OUT_VAR_DEPTH,
    .input  = cgra_input,
    .output = cgra_output,
    .config = config,
    .func   = software,
    .check  = check,
    .name   = "convolution",
};

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

Pixel __attribute__((section(".xheep_data_interleaved"))) In_Img_interleaved[IMG_DIM + FILT_DIM];

void config()
{

    //copy data to the interleaved buffer
    for(int i = 0; i < IMG_DIM + FILT_DIM; i++)
        In_Img_interleaved[i] = In_Img[i];

#ifdef DEBUG_INPUT_DATA_CGRA
    for(int i = 0; i < IMG_DIM + FILT_DIM; i++) {
        printf("In_Img[%d] = %d (address %x)\n\r",i,In_Img_interleaved[i],&In_Img_interleaved[i]);
        for(int j=0;j<10000;j++) asm volatile("nop");
    }
#endif

    for(int16_t i = 0; i < (IN_VAR_DEPTH); i++) {
        cgra_input[0][i] = In_Img_interleaved[3*i];
        cgra_input[1][i] = In_Img_interleaved[3*i + 1];
        cgra_input[2][i] = In_Img_interleaved[3*i + 2];
        
    }

}

void software(void)
{
    conv( outSW );
}

uint32_t check(void)
{
 uint32_t errors = 0;
/*
    for(int16_t i = 0; i < OUT_VAR_DEPTH; i++) {
        outCGRA[i] = cgra_output[3][i];
        
    }
*/
#if PRINT_CGRA_RESULTS
        PRINTF("------------------------------\n");
    for( uint16_t i = 0; i < H_outputs; i++ ){
            PRINTF("[%d] %d \n", i, cgra_output[3][i]);
        

    }
        PRINTF("------------------------------\n");
        printf("Errors: %d\n", errors);
    o_ret_cgra = outCGRA[4];
#endif //PRINT_CGRA_RESULTS
    return errors;
}

/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/