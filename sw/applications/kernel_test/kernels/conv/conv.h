/*
                              *******************
******************************* H HEADER FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : conv.h                                                   **
** version  : 1                                                            **
** date     : 2023-08-28                                                    **
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
* @file   conv.h
* @date   2023-08-28
* @brief  A description of the kernel...
*
*/

#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

/****************************************************************************/
/**                                                                        **/
/**                            MODULES USED                                **/
/**                                                                        **/
/****************************************************************************/

#include "../../kernels_common/kernels_common.h"

// general parameters

#define stride 1
#define padding 0
#define N_filter 16
#define channel 16
#define batch_size 1

// input parameters

#define row_input 16
#define col_input 16
#define C_input channel

// filter parameters

#define row_filter 3
#define col_filter 3
#define C_filter channel
#define FILT_HALF_x (row_filter / 2)
#define FILT_HALF_y (col_filter / 2)

// output parameters
#define N_output batch_size
#define row_output (((row_input + 2 * padding - row_filter) / stride) + 1)
#define col_output (((col_input + 2 * padding - col_filter) / stride) + 1)
#define channel_output N_filter


/****************************************************************************/
/**                                                                        **/
/**                          EXPORTED VARIABLES                            **/
/**                                                                        **/
/****************************************************************************/

extern kcom_kernel_t conv_kernel;

#endif // _REVERSEBITS_H_
/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/