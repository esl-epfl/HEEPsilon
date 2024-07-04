//
// Created by alireza on 10/6/23.
//

#include "tokenPosEmbeddingC.h"


void createTokenPosEmbedding(TokenPosEmbedding* tokenPosEmbedding, quant_bit_width* pos_matrix, quant_bit_width* cls_token_vector, size_t seq_len, size_t input_dim, size_t pos_matrix_dim) {
    tokenPosEmbedding->cls_token_vector_ = cls_token_vector;
    tokenPosEmbedding->pos_matrix_ = pos_matrix;
    tokenPosEmbedding->seq_len_ = seq_len;
    tokenPosEmbedding->input_dim_ = input_dim;
}

void clsConcatenate(TokenPosEmbedding* tpe, quant_bit_width* input, quant_bit_width* concatenated_input) {
    // Copy cls_token_ into the concatenated array column-wise at the beginning
    for (size_t i = 0; i < tpe->input_dim_; ++i) {
        concatenated_input[i] = tpe->cls_token_vector_[i];
    }
    // Copy the input array into the concatenated array
    for (size_t i = 0; i < tpe->seq_len_ * tpe->input_dim_; ++i) {
        concatenated_input[i + tpe->input_dim_] = input[i];
    }
}

void posEmbedding(TokenPosEmbedding* tpe, quant_bit_width* input) {
    for (size_t i = 0; i < (tpe->seq_len_ + 1); ++i) {
        for (size_t j = 0; j < tpe->input_dim_; ++j) {
            input[i * tpe->input_dim_+ j] += tpe->pos_matrix_[i * tpe->input_dim_ + j];
        }
    }
}

