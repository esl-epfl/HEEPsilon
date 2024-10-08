//
// Created by alireza on 10/6/23.
//
#include "weightsAndBiasesC.h"
#include <stdint.h>
#include <stddef.h>
#include "data_cpp/data.cpp"

void getWeights(quant_bit_width * weightVec[]){

    int weightVectorIndex = 0;
    weightVec[weightVectorIndex++] = to_patch_embedding_layer_norm1_weight;
    weightVec[weightVectorIndex++] = to_patch_embedding_linear_weight;
    weightVec[weightVectorIndex++] = to_patch_embedding_layer_norm2_weight;


    /* *************************** Layer 1 ***************************** */
    weightVec[weightVectorIndex++] = transformer_layers_0_0_norm_weight;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_Q_H0;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_K_H0;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_V_H0;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_Q_H1;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_K_H1;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_V_H1;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_Q_H2;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_K_H2;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_V_H2;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_Q_H3;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_K_H3;
    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_to_qkv_weight_V_H3;

    weightVec[weightVectorIndex++] = transformer_layers_0_0_fn_projection_weight;

    weightVec[weightVectorIndex++] = transformer_layers_0_1_norm_weight;

    weightVec[weightVectorIndex++] = transformer_layers_0_1_fn_ff1_weight;
    weightVec[weightVectorIndex++] = transformer_layers_0_1_fn_ff2_weight;

    /* *************************** Layer 2 ***************************** */
    weightVec[weightVectorIndex++] = transformer_layers_1_0_norm_weight;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_Q_H0;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_K_H0;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_V_H0;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_Q_H1;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_K_H1;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_V_H1;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_Q_H2;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_K_H2;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_V_H2;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_Q_H3;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_K_H3;
    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_to_qkv_weight_V_H3;

    weightVec[weightVectorIndex++] = transformer_layers_1_0_fn_projection_weight;

    weightVec[weightVectorIndex++] = transformer_layers_1_1_norm_weight;

    weightVec[weightVectorIndex++] = transformer_layers_1_1_fn_ff1_weight;
    weightVec[weightVectorIndex++] = transformer_layers_1_1_fn_ff2_weight;

    /* *************************** Layer 3 ***************************** */
    weightVec[weightVectorIndex++] = transformer_layers_2_0_norm_weight;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_Q_H0;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_K_H0;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_V_H0;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_Q_H1;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_K_H1;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_V_H1;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_Q_H2;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_K_H2;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_V_H2;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_Q_H3;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_K_H3;
    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_to_qkv_weight_V_H3;

    weightVec[weightVectorIndex++] = transformer_layers_2_0_fn_projection_weight;

    weightVec[weightVectorIndex++] = transformer_layers_2_1_norm_weight;

    weightVec[weightVectorIndex++] = transformer_layers_2_1_fn_ff1_weight;
    weightVec[weightVectorIndex++] = transformer_layers_2_1_fn_ff2_weight;

    /* *************************** Layer 4 ***************************** */
    weightVec[weightVectorIndex++] = transformer_layers_3_0_norm_weight;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_Q_H0;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_K_H0;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_V_H0;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_Q_H1;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_K_H1;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_V_H1;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_Q_H2;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_K_H2;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_V_H2;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_Q_H3;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_K_H3;
    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_to_qkv_weight_V_H3;

    weightVec[weightVectorIndex++] = transformer_layers_3_0_fn_projection_weight;

    weightVec[weightVectorIndex++] = transformer_layers_3_1_norm_weight;

    weightVec[weightVectorIndex++] = transformer_layers_3_1_fn_ff1_weight;
    weightVec[weightVectorIndex++] = transformer_layers_3_1_fn_ff2_weight;

    /* *************************** MLP HEAD ***************************** */
    weightVec[weightVectorIndex++] = mlp_head_layer_norm_weight;
    weightVec[weightVectorIndex++] = mlp_head_linear_weight;

}

void getBiases(quant_bit_width * biasVec[]){
    int biasVectorIndex = 0;
    biasVec[biasVectorIndex++] = to_patch_embedding_layer_norm1_bias;
    biasVec[biasVectorIndex++] = to_patch_embedding_linear_bias;
    biasVec[biasVectorIndex++] = to_patch_embedding_layer_norm2_bias;

    /* *************************** Layer 1 ***************************** */
    biasVec[biasVectorIndex++] = transformer_layers_0_0_norm_bias;
    for (int i=0; i<3*4; i++)
        biasVec[biasVectorIndex++] = (quant_bit_width *) NULL;

    biasVec[biasVectorIndex++] = transformer_layers_0_0_fn_projection_bias;
    biasVec[biasVectorIndex++] = transformer_layers_0_1_norm_bias;

    biasVec[biasVectorIndex++] = transformer_layers_0_1_fn_ff1_bias;
    biasVec[biasVectorIndex++] = transformer_layers_0_1_fn_ff2_bias;

    /* *************************** Layer 2 ***************************** */
    biasVec[biasVectorIndex++] = transformer_layers_1_0_norm_bias;

    for (int i=0; i<3*4; i++)
        biasVec[biasVectorIndex++] = (quant_bit_width *) NULL;

    biasVec[biasVectorIndex++] = transformer_layers_1_0_fn_projection_bias;
    biasVec[biasVectorIndex++] = transformer_layers_1_1_norm_bias;

    biasVec[biasVectorIndex++] = transformer_layers_1_1_fn_ff1_bias;
    biasVec[biasVectorIndex++] = transformer_layers_1_1_fn_ff2_bias;

    /* *************************** Layer 3 ***************************** */
    biasVec[biasVectorIndex++] = transformer_layers_2_0_norm_bias;

    for (int i=0; i<3*4; i++)
        biasVec[biasVectorIndex++] = (quant_bit_width *) NULL;

    biasVec[biasVectorIndex++] = transformer_layers_2_0_fn_projection_bias;
    biasVec[biasVectorIndex++] = transformer_layers_2_1_norm_bias;

    biasVec[biasVectorIndex++] = transformer_layers_2_1_fn_ff1_bias;
    biasVec[biasVectorIndex++] = transformer_layers_2_1_fn_ff2_bias;

    /* *************************** Layer 4 ***************************** */
    biasVec[biasVectorIndex++] = transformer_layers_3_0_norm_bias;

    for (int i=0; i<3*4; i++)
        biasVec[biasVectorIndex++] = (quant_bit_width *) NULL;

    biasVec[biasVectorIndex++] = transformer_layers_3_0_fn_projection_bias;
    biasVec[biasVectorIndex++] = transformer_layers_3_1_norm_bias;

    biasVec[biasVectorIndex++] = transformer_layers_3_1_fn_ff1_bias;
    biasVec[biasVectorIndex++] = transformer_layers_3_1_fn_ff2_bias;

    /* *************************** MLP HEAD ***************************** */
    biasVec[biasVectorIndex++] = mlp_head_layer_norm_bias;
    biasVec[biasVectorIndex++] = mlp_head_linear_bias;
}

quant_bit_width * getPosEmbedding(){
    quant_bit_width * posMatrix;
    posMatrix = pos_embedding;
    return posMatrix;
}

quant_bit_width * getClassToken(){
    quant_bit_width * clsTokenVector;
    clsTokenVector = cls_token;
    return clsTokenVector;
}


