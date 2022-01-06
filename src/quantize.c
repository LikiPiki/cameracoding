#include "quantize.h"

void quantize_int_block(int_block *ibl, int32_t level) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        ibl->line[i] /= level;
    }
}

void quantize_int_blocks(int_blocks *ibls, int32_t level) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        quantize_int_block(&ibls->line[i], level);
    }
}

void quantize_inverse_int_block(int_block *ibl, int32_t level) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        ibl->line[i] *= level;
    }
}

void quantize_inverse_int_blocks(int_blocks *ibls, int32_t level) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        quantize_inverse_int_block(&ibls->line[i], level);
    }
}
