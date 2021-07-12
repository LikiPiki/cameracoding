#pragma once

#include <stdio.h>
#include <stdint.h>

#include "consts.h"

/*
 * Структура одного видеокадра
 */
typedef struct {
	uint8_t y[FRAME_SIZE];
	uint8_t u[FRAME_SIZE / 4];
	uint8_t v[FRAME_SIZE / 4];
} Frame;

void readFrame(FILE *fp, Frame *frame);
void quatizeBlock(int *block);
void readFromFile();
