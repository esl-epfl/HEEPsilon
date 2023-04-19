/*
                              *******************
******************************* C SOURCE FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : ${filename}.c                                                 **
** version  : 1                                                            **
** date     : ${date}                                                       **
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
* @file   ${filename}.c
* @date   ${date}
* @brief  ${description}
*
*/

#define _${FILENAME}_C

/****************************************************************************/
/**                                                                        **/
/*                             MODULES USED                                 */
/**                                                                        **/
/****************************************************************************/
#include <stdint.h>

#include "${filename}.h"
#include "${aux_folder}function.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

#define CGRA_COLS       4
#define IN_VAR_DEPTH    ${in_var_depth}
#define OUT_VAR_DEPTH   1

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

static uint32_t cgra_imem_bitstream[CGRA_IMEM_SIZE] = { ${imem} };
static uint32_t cgra_kmem_bitstream[CGRA_KMEM_SIZE] = { ${kmem} };

static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));
static int32_t sw_res, cgra_res;

${in_vars}
${out_vars}

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED VARIABLES                             */
/**                                                                        **/
/****************************************************************************/

extern kcom_kernel_t ${shortname}_kernel = {
    .kmem   = cgra_kmem_bitstream,
    .imem   = cgra_imem_bitstream,
    .col_n  = CGRA_COLS,
    .input  = cgra_input,
    .output = cgra_output,
    .config = config,
    .func   = software,
    .check  = check,
    .name   = "${Filename}",
};

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

void config()
{
${config}
}

void software(void) 
{
    sw_res = ${function}( ${in_args} );
}

uint32_t check(void) 
{
    uint32_t errors = 0;
    cgra_res = cgra_output[${out_col}][0];   
    if (cgra_res != sw_res) {
        errors++;
    }
    return errors;
}

/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/