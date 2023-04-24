#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

uint32_t sha(int W[]){
	for (int i = 16; i < 80; ++i) 
		W[i] = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
	return (int32_t*)W;
}

#endif // _CGRA_FUNCTION_H_