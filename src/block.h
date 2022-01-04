#pragma once

#include <stdint.h>
#include <stddef.h>

#define BLOCK_WIDTH (16)
#define BLOCK_SIZE (BLOCK_WIDTH * BLOCK_WIDTH)

/**
 * Основной блок кадра 16x16 пикселей
 */
typedef struct {
    union {
        uint8_t matrix[BLOCK_WIDTH][BLOCK_WIDTH];
        uint8_t line[BLOCK_SIZE];
    };
} block;

/**
 * Вспомогательный блок для использования в dct.h
 *
 * В преобразования числа могу быть больше чем 256
 */
typedef struct {
    union {
        int matrix[BLOCK_WIDTH][BLOCK_WIDTH];
        int line[BLOCK_SIZE];
    };
} int_block;

block* block_init();

void block_destroy(block* bl);

void block_show(block* bl);

int_block* block_int_init();

void block_int_destroy(int_block* bl);

void block_int_show(int_block* bl);

void block_run_dct(block* bl, int_block *ibl);

void block_run_idct(int_block *ibl, block* bl);

