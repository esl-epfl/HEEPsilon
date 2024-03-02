#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

unsigned reversebits ( unsigned index, unsigned NumBits )
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