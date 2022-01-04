#pragma once

#include <stdint.h>
#include <stddef.h>

#define DATA_FRAME_WIDTH (1920)
#define DATA_FRAME_HEIGHT (1080)
#define DATA_FRAME_SIZE (DATA_FRAME_WIDTH * DATA_FRAME_HEIGHT)

/*
 * Структура одного видеокадра файла data
 * Блоки в нем лежат по порядку блок за блоком
 *
 * В файле лежит сначала Y компонента
 * Затем четверть кадра U компонент
 * Затем четверть кадра V компонент
 */
typedef struct {
    uint8_t y[DATA_FRAME_SIZE];
    uint8_t u[DATA_FRAME_SIZE / 4];
    uint8_t v[DATA_FRAME_SIZE / 4];
} data_frame;

data_frame* data_frame_init();

void data_frame_destroy(data_frame* frame);

