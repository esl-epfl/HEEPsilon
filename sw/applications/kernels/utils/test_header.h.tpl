#ifndef _CGRA_BITSTREAM_H_
#define _CGRA_BITSTREAM_H_

#include <stdint.h>

#include "cgra.h"

// Kernel 0 => NULL
${kernel_id}

uint32_t cgra_kem_bitstream[CGRA_KMEM_SIZE] = {
${word_config}
};

uint32_t cgra_imem_bistream[CGRA_IMEM_SIZE] = {
${machine_code}
};

#endif // _CGRA_BITSTREAM_H_
