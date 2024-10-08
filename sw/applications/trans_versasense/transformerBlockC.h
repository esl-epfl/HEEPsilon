//
// Created by alireza on 10/6/23.
//

#ifndef FVLLMONTITRANSFORMER_TRANSFORMERBLOCK_H
#define FVLLMONTITRANSFORMER_TRANSFORMERBLOCK_H

#include <stddef.h>
#include <stdint.h>
#include "selfattentionC.h"
#include "addNormC.h"
#include "dense_layerC.h"
#include "tokenPosEmbeddingC.h"
#include "param.h"
#include "transposeC.h"
#include "cgra.h"

typedef struct {
    size_t num_heads_;
    size_t head_hidden_size_;
    size_t input_dim_;
    size_t ff_size_;
    SingleHeadSelfAttn* selfatten[NUM_LAYERS*NUM_HEAD];
    int32_t* multihead_out;
    int32_t* condense_out;
    int32_t* intermediateFF;
    int32_t* intermediateFFBlockWise;
    AddNormalize addNorm;
    AddNormalize addNorm2;
    AddNormalize transformer_layer_0_addNorm[NUM_LAYERS];
    AddNormalize transformer_layer_1_addNorm[NUM_LAYERS];
    AddNormalize mlp_head_norm;
    TokenPosEmbedding* token;
    Dense* condense[NUM_LAYERS];
    Dense* feedForward0[NUM_LAYERS];
    Dense* feedForward1[NUM_LAYERS];
    Dense* patchEmbedding;
    Dense* mlp_head_linear;
    #ifndef REARRANGE
    int32_t* multihead_out_reshape;
    #endif
} TransformerBlock;

TransformerBlock* createTransformerBlock(size_t pre_seq_len, size_t input_dim, size_t head_hidden_size, size_t num_heads, size_t ff_size, int32_t** weightVector, int32_t** biasVector, int32_t* clsTokenVector, int32_t* posMatrix);
void destroyTransformerBlock(TransformerBlock* transformerBlock);
void computeFixedPoint(TransformerBlock* transformerBlock, size_t seq_len, int32_t* input, int32_t* input_normalized, int32_t* output, int32_t* intermediate, int32_t* aux_padding, int32_t* qkv, void * kperf);

#endif //FVLLMONTITRANSFORMER_TRANSFORMERBLOCK_H
