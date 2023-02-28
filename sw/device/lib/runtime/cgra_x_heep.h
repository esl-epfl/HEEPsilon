// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef CGRA_X_HEEP_H_
#define CGRA_X_HEEP_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "core_v_mini_mcu.h"

#define EXT_XBAR_NMASTER 4
#define EXT_XBAR_NSLAVE  1

#define CGRA_START_ADDRESS (EXT_SLAVE_START_ADDRESS + 0x000000)
#define CGRA_SIZE 0x100000
#define CGRA_END_ADDRESS (CGRA_START_ADDRESS + CGRA_SIZE)

#define CGRA_PERIPH_START_ADDRESS (EXT_PERIPHERAL_START_ADDRESS + 0x0000000)
#define CGRA_PERIPH_SIZE 0x0001000
#define CGRA_PERIPH_END_ADDRESS (CGRA_PERIPH_START_ADDRESS + CGRA_PERIPH_SIZE)

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // CGRA_X_HEEP_H_
