//
// Created by alireza on 10/6/23.
//

#ifndef FVLLMONTITRANSFORMER_SOFTMAXC_H
#define FVLLMONTITRANSFORMER_SOFTMAXC_H

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "param.h"

void computeSoftmax(int32_t* input, size_t seq_len);


#endif //FVLLMONTITRANSFORMER_SOFTMAXC_H
