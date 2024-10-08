//
// Created by alireza on 10/6/23.
//

#include <stdio.h>
#include "selfattentionC.h"


void create_SingleHeadSelfAttn(SingleHeadSelfAttn* self_attn, size_t pre_seq_len, size_t input_dim, size_t head_hidden_size, int32_t** weightVector) {
    self_attn->pre_seq_len = pre_seq_len;
    self_attn->head_hidden_size = head_hidden_size;
    createDense(self_attn->query_layer, input_dim, head_hidden_size, weightVector[0], NULL);
    createDense(self_attn->key_layer, input_dim, head_hidden_size, weightVector[1], NULL);
    createDense(self_attn->value_layer, input_dim, head_hidden_size, weightVector[2], NULL);
}

void destroy_SingleHeadSelfAttn(SingleHeadSelfAttn* self_attn) {
    free(self_attn->query_layer_out);
    free(self_attn->key_layer_out);
    free(self_attn->key_transposed_layer_out);
    free(self_attn->value_layer_out);
    free(self_attn->attention_scores);

    destroyDense(self_attn->query_layer);
    destroyDense(self_attn->key_layer);
    destroyDense(self_attn->value_layer);

    free(self_attn);
}

void removeExtraCols(int32_t* input, size_t rows, int cols) {
    int nElems = 3;
    for(int r = 1; r < rows; r++, nElems+=3){
        for(int c = 0; c < cols -3; c++){
            input[r*cols+c -nElems] = input[r*cols+c];
        }
    }
}

void compute_SingleHeadSelfAttn(SingleHeadSelfAttn* self_attn, int32_t* input, int32_t* output, int32_t* qkv, int32_t* intermediate) {
    self_attn->query_layer_out = qkv;
    self_attn->key_layer_out = qkv + self_attn->pre_seq_len * self_attn->head_hidden_size;
    self_attn->value_layer_out = qkv + 2 * self_attn->pre_seq_len * self_attn->head_hidden_size;
    self_attn->key_transposed_layer_out = qkv + 3 * self_attn->pre_seq_len * self_attn->head_hidden_size;

    // This 3 mmul need to think they have 124 rows instead of 121
    // TODO: Check that the outputs have enough space for the extra 3 rows
    computeDense(self_attn->query_layer, self_attn->pre_seq_len +3, input, self_attn->query_layer_out); // 121x16x4
    computeDense(self_attn->key_layer, self_attn->pre_seq_len +3, input, self_attn->key_layer_out); // 121x16x4
    computeDense(self_attn->value_layer, self_attn->pre_seq_len +3, input, self_attn->value_layer_out); // 121x16x4

    transpose_quant(self_attn->key_layer_out, self_attn->key_transposed_layer_out, self_attn->pre_seq_len +3, self_attn->head_hidden_size);
    MatMul_scale(self_attn->key_transposed_layer_out, 1, (self_attn->pre_seq_len +3) * self_attn->head_hidden_size);

    // 121x4x121
    MatMul_multiply(self_attn->pre_seq_len +3, self_attn->query_layer_out, self_attn->key_transposed_layer_out, intermediate, self_attn->head_hidden_size, self_attn->pre_seq_len +3);
    
    printf("\rRemove cols\n");
    // 121x124 -> 121x121 Remove extra cols
    removeExtraCols(intermediate, self_attn->pre_seq_len, 124);

    printf("\rSoftmax\n");
    // Now intermediate is 121x121
    computeSoftmax(intermediate, self_attn->pre_seq_len);
    
    // 121x121x4
    MatMul_multiply(self_attn->pre_seq_len +3, intermediate, self_attn->value_layer_out, output, self_attn->pre_seq_len, self_attn->head_hidden_size);
}
