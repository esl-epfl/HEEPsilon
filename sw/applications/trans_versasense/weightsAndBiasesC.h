//
// Created by alireza on 10/6/23.
//

#ifndef FVLLMONTITRANSFORMER_WEIGHTSANDBIASESC_H
#define FVLLMONTITRANSFORMER_WEIGHTSANDBIASESC_H

#include "param.h"

void getWeights(quant_bit_width * weightVec[]);
void getBiases(quant_bit_width * biasVec[]);
quant_bit_width * getPosEmbedding();
quant_bit_width * getClassToken();

#endif //FVLLMONTITRANSFORMER_WEIGHTSANDBIASESC_H
