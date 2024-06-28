//
// Created by alireza on 10/6/23.
//

#ifndef FVLLMONTITRANSFORMER_SELFATTENTIONC_H
#define FVLLMONTITRANSFORMER_SELFATTENTIONC_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "dense_layerC.h"
#include "softmaxC.h"
#include "transposeC.h"
#include "matMulC.h"
#include "param.h"
#include "cgra.h"

typedef struct {
    Dense* query_layer;
    Dense* key_layer;
    Dense* value_layer;
    int32_t* query_layer_out;
    int32_t* key_layer_out;
    int32_t* key_transposed_layer_out;
    int32_t* value_layer_out;
    int32_t* attention_scores;
    size_t pre_seq_len;
    size_t head_hidden_size;
} SingleHeadSelfAttn;

void create_SingleHeadSelfAttn(SingleHeadSelfAttn*, size_t pre_seq_len, size_t input_dim, size_t head_hidden_size, int32_t** weightVector);
void destroy_SingleHeadSelfAttn(SingleHeadSelfAttn* self_attn);
void compute_SingleHeadSelfAttn(SingleHeadSelfAttn* self_attn, int32_t* input, int32_t* output, int32_t* qkv, int32_t* intermediate);

#endif //FVLLMONTITRANSFORMER_SELFATTENTIONC_H
