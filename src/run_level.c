#include "run_level.h"

#include "logger.h"

#include <stdlib.h>

run_level_int_blocks* run_level_int_blocks_init() {
    run_level_int_blocks* rlibls = (run_level_int_blocks*)malloc(sizeof(run_level_int_blocks));

    if (rlibls == NULL) {
        logger_log("Cannot allocate drun level int blocks");
    }

    return rlibls;
}

void run_level_int_blocks_destroy(run_level_int_blocks* rlibls) {
    if (rlibls == NULL) {
        logger_log("Cannot destroy run level int blocks");
        return;
    }

    free(rlibls);
}

run_level_int_block* run_level_int_block_init() {
    run_level_int_block* rlibl = (run_level_int_block*)malloc(sizeof(run_level_int_block));

    if (rlibl == NULL) {
        logger_log_and_exit("Cannot allocate memory for run level int block");
    }

    return rlibl;
}

void run_level_int_block_destroy(run_level_int_block* rlibl) {
    if (rlibl == NULL) {
        logger_log("Cannot destroy empty run level int block");
        return;
    }

    free(rlibl);
}

void run_level_run_block(int_block* ibl, run_level_int_block* rlibl) {
    rlibl->size = 0;

    int value = ibl->line[0];
    int zero_count = 0;

    size_t j = 0;
    for (size_t i = 1; i < BLOCK_SIZE; i++) {
        if (ibl->line[i] == 0) {
            zero_count++;
        } else {
            rlibl->line[j++] = value;
            rlibl->line[j++] = zero_count;
            rlibl->size += 2;

            zero_count = 0;
            value = ibl->line[i];
        }
    }

    rlibl->line[j++] = value;
    rlibl->line[j++] = zero_count;
    rlibl->size += 2;
}

void run_level_run_blocks(int_blocks* ibls, run_level_int_blocks* rlibls) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        run_level_run_block(&ibls->line[i], &rlibls->line[i]);
    }
}

void run_level_inverse_run_block(run_level_int_block* rlibl, int_block* bl) {
    size_t k = 0;

    for (size_t i = 0; i < rlibl->size; i += 2) {
        bl->line[k++] = rlibl->line[i];

        for (size_t j = 0; j < rlibl->line[i + 1]; j++)
            bl->line[k++] = 0;
    }
}

void run_level_inverse_run_blocks(run_level_int_blocks* rlibls, int_blocks* ibls) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        run_level_inverse_run_block(&rlibls->line[i] ,&ibls->line[i]);
    }
}
