/*
                              *******************
******************************* C SOURCE FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : sha2.c                                                 **
** version  : 1                                                            **
** date     : N_ITERS23-06-27                                                       **
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
* @file   sha2.c
* @date   N_ITERS23-06-27
* @brief  A description of the kernel...
*
*/

#define _SHA2_C

/****************************************************************************/
/**                                                                        **/
/*                             MODULES USED                                 */
/**                                                                        **/
/****************************************************************************/
#include <stdint.h>

#include "sha2.h"
#include "function.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

#if CGRA_N_COLS  == 2

    #define CGRA_COLS       2
    #define IN_VAR_DEPTH    7
    #define OUT_VAR_DEPTH   23

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

    const uint32_t  cgra_imem_sha2[CGRA_IMEM_SIZE] = {  0xa90000, 0xa0b0000, 0x0, 0x0, 0x0, 0x700b0000, 0x2a3d001b, 0x0, 0x2a280005, 0x18480000, 0x0, 0x7a180004, 0x61080000, 0x0, 0x1bf0000, 0x7a0b0001, 0x0, 0x49080000, 0x10b00000, 0x0, 0x0, 0xc80000, 0xa080000, 0x0, 0xa080000, 0x0, 0x0, 0x30080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10b00000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xab0000, 0xa90000, 0xad0000, 0x800f0000, 0x2a28001e, 0x10b00000, 0x600d0000, 0x70090000, 0xa80000, 0x51080000, 0x12080000, 0xa80000, 0x124b0000, 0x29080000, 0xa80000, 0x51880005, 0x80b00000, 0x70b00000, 0x60b00000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x50090000, 0x0, 0x0, 0x1a501fff, 0x31430000, 0x0, 0x37400000, 0x6a380002, 0x0, 0x37080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  };
    static uint32_t cgra_kmem_sha2[CGRA_KMEM_SIZE] = {  0x0, 0x3015, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
    static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

    static uint32_t	i_W_soft[N_ITERS];
    static uint32_t	i_W_cgra[N_ITERS];

    static uint32_t	*o_ret_soft;
    static uint32_t	o_ret_cgra[5];


    /****************************************************************************/
    /**                                                                        **/
    /*                           EXPORTED VARIABLES                             */
    /**                                                                        **/
    /****************************************************************************/

    extern kcom_kernel_t sha2_kernel = {
        .kmem   = cgra_kmem_sha2,
        .imem   = cgra_imem_sha2,
        .col_n  = CGRA_COLS,
        .in_n   = IN_VAR_DEPTH,
        .out_n  = OUT_VAR_DEPTH,
        .input  = cgra_input,
        .output = cgra_output,
        .config = config,
        .func   = software,
        .check  = check,
        .name   = "Sha2",
    };

    /****************************************************************************/
    /**                                                                        **/
    /*                            LOCAL FUNCTIONS                               */
    /**                                                                        **/
    /****************************************************************************/

    void config()
    {

        for(int i = 0; i < N_ITERS; i++ )
        {
            i_W_soft[i] = kcom_getRand() % (UINT_MAX - 1 - 0 + 1) + 0;
            i_W_cgra[i] = i_W_soft[i];
        }
        cgra_input[0][0] = N_ITERS;
        cgra_input[0][2] = 1518500249;
        cgra_input[0][1] = i_W_cgra;

    }

    void software(void)
    {
        o_ret_soft = sha2( i_W_soft );
    }

    uint32_t check(void)
    {
        uint32_t errors = 0;

        o_ret_cgra[0] = cgra_output[0][0];
        o_ret_cgra[1] = cgra_output[0][1];
        o_ret_cgra[2] = cgra_output[0][2];
        o_ret_cgra[3] = cgra_output[0][3];
        o_ret_cgra[4] = cgra_output[0][4];


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

        for( int i = 0; i < 5; i++ )
        {
    #if PRINT_RESULTS
            PRINTF("%08x\t%08x\t%s\n",
            o_ret_cgra[i],
            o_ret_soft[i],
            (o_ret_cgra[i] != o_ret_soft[i]) ? "Wrong!" : ""
            );
    #endif

            if (o_ret_cgra[i] != o_ret_soft[i]) {
                errors++;
            }
        }
        return errors;
    }

#elif CGRA_N_COLS == 3

    #define CGRA_COLS       3
    #define IN_VAR_DEPTH    1
    #define OUT_VAR_DEPTH   3

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

    const uint32_t  cgra_imem_sha2[CGRA_IMEM_SIZE] = {  0xad0000, 0xa090000, 0x600b0000, 0x50090000, 0x43480000, 0x0, 0x63400000, 0x84080000, 0x47080000, 0x0, 0x60b00000, 0xc80000, 0x0, 0x0, 0x0, 0x4a30001b, 0x0, 0x5a501fff, 0x0, 0x2080000, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0xa080000, 0x0, 0x0, 0x0, 0x36080000, 0x4a080001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x0, 0x0, 0x20080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10b00000, 0x0, 0x0, 0x0, 0x50080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1a28001e, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x0, 0x0, 0x0, 0x32400000, 0x2a300002, 0x0, 0x0, 0x21480000, 0x10b00000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x0, 0x3a280005, 0x0, 0x0, 0x2a180004, 0x25080000, 0x5b80000, 0x51080000, 0x10b00000, 0x0, 0xa080000, 0x0, 0x20080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10b00000, 0x0, 0xab0000, 0x0, 0x0, 0x0, 0x0, 0x50080000, 0x54080000, 0x0, 0x0, 0x57880002, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  };
    static uint32_t cgra_kmem_sha2[CGRA_KMEM_SIZE] = {  0x0, 0x700b, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    };

    static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
    static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

    static uint32_t	i_W_soft[N_ITERS];
    static uint32_t	i_W_cgra[N_ITERS];

    static uint32_t	*o_ret_soft;
    static uint32_t	o_ret_cgra[5];


    /****************************************************************************/
    /**                                                                        **/
    /*                           EXPORTED VARIABLES                             */
    /**                                                                        **/
    /****************************************************************************/

    extern kcom_kernel_t sha2_kernel = {
        .kmem   = cgra_kmem_sha2,
        .imem   = cgra_imem_sha2,
        .col_n  = CGRA_COLS,
        .in_n   = IN_VAR_DEPTH,
        .out_n  = OUT_VAR_DEPTH,
        .input  = cgra_input,
        .output = cgra_output,
        .config = config,
        .func   = software,
        .check  = check,
        .name   = "Sha2",
    };

    /****************************************************************************/
    /**                                                                        **/
    /*                            LOCAL FUNCTIONS                               */
    /**                                                                        **/
    /****************************************************************************/

    void config()
    {
        for(int i = 0; i < N_ITERS; i++ )
        {
            i_W_soft[i] = kcom_getRand() % (UINT_MAX - 1 - 0 + 1) + 0;
            i_W_cgra[i] = i_W_soft[i];
        }
        cgra_input[0][0] = i_W_cgra;
        cgra_input[2][0] = 0x5a827999L;
        cgra_input[2][1] = N_ITERS;

    }

    void software(void)
    {
        o_ret_soft = sha2( i_W_soft );
    }

    uint32_t check(void)
    {
        uint32_t errors = 0;

        o_ret_cgra[4] = cgra_output[0][0]; // E
        o_ret_cgra[3] = cgra_output[0][1]; // D
        o_ret_cgra[0] = cgra_output[0][2]; // A

        o_ret_cgra[1] = cgra_output[1][0]; // B

        o_ret_cgra[2] = cgra_output[2][0]; // C

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

        for( int i = 0; i < 5; i++ )
        {
    #if PRINT_RESULTS
            PRINTF("%08d\t%08d\t%s\n",
                o_ret_cgra[i],
                o_ret_soft[i],
                (o_ret_cgra[i] != o_ret_soft[i]) ? "Wrong!" : ""
                );
    #endif

            if (o_ret_cgra[i] != o_ret_soft[i]) {
                errors++;
            }
        }
        return errors;
    }

#elif CGRA_N_COLS == 4

    #define CGRA_COLS       4
    #define IN_VAR_DEPTH    1
    #define OUT_VAR_DEPTH   3

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

    const uint32_t  cgra_imem_sha2[CGRA_IMEM_SIZE] = {  0x0,  0x3080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xc80000,  0xa080000,  0x4080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x10b00000,  0x0,  0x0,  0x0,  0x2a30001b,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa080000,  0x0,  0x0,  0x0,  0x3a310002,  0x3a28001e,  0x16480000,  0x0,  0x0,  0x10b00000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x24400000,  0x31080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa90000,  0x0,  0x4a280005,  0x0,  0x36080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x24480000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa080000,  0x0,  0x0,  0x4080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x10b00000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x25400000,  0x0,  0x41080000,  0x0,  0x0,  0x0,  0x0,  0xa080000,  0x0,  0x0,  0x5a080001,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa090014,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x26880001,  0x0,  0x0,  0xa080000,  0x0,  0x4080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x10b00000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xab0000,  0x0,  0x3a190004,  0x5a501fff,  0x76080000,  0x0,  0x0,  0x42080000,  0x0,  0x0,  0x0,  0xa080000,  0x4080000,  0x0,  0x0,  0x0,  0x2b90000,  0x0,  0x0,  0x26080000,  0x10b00000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x4080000,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 };
    static uint32_t cgra_kmem_sha2[CGRA_KMEM_SIZE] = {  0x0,  0xf00a,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 };

    static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
    static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

    static int32_t	i_W_soft[N_ITERS];
    static int32_t	i_W_cgra[N_ITERS];

    static int32_t	*o_ret_soft;
    static int32_t	o_ret_cgra[5];


    /****************************************************************************/
    /**                                                                        **/
    /*                           EXPORTED VARIABLES                             */
    /**                                                                        **/
    /****************************************************************************/

    extern kcom_kernel_t sha2_kernel = {
        .kmem   = cgra_kmem_sha2,
        .imem   = cgra_imem_sha2,
        .col_n  = CGRA_N_COLS,
        .in_n   = IN_VAR_DEPTH,
        .out_n  = OUT_VAR_DEPTH,
        .input  = cgra_input,
        .output = cgra_output,
        .config = config,
        .func   = software,
        .check  = check,
        .name   = "Sha2",
    };

    /****************************************************************************/
    /**                                                                        **/
    /*                            LOCAL FUNCTIONS                               */
    /**                                                                        **/
    /****************************************************************************/

    void config()
    {
        for(int i = 0; i < N_ITERS; i++ )
        {
            i_W_soft[i] = kcom_getRand() % (UINT_MAX - 1 - 0 + 1) + 0;
            i_W_cgra[i] = i_W_soft[i];
        }
        cgra_input[0][0] = i_W_cgra;
        cgra_input[1][0] = 1518500249;

    }

    void software(void)
    {
        o_ret_soft = sha2( i_W_soft );
    }

    uint32_t check(void)
    {
        uint32_t errors = 0;

        o_ret_cgra[1] = cgra_output[1][0];
        o_ret_cgra[0] = cgra_output[1][1];
        o_ret_cgra[2] = cgra_output[3][0];
        o_ret_cgra[3] = cgra_output[3][1];
        o_ret_cgra[4] = cgra_output[3][2];


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

        for( int i = 0; i < 5; i++ )
        {
    #if PRINT_RESULTS
            PRINTF("%08x\t%08x\t%s\n",
            o_ret_cgra[i],
            o_ret_soft[i],
            (o_ret_cgra[i] != o_ret_soft[i]) ? "Wrong!" : ""
            );
    #endif

            if (o_ret_cgra[i] != o_ret_soft[i]) {
                errors++;
            }
        }
        return errors;
    }

#endif

/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/