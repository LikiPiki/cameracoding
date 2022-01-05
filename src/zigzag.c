#include <stdio.h>

#include "zigzag.h"

void zigzag_int_block(int_block *ibl) {
    int_block tmp;

    ssize_t x = 0, y = 0, j = 0;
    ssize_t *px = &x, *py = &y;

    for (size_t i = 0; i < BLOCK_WIDTH * 2 - 1; i++) {

        do {
            tmp.line[j++] = ibl->matrix[y][x];
            (*px)++, (*py)--;
        } while (x >= 0 && y >= 0 && x < BLOCK_WIDTH && y < BLOCK_WIDTH);

        if (*px == BLOCK_WIDTH) 
            (*px)--, *py += 2;
        else 
            (*py)++;

        ssize_t* tmp = px;
        px = py;
        py = tmp;
    }

    block_int_copy(&tmp, ibl);
}

void zigzag_int_blocks(int_blocks *ibl) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        zigzag_int_block(&ibl->line[i]);
    }
}

void zigzag_inverse_int_block(int_block *ibl) {
    int_block tmp;

    ssize_t x = BLOCK_WIDTH - 1, y = BLOCK_WIDTH - 1, j = BLOCK_SIZE - 1;
    ssize_t *px = &x, *py = &y;

    for (size_t i = 0; i < BLOCK_WIDTH * 2 - 1; i++) {

        do {
            tmp.matrix[y][x] = ibl->line[j--];
            (*px)--, (*py)++;
        } while (x >= 0 && y >= 0 && x < BLOCK_WIDTH && y < BLOCK_WIDTH);

        if (*px == -1) 
            (*px)++, *py -= 2;
        else 
            (*py)--;

        ssize_t* tmp = px;
        px = py;
        py = tmp;
    }

    block_int_copy(&tmp, ibl);
}

void zigzag_inverse_int_blocks(int_blocks *ibl) {
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        zigzag_inverse_int_block(&ibl->line[i]);
    }
}
