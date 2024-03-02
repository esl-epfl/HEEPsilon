#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "conv.h"
#include "dma.h"
// im2col parameters

#define col_im2col (row_filter * col_filter * C_filter)
#define row_im2col (row_output * col_output)



void __attribute((noinline)) im2col_DMA(int32_t *input_to_CGRA, int32_t *filter_to_CGRA, int out_row, int out_col, int output_channel)
{

  int i, j, k, l, c, m, n, o, p, q, r, s, t, u, v, w;
  for (i = 0; i < row_filter; i++)
      {
            
            input_to_CGRA[w+C_filter*j+C_filter*col_filter*i]=input[0][i+out_row][j+out_col][w]; 
            filter_to_CGRA[w+C_filter*j+C_filter*col_filter*i] = filter[output_channel][i][j][w];  
            //if(output_channel == 1)
            //printf("input: %d, filter: %d \n", input_to_CGRA[w+C_filter*j+C_filter*col_filter*i], filter_to_CGRA[w+C_filter*j+C_filter*col_filter*i]);            
                  
            
      }
}
