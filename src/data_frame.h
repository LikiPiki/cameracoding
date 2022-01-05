#pragma once

#include <stdint.h>
#include <stddef.h>

#include "block.h"

#define DATA_FRAME_Y_WIDTH (1920)
#define DATA_FRAME_Y_HEIGHT (1080)
#define DATA_FRAME_Y_SIZE (DATA_FRAME_Y_WIDTH * DATA_FRAME_Y_HEIGHT)

#define DATA_FRAME_U_WIDTH (DATA_FRAME_Y_WIDTH / 2)
#define DATA_FRAME_U_HEIGHT (DATA_FRAME_Y_HEIGHT / 2)
#define DATA_FRAME_U_SIZE (DATA_FRAME_U_WIDTH * DATA_FRAME_U_HEIGHT)

#define DATA_FRAME_V_WIDTH (DATA_FRAME_Y_WIDTH / 2)
#define DATA_FRAME_V_HEIGHT (DATA_FRAME_Y_HEIGHT / 2)
#define DATA_FRAME_V_SIZE (DATA_FRAME_V_WIDTH * DATA_FRAME_V_HEIGHT)


/*
 * Структура одного видеокадра файла data
 * Блоки в нем лежат по порядку блок за блоком
 *
 * В файле лежит сначала Y компонента
 * Затем четверть кадра U компонент
 * Затем четверть кадра V компонент
 */
typedef struct {
    union {
        uint8_t matrix[DATA_FRAME_Y_HEIGHT][DATA_FRAME_Y_WIDTH];
        uint8_t line[DATA_FRAME_Y_SIZE];
    } y;
    union {
        uint8_t matrix[DATA_FRAME_U_HEIGHT][DATA_FRAME_U_WIDTH];
        uint8_t line[DATA_FRAME_U_SIZE];
    } u;
    union {
        uint8_t matrix[DATA_FRAME_V_HEIGHT][DATA_FRAME_V_WIDTH];
        uint8_t line[DATA_FRAME_V_SIZE];
    } v;
} data_frame;

data_frame* data_frame_init();

void data_frame_destroy(data_frame* frame);

/**
 * Разбиение одного кадра на блоки
 *
 * В основном будут целые блоки, нижние блоки заполняются не полностью
 * Дублируем последнюю строку блока пока не заполним блок
 */
void data_frame_to_blocks(data_frame* frame, blocks* bls);

void data_frame_from_blocks(blocks* bls, data_frame* frame);
