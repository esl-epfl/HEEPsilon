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
#define IN_VAR_DEPTH    6
#define OUT_VAR_DEPTH   196




/****************************************************************************/
/**                                                                        **/
/*                      PROTOTYPES OF LOCAL FUNCTIONS                       */
/**                                                                        **/
/****************************************************************************/

static void        config  (int input_ch, int output_ch);
static void         loading_buffer(void);
static void        software(void);
static uint32_t    check   (void);


/**********************************************cgra_input******************************/
/**                                                                        **/
/*                            GLOBAL VARIABLES                              */
/**                                                                        **/
/****************************************************************************/

const uint32_t  cgra_imem_conv[CGRA_IMEM_SIZE] = {0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x0, 0x6bf0000, 0x971d0000, 0x0, 0x6a090004, 0x6bf0000, 0x971d0000, 0x0, 0x6a090008, 0x0, 0x0, 0x0, 0x0, 0xc80000, 0xc80000, 
                                                  0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x0, 0x6bf0000, 0x971d0000, 0x0, 0x6a090004, 0x6bf0000, 0x971d0000, 0x0, 0x6a090008, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
                                                  0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x0, 0x6bf0000, 0x971d0000, 0x0, 0x6a090004, 0x6bf0000, 0x971d0000, 0x0, 0x6a090008, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
                                                  0xad0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8a150004, 0x0, 0x0, 0x8a0d0004, 0x0, 0x0, 0x40b00000, 0xb0000, 0x0, 0x8a0d0004, 0x0, 0x0, 0x40b00000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x6bf0000, 0x971d0000, 0x0, 0x6a090004, 0x6bf0000, 0x971d0000, 0x0, 0x6a090008, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x6bf0000, 0x971d0000, 0x0, 0x6a090004, 0x6bf0000, 0x971d0000, 0x0, 0x6a090008, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x0, 0x6bf0000, 0x971d0000, 0x0, 0x6a090004, 0x6bf0000, 0x971d0000, 0x3080000f, 0x6a090008, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa00d00c4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8a150001, 0x0, 0x0, 0x0, 0x8a150001, 0x80880009, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x6bf0000, 0x971d0000, 0x840d0000, 0x6a090004, 0x6bf0000, 0x971d0000, 0x840d0000, 0x6a090008, 0x840d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x6bf0000, 0x971d0000, 0x840d0000, 0x6a090004, 0x6bf0000, 0x971d0000, 0x840d0000, 0x6a090008, 0x840d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0xab0000, 0x6bf0000, 0x971d0000, 0x840d0000, 0x6a090004, 0x6bf0000, 0x971d0000, 0x840d0000, 0x6a090008, 0x840d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa00d000d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8080000d, 0x0, 0x8a150001, 0x0, 0xa00d000e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x500d0000, 0x840d0000, 0x830d0000, 0x0, 0x500d0000, 0x0, 0x500d0000, 0x840d0000, 0x830d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x500d0000, 0x840d0000, 0x0, 0x0, 0x500d0000, 0x0, 0x500d0000, 0x840d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x500d0000, 0x840d0000, 0x0, 0x0, 0x500d0000, 0x0, 0x500d0000, 0x840d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x20d0000, 0x830d0000, 0x0, 0x9080000a, 0x0, 0x0, 0x20d0000, 0x830d0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, };
static uint32_t cgra_kmem_conv[CGRA_KMEM_SIZE] = {  0x0, 0xf013, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,};
extern int32_t outputs[N_output][channel_output][row_output][col_output];
static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

static int32_t buffer[row_output][col_output] __attribute__ ((aligned (4)));
static int32_t output_from_CGRA[N_filter][row_output][col_output] __attribute__ ((aligned (4)));

static int input_channel;
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
    .name   = "convolution",
};

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

void config(int input_ch, int output_ch)
{
input_channel = input_ch;
output_channel = output_ch;
cgra_input[3][0] = &buffer[0][0];


    for(int16_t i = 0; i < 3; i++) {
        cgra_input[0][i] =   &(input[0][input_channel][i][0]);
        cgra_input[1][i] =   &(input[0][input_channel][i][1]);
        cgra_input[2][i] =   &(input[0][input_channel][i][2]);
        
}

    for(int16_t i = 3; i < 6; i++){
        cgra_input[0][i] = filter[output_channel][input_channel][i-3][0];
        cgra_input[1][i] = filter[output_channel][input_channel][i-3][1];
        cgra_input[2][i] = filter[output_channel][input_channel][i-3][2];
    }

}


void loading_buffer(){
if(input_channel == 0){
    for(int i = 0; i < row_output; i++){
        for(int j = 0; j < col_output; j++){
            output_from_CGRA[output_channel][i][j] = 0; 
        }
    }
    
}
    for(int i = 0; i < row_output; i++){
        for(int j = 0; j < col_output; j++){
            buffer[i][j] = cgra_output[3][i*col_output + j];
        }
    }
    
        for(int i = 0; i < row_output; i++){
            for(int j = 0; j < col_output; j++){
                output_from_CGRA[output_channel][i][j] += buffer[i][j];
                
                
        }
    }

}


void software(void)
{
    printf("Software begins: \n");
    conv2D();
}

uint32_t check(void)
{
    printf("Check begins: \n");
    uint32_t errors = 0;

for(int l = 0; l < N_filter; l ++){
    for(int i = 0; i < row_output; i++){
        for(int j = 0; j < col_output; j++){
            if(outputs[0][0][i][j] != output_from_CGRA[l][i][j]){
                errors++;
                printf("Error at %d %d\n", i, j);
                printf("Expected %d, got %d\n", outputs[0][0][i][j], output_from_CGRA[l][i][j]);
            }
        }
    }
    printf("For Filter %d there are no errors. \n", l);
}
    if(errors == 0){
        printf("No errors\n");
    }
    else{
        printf("Errors: %d\n", errors);
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