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
#include "../function.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

#define CGRA_COLS       ${cols_n}
#define IN_VAR_DEPTH    ${in_vars_depth}
#define OUT_VAR_DEPTH   ${out_vars_depth}

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

const uint32_t  cgra_imem_${shortname}[CGRA_IMEM_SIZE] = { ${imem} };
static uint32_t cgra_kmem_${shortname}[CGRA_KMEM_SIZE] = { ${kmem} };

static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

${in_vars}
${out_vars}

/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED VARIABLES                             */
/**                                                                        **/
/****************************************************************************/

extern kcom_kernel_t ${shortname}_kernel = {
    .kmem   = cgra_kmem_${shortname},
    .imem   = cgra_imem_${shortname},
    .col_n  = CGRA_COLS,
    .in_n   = IN_VAR_DEPTH,
    .out_n  = OUT_VAR_DEPTH,
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
    ${out_var_soft} = ${function}( ${in_args} );
}

uint32_t check(void) 
{
    uint32_t errors = 0;
    
${check_load}

#if PRINT_CGRA_RESULTS
    PRINTF("------------------------------\n");
    for( uint8_t c = 0; c < CGRA_COLS; c ++)
    {
        for( uint8_t r = 0; r < OUT_VAR_DEPTH; r++ )
        {
            PRINTF("[%d][%d]:%08x\t\t",c,r,cgra_output[c][r]);
        }
        PRINTF("\n");
    }
#endif //PRINT_CGRA_RESULTS


#if PRINT_RESULTS
        PRINTF("\nCGRA\t\tSoft\n");
#endif

    for( int i = 0; i < ${out_vars_n}; i++ )
    {
#if PRINT_RESULTS
        PRINTF("%08x\t%08x\t%s\n",
        ${cgra_res_elem},
        ${soft_res_elem},
        (${cgra_res_elem} != ${soft_res_elem}) ? "Wrong!" : ""
        );
#endif

        if (${cgra_res_elem} != ${soft_res_elem}) {
            errors++;
        }
    }
    return errors;
}

/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/