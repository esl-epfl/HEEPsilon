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

void add_padding(int32_t *input, int32_t *output, int rows, int columns, int padding_width) {
    // Calcular el nuevo ancho de la matriz con el relleno
    int new_columns = columns + padding_width;

    // Crear una nueva matriz con el nuevo ancho y copiar los elementos de la matriz original
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            output[i * new_columns + j] = input[i * columns + j];
        }
        // Rellenar las nuevas columnas con ceros
        for (int j = columns; j < new_columns; j++) {
            output[i * new_columns + j] = 0; // O cualquier otro valor de relleno que desees
        }
    }
}

void remove_padding(int *matrix, int rows, int columns, int padding_width) {
    // Calculate the new width of the matrix after removing padding
    int new_columns = columns - padding_width;

    // Shift the elements of each row to remove the padding
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < new_columns; j++) {
            matrix[i * new_columns + j] = matrix[i * columns + j];
        }
    }
}


void compute_SingleHeadSelfAttn(SingleHeadSelfAttn* self_attn, int32_t* input, int32_t* output, int32_t* qkv, int32_t* intermediate, int32_t* aux_padding) {
    self_attn->query_layer_out = qkv;
    self_attn->key_layer_out = qkv + self_attn->pre_seq_len * self_attn->head_hidden_size;
    self_attn->value_layer_out = qkv + 2 * self_attn->pre_seq_len * self_attn->head_hidden_size;
    self_attn->key_transposed_layer_out = qkv + 3 * self_attn->pre_seq_len * self_attn->head_hidden_size;

    // This 3 mmul need to think they have 16 rows instead of 13
    int padding = 3;
    // TODO: Check that the outputs have enough space for the extra padding rows and cols
    computeDense(self_attn->query_layer, self_attn->pre_seq_len + padding, input, self_attn->query_layer_out); // 13x16x4 (16x16x4)
    computeDense(self_attn->key_layer, self_attn->pre_seq_len + padding, input, self_attn->key_layer_out); // 13x16x4 (16x16x4)
    computeDense(self_attn->value_layer, self_attn->pre_seq_len + padding, input, self_attn->value_layer_out); // 13x16x4 (16x16x4)

    transpose_quant(self_attn->key_layer_out, self_attn->key_transposed_layer_out, self_attn->pre_seq_len +3, self_attn->head_hidden_size); // 4x13
    MatMul_scale(self_attn->key_transposed_layer_out, 1, self_attn->pre_seq_len * self_attn->head_hidden_size); //4x13

    // Add padding 4x13 -> 4x16
    add_padding(self_attn->key_transposed_layer_out, aux_padding, self_attn->pre_seq_len, self_attn->head_hidden_size, padding); // 4x16

    // 13x4x13 (16x4x16)
    MatMul_multiply(self_attn->pre_seq_len + padding, self_attn->query_layer_out, aux_padding, intermediate, self_attn->head_hidden_size, self_attn->pre_seq_len + padding);
    
    // 13x16 -> 13x13
    remove_padding(intermediate, self_attn->pre_seq_len, self_attn->pre_seq_len, padding);

    //printf("\rSoftmax\n");
    computeSoftmax(intermediate, self_attn->pre_seq_len);
    
    // 13x13x4
    MatMul_multiply(self_attn->pre_seq_len + padding, intermediate, self_attn->value_layer_out, output, self_attn->pre_seq_len, self_attn->head_hidden_size);
}
