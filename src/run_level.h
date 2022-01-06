#pragma once

#include "block.h"

typedef struct {
    int line[BLOCK_SIZE * 2];
    size_t size;
} run_level_int_block;

typedef struct {
    run_level_int_block line[BLOCKS_YUV_SIZE];
} run_level_int_blocks;

run_level_int_blocks* run_level_int_blocks_init();

void run_level_int_blocks_destroy();

run_level_int_block* run_level_int_block_init();

void run_level_int_block_destroy(run_level_int_block* rlibl);

void run_level_run_block(int_block* ibl, run_level_int_block* rlibl);

void run_level_run_blocks(int_blocks* ibls, run_level_int_blocks* rlibls);

void run_level_inverse_run_block(run_level_int_block* rlibl, int_block* bl);

void run_level_inverse_run_blocks(run_level_int_blocks* rlibls, int_blocks* ibls);

