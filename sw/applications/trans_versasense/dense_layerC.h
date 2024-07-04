//
// Created by alireza on 10/5/23.
//

#ifndef FVLLMONTITRANSFORMER_DENSE_LAYERC_H
#define FVLLMONTITRANSFORMER_DENSE_LAYERC_H


#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "param.h"
#include "cgra.h"

// Define the struct
typedef struct {
    size_t input_size_;
    size_t output_size_;
    int32_t* weight; // quant_bit_width is a typedef for int32_t
    int32_t* bias; // quant_bit_width is a typedef for int32_t
} Dense;

void createDense(Dense* dense, size_t input_dim, size_t output_dim, quant_bit_width *weight, quant_bit_width* bias);
void destroyDense(Dense* dense);
void multiplyweight(Dense* dense, size_t seq_len, int32_t* input, int32_t* output);
void addbias(Dense* dense, size_t seq_len, int32_t* output);
void computeDense(Dense* dense, size_t seq_len, int32_t* input, int32_t* output);
void computeDenseOneRow(Dense* dense, size_t seq_len, int32_t* input, int32_t* output);
void activation(Dense* dense, size_t length, int32_t* input, int32_t* output);

#endif //FVLLMONTITRANSFORMER_DENSE_LAYERC_H
