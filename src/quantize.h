#pragma once

#include "block.h"

#define QUANTIZE_DEFAULT_LEVEL (64)

void quantize_int_block(int_block *ibl, size_t level);

void quantize_int_blocks(int_blocks *ibls, size_t level);

void quantize_inverse_int_block(int_block *ibl, size_t level);

void quantize_inverse_int_blocks(int_blocks *ibls, size_t level);

