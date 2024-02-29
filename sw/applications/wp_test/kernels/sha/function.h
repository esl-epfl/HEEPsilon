#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

#define N_ITERS 80

uint32_t sha_transform(int W[]){
	for (int i = 16; i < N_ITERS; ++i)
		W[i] = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
	return (int32_t*)W;
}

#endif // _CGRA_FUNCTION_H_