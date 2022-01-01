#pragma once

#include <stdint.h>
#include <stdio.h>

#include "consts.h"

typedef struct {
  uint8_t y[FRAME_SIZE];
  uint8_t u[FRAME_SIZE / 4];
  uint8_t v[FRAME_SIZE / 4];
} Frame;

void readFrame(FILE *fp, Frame *frame);
void quatizeBlock(int *block);
void writter_coder();
