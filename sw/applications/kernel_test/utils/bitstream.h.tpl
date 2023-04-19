/*
                              *******************
******************************* H HEADER FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : cgra_bitstream.h                                                   **
** version  : 1                                                            **
** date     : ${date}                                                    **
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
* @file   cgra_bitstream.h
* @date   ${date}
* @brief  ${description}
*
*/

#ifndef _CGRA_BITSTREAM_H_
#define _CGRA_BITSTREAM_H_

#include <stdint.h>

#include "cgra.h"

// Kernel 0 => NULL
#define ${FILENAME}_KER_ID 1

uint32_t cgra_kmem_bitstream[CGRA_KMEM_SIZE] = {
${word_config}
};

uint32_t cgra_imem_bitstream[CGRA_IMEM_SIZE] = {
${machine_code}
};

#endif // _CGRA_BITSTREAM_H_
