#pragma once

#include <stdint.h>
#include <stddef.h>

#define BLOCK_WIDTH (16)
#define BLOCK_SIZE (BLOCK_WIDTH * BLOCK_WIDTH)

// Y блоки
// 120 блоков влезает в ширину кадра
#define BLOCKS_Y_WIDTH (120)
// 67 полных блоков влезает по высоте в кадр, добавляем еще один, чтобы покрыть все пиксели
#define BLOCKS_Y_HEIGHT (68)
#define BLOCKS_Y_SIZE (BLOCKS_Y_WIDTH * BLOCKS_Y_HEIGHT)

// U блоки (1920 / 2 / 16) блоков в строке
// по высоте (1080 / 2 / 16) = 33 + 1 блок
#define BLOCKS_U_WIDTH (60)
#define BLOCKS_U_HEIGHT (34)
#define BLOCKS_U_SIZE (BLOCKS_U_WIDTH * BLOCKS_U_HEIGHT)

// V блоки
#define BLOCKS_V_WIDTH (60)
#define BLOCKS_V_HEIGHT (34)
#define BLOCKS_V_SIZE (BLOCKS_V_WIDTH * BLOCKS_V_HEIGHT)

// Общее количество всех блоков
#define BLOCKS_YUV_SIZE (BLOCKS_Y_SIZE + BLOCKS_U_SIZE + BLOCKS_V_SIZE)

/**
 * Основной блок кадра 16x16 пикселей
 */
typedef struct {
    union {
        uint8_t matrix[BLOCK_WIDTH][BLOCK_WIDTH];
        uint8_t line[BLOCK_SIZE];
    };
} block;

block* block_init();

void block_destroy(block* bl);

void block_show(block* bl);

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

int_block* block_int_init();

void block_int_copy(int_block* src, int_block* dest);

void block_int_destroy(int_block* bl);

void block_int_show(int_block* bl);

void block_run_dct(block* bl, int_block* ibl);

void block_run_idct(int_block *ibl, block* bl);

/**
 * Кадр разделенный на блоки
 */
typedef struct {
    union {
        struct {
            union {
                block matrix[BLOCKS_Y_HEIGHT][BLOCKS_Y_WIDTH];
                block line[BLOCKS_Y_SIZE];
            } y;
            union {
                block matrix[BLOCKS_U_HEIGHT][BLOCKS_U_WIDTH];
                block line[BLOCKS_U_SIZE];
            } u;
            union {
                block matrix[BLOCKS_V_HEIGHT][BLOCKS_V_WIDTH];
                block line[BLOCKS_V_SIZE];
            } v;
        };
        block line[BLOCKS_YUV_SIZE];
    };
} blocks;

blocks* blocks_init();

void blocks_destroy(blocks* bls);

/**
 * Кадр разделенный на интовые блоки
 *
 * Для того чтобы использовать преобразования 
 */
typedef struct {
    union {
        struct {
            union {
                int_block matrix[BLOCKS_Y_HEIGHT][BLOCKS_Y_WIDTH];
                int_block line[BLOCKS_Y_SIZE];
            } y;
            union {
                int_block matrix[BLOCKS_U_HEIGHT][BLOCKS_U_WIDTH];
                int_block line[BLOCKS_U_SIZE];
            } u;
            union {
                int_block matrix[BLOCKS_V_HEIGHT][BLOCKS_V_WIDTH];
                int_block line[BLOCKS_V_SIZE];
            } v;
        };
        int_block line[BLOCKS_YUV_SIZE];
    };
} int_blocks;

int_blocks* blocks_int_init();

void blocks_int_destroy(int_blocks* ibls);

void blocks_run_dct(blocks* bl, int_blocks* ibl);

void blocks_run_idct(int_blocks *ibl, blocks* bl);


