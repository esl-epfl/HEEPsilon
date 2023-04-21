#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

void sha_transform(int W[]){
	for (int i = 16; i < 80; ++i) 
		W[i] = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
}

#endif // _CGRA_FUNCTION_H_