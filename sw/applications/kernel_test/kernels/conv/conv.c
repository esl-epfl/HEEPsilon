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
#define IN_VAR_DEPTH    32
#define OUT_VAR_DEPTH   1




/****************************************************************************/
/**                                                                        **/
/*                      PROTOTYPES OF LOCAL FUNCTIONS                       */
/**                                                                        **/
/****************************************************************************/

static void        config  (void);
static void         loading_buffer(int output_channel, int out_row, int out_col);
static void        software(void);
static uint32_t    check   (void);
static void         im2col_conv(int32_t *input_to_CGRA, int32_t *filter_to_CGRA, int out_row, int out_col, int output_channel);
static void         im2col_func(int output_channel, int out_row, int out_col);


/**********************************************cgra_input******************************/
/**                                                                        **/
/*                            GLOBAL VARIABLES                              */
/**                                                                        **/
/****************************************************************************/

static uint32_t  __attribute__((section(".xheep_data_interleaved"))) cgra_imem_conv[CGRA_IMEM_SIZE] = {0xa90000, 0xad0000, 0xa080009, 0x1ac00004, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0xab80004, 0x1a100001, 0x10880004, 0x90080000, 0x15080000, 0x0, 0x0, 0x0, 0x0, 0xc80000, 0xc80000, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x15080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x15080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x15080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x4ac00004, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x14080000, 0x15080000, 0x13080000, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x14080000, 0x15080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x14080000, 0x15080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xad0000, 0x0, 0x0, 0x6bb0000, 0x8b80000, 0x17180000, 0x190f0000, 0x6a090040, 0x8a0d0040, 0x0, 0x0, 0x0, 0x90080000, 0x14080000, 0x15080000, 0x12080000, 0x13080000, 0x10b00000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, };
static uint32_t __attribute__((section(".xheep_data_interleaved"))) cgra_kmem_conv[CGRA_KMEM_SIZE] = {  0x0, 0xf014, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, };
extern int32_t __attribute__((section(".xheep_data_interleaved"))) output[N_output][channel_output][row_output][col_output];
static int32_t __attribute__((section(".xheep_data_interleaved"))) cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
static int32_t __attribute__((section(".xheep_data_interleaved"))) cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));
static int32_t __attribute__((section(".xheep_data_interleaved"))) input_to_CGRA[row_filter * col_filter * C_filter];
static int32_t __attribute__((section(".xheep_data_interleaved"))) filter_to_CGRA[row_filter * col_filter * C_filter];
static int32_t __attribute__((section(".xheep_data_interleaved"))) output_from_CGRA[N_filter][row_output][col_output] __attribute__ ((aligned (4)));

static int out_row;
static int out_col;
static int output_channel;


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
    .loading_buffer = loading_buffer,
    .im2col = im2col_func,
    .name   = "convolution",
};

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

void im2col_func(int output_channel, int out_row, int out_col)
{
    
out_row = out_row;
out_col = out_col;
output_channel = output_channel;
im2col_conv(input_to_CGRA, filter_to_CGRA, out_row, out_col, output_channel);
}

void config()
{
printf("config\n");


  

for(int i = 0; i < 4; i++){
    cgra_input[0][i] = &(input_to_CGRA[4*i]  );
    cgra_input[1][i] = &(input_to_CGRA[4*i+1] );
    cgra_input[2][i] = &(input_to_CGRA[4*i+2]);
    cgra_input[3][i] = &(input_to_CGRA[4*i+3]);
}

for(int i = 4; i < 8; i++){
    cgra_input[0][i] = &(filter_to_CGRA[(i%4)*4]);
    cgra_input[1][i] = &(filter_to_CGRA[(i%4)*4+1]);
    cgra_input[2][i] = &(filter_to_CGRA[(i%4)*4+2]);
    cgra_input[3][i] = &(filter_to_CGRA[(i%4)*4+3]);
}

}


void loading_buffer(int output_channel, int out_row, int out_col){
output_from_CGRA[output_channel][out_row][out_col]=cgra_output[3][0];

}


void software(void)
{
    conv2D();
}

uint32_t check(void)
{

    uint32_t errors = 0;

for(int l = 0; l < N_filter; l ++){
    for(int i = 0; i < row_output; i++){
        for(int j = 0; j < col_output; j++){
            if(output[0][l][i][j] != output_from_CGRA[l][i][j]){
                errors++;
                //printf("Error at %d %d\n", i, j);
                //printf("Expected %d, got %d\n", output[0][l][i][j], output_from_CGRA[l][i][j]);
            }
        }
    }
    


}


    //for(int16_t i = 0; i < IN_VAR_DEPTH; i++) {
    //    outCGRA[4 + i*3] = cgra_output[3][i];
    //}
//
    //for( uint16_t i = 0; i < IMG_DIM; i++ ){
    //        PRINTF("[%d] %d != %d\n", i, outSW[i],outCGRA[i] );
    //    // if( outImg[i] != Gold_Out_Img[i] ){
    //    //     errors++;
    //    // }
//
//
    //}
//
    //o_ret_cgra = outCGRA[4];

    return errors;
}

/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/