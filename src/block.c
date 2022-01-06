#include <stdlib.h>

#include "block.h"
#include "logger.h"
#include "dct.h"


block* block_init() {
    block* bl = (block*)malloc(sizeof(block));

    if (bl == NULL) {
        logger_log_and_exit("Cannot allocate memory for block");
    }

    return bl;
}

void block_int_copy(int_block* src, int_block* dest) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest->line[i] = src->line[i]; 
    }
}

void block_destroy(block* bl) {
    if (bl == NULL) {
        logger_log("Cannot destroy empty block");
        return;
    }

    free(bl);
}

void block_show(block* bl) {
    for (size_t i = 0; i < BLOCK_WIDTH; i++) {
        for (size_t j = 0; j < BLOCK_WIDTH; j++) {
            printf("%3d ", bl->matrix[i][j]);
        }
        printf("\n");
    }
}

int_block* block_int_init() {
    int_block* ibl = (int_block*)malloc(sizeof(int_block));

    if (ibl == NULL) {
        logger_log_and_exit("Cannot allocate memory for int_block");
    }

    return ibl;
}

void block_int_destroy(int_block* ibl) {
    if (ibl == NULL) {
        logger_log("Cannot destroy empty int_block");
        return;
    }

    free(ibl);
}

void block_int_show(int_block* bl) {
    for (size_t i = 0; i < BLOCK_WIDTH; i++) {
        for (size_t j = 0; j < BLOCK_WIDTH; j++) {
            printf("%5d ", bl->matrix[i][j]);
        }
        printf("\n");
    }
}

// Копирование обычного блока в интовый блок
void block_copy_bl_to_ibl(block *bl, int_block *ibl) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        ibl->line[i] = bl->line[i];
    }
}

// Копирование интового блока в обычный блок
void block_copy_ibl_to_bl(int_block *ibl, block *bl) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        bl->line[i] = ibl->line[i];
    }
}

void block_run_dct(block* bl, int_block* ibl) {
    block_copy_bl_to_ibl(bl, ibl);
    DCT_16x16(ibl->line);
}

void block_run_idct(int_block* ibl, block* bl) {
    int_block temp;

    IDCT_16x16(ibl->line, temp.line);
    block_copy_ibl_to_bl(&temp, bl);
}


blocks* blocks_init() {
    blocks* bls = (blocks*)malloc(sizeof(blocks));

    if (bls == NULL) {
        logger_log_and_exit("Cannot allocate memory for blocks");
    }

    return bls;
}

void blocks_destroy(blocks* bls) {
    if (bls == NULL) {
        logger_log("Cannot destroy empty blocks");
        return;
    }

    free(bls);
}

int_blocks* blocks_int_init() {
    int_blocks* ibls = (int_blocks*)malloc(sizeof(int_blocks));

    if (ibls == NULL) {
        logger_log_and_exit("Cannot allocate memory for int blocks");
    }

    return ibls;
}

void blocks_int_destroy(int_blocks *ibls) {
    if (ibls == NULL) {
        logger_log("Cannot destroy empty int blocks");
        return;
    }

    free(ibls);
}

void blocks_run_dct(blocks* bl, int_blocks* ibl) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        block_run_dct(&bl->line[i], &ibl->line[i]);
    }
}

void blocks_run_idct(int_blocks *ibl, blocks* bl) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        block_run_idct(&ibl->line[i], &bl->line[i]);
    }
}

