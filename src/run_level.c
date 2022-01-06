#include "run_level.h"

#include "logger.h"

#include <stdlib.h>

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

void run_level_run(int_block* bl, run_level_int_block* rlibl) {
    rlibl->size = 0;

    int value = bl->line[0];
    int zero_count = 0;

    size_t j = 0;
    for (size_t i = 1; i < BLOCK_SIZE; i++) {
        if (bl->line[i] == 0) {
            zero_count++;
        } else {
            rlibl->line[j++] = value;
            rlibl->line[j++] = zero_count;
            rlibl->size += 2;

            zero_count = 0;
            value = bl->line[i];
        }
    }

    rlibl->line[j++] = value;
    rlibl->line[j++] = zero_count;
    rlibl->size += 2;
}

void run_level_inverse_run(run_level_int_block* rlibl, int_block* bl) {
    size_t k = 0;

    for (size_t i = 0; i < rlibl->size; i += 2) {
        bl->line[k++] = rlibl->line[i];

        for (size_t j = 0; j < rlibl->line[i + 1]; j++)
            bl->line[k++] = 0;
    }
}
