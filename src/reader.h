#pragma once

#include <stdio.h>

#include "consts.h"

/*
 * Структура одного видеокадра файла data
 * Блоки в нем лежат по порядку блок за блоком
 */
typedef struct {
	int y[FRAME_SIZE];
	int u[FRAME_SIZE / 4];
	int v[FRAME_SIZE / 4];
} data_frame;

void reader_decode();
void read_data_file(FILE *fp, data_frame *frame);