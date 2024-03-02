#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

/*
num_cols=4

read_col0

read_col1 

read_col2 

read_col3 
long x 0,UINT_MAX

write_col0 
int return

write_col1 

write_col2

write_col3 

*/

/* 
  Revert a specified number of bits (starting from lsb) of the variable index
*/
int bit_count(long x)
{
        int n = 0;

        if (x == 0){
            return 0;
        }
        
        do{
          n++;
        }while (0 != (x = x&(x-1)));

        return(n);
}

#endif // _CGRA_FUNCTION_H_