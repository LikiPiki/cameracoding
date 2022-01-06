#pragma once

#include "block.h"

#define QUANTIZE_DEFAULT_LEVEL (256)

void quantize_int_block(int_block *ibl, int32_t level);

void quantize_int_blocks(int_blocks *ibls, int32_t level);

void quantize_inverse_int_block(int_block *ibl, int32_t level);

void quantize_inverse_int_blocks(int_blocks *ibls, int32_t level);

