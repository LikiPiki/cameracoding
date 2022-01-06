#pragma once

#include "block.h"

typedef struct {
    int line[BLOCK_SIZE * 2];
    size_t size;
} run_level_int_block;

run_level_int_block* run_level_int_block_init();

void run_level_int_block_destroy(run_level_int_block* rlibl);

void run_level_run(int_block* bl, run_level_int_block* rlibl);

void run_level_inverse_run(run_level_int_block* rlibl, int_block* bl);
