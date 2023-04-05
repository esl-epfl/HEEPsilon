#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

/*
num_cols=4

read_col0
unsigned index 0,UINT_MAX
unsigned NumBits 0,32

read_col1 

read_col2 

read_col3 

write_col0 

write_col1 

write_col2
unsigned return

write_col3 

*/

/* 
  Revert a specified number of bits (starting from lsb) of the variable index
*/
unsigned ReverseBits ( unsigned index, unsigned NumBits )
{
    unsigned i, rev;

    for ( i=rev=0; i < NumBits; i++ )
    {
        rev = (rev << 1) | (index & 1);
        index >>= 1;
    }

    return rev;
}

#endif // _CGRA_FUNCTION_H_