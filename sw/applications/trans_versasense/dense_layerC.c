//
// Created by alireza on 10/6/23.
//

#include "dense_layerC.h"
#include <stdio.h>

void createDense(Dense* dense, size_t input_dim, size_t output_dim, quant_bit_width *weight, quant_bit_width* bias) {
    dense->input_size_ = input_dim;
    dense->output_size_ = output_dim;
    dense->weight = weight;
    dense->bias = bias;
}

void destroyDense(Dense* dense) {
    // Free the memory allocated for the Dense struct
    free(dense);
}

void multiplyweight(Dense* dense, size_t seq_len, int32_t* input, int32_t* output) {
    for (int length = 0; length < seq_len; length++) {
        for (int out_idx = 0; out_idx < dense->output_size_; out_idx++) {
            int32_t* weight_ptr = dense->weight + out_idx;
            int32_t* output_ptr = output + (length * dense->output_size_) + out_idx;
            int32_t* input_ptr = input + (length * dense->input_size_);
            int32_t sum = 0;
            for (int i = 0; i < dense->input_size_; i++) {
                sum += MUL_HQ(*weight_ptr, *input_ptr); // MUL_HQ macro
                input_ptr++;
                weight_ptr += dense->output_size_;
            }
            *(output_ptr) = (int32_t) (sum >> NUM_FRACTION_BITS); // NUM_FRACTION_BITS macro
        }
    }
}

void addbias(Dense* dense, size_t seq_len, int32_t* output) {
    for (size_t idx = 0; idx < seq_len; idx++) {
        for (size_t feature_idx = 0; feature_idx < dense->output_size_; feature_idx++) {
            output[idx * dense->output_size_ + feature_idx] += dense->bias[feature_idx];
        }
    }
}

void computeDenseOneRow(Dense* dense, size_t seq_len, int32_t* input, int32_t* output){ // 1x16x16
    multiplyweight(dense, seq_len, input, output);
}

void computeDense(Dense* dense, size_t seq_len, int32_t* input, int32_t* output) {
    //multiplyweight(dense, seq_len, input, output);
    //printf("\rMul %dx%dx%d\n", seq_len, dense->input_size_, dense->output_size_);
    multiply_cgra(input, seq_len, dense->input_size_, dense->weight, dense->output_size_, output);
    if (dense->bias != NULL) {
        addbias(dense, seq_len, output);
    }
}

void activation(Dense* dense, size_t length, int32_t* input, int32_t* output) {
    float in_float, in_tanh;
    int32_t x3, in_tanh_fxp;
    for (int i = 0; i < length; i++) {
        x3 = MUL(MUL(input[i], input[i]), input[i]);
        x3 = MUL(x3, 183); // 183 = 0.044715 in fixed-point 12 bit
        x3 += input[i];
        x3 = MUL(x3, 3268); // 3268 = sqrt(2/PI) in fixed-point 12 bit
        in_float = (float) x3 / (float) (1 << NUM_FRACTION_BITS);
        in_tanh = tanhf(in_float);
        in_tanh_fxp = (int32_t) (in_tanh * (1 << NUM_FRACTION_BITS));
        in_tanh_fxp += (1 << NUM_FRACTION_BITS);
        output[i] = MUL(in_tanh_fxp, input[i] >> 1);
    }
}

