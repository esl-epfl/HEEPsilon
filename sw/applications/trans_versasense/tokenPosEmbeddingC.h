//
// Created by alireza on 10/6/23.
//

#ifndef FVLLMONTITRANSFORMER_TOKENPOSEMBEDDINGC_H
#define FVLLMONTITRANSFORMER_TOKENPOSEMBEDDINGC_H

#include "param.h"
#include "stdlib.h"

typedef struct{
    quant_bit_width* cls_token_vector_;
    quant_bit_width* pos_matrix_;
    size_t seq_len_;
    size_t input_dim_;
} TokenPosEmbedding;


void createTokenPosEmbedding(TokenPosEmbedding*, quant_bit_width* pos_matrix, quant_bit_width* cls_token_vector, size_t seq_len, size_t input_dim, size_t pos_matrix_dim);
void clsConcatenate(TokenPosEmbedding* tpe, quant_bit_width* input, quant_bit_width* concatenated_input);
void posEmbedding(TokenPosEmbedding* tpe, quant_bit_width* input);


#endif //FVLLMONTITRANSFORMER_TOKENPOSEMBEDDINGC_H
