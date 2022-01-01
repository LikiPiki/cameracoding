#include <stdio.h>
#include <stdlib.h>

#include "dct.h"
#include "writter.h"

/*
 * Чтение одного кадра
 */
void readFrame(FILE *fp, Frame *frame) {
  if (fread(frame, sizeof(uint8_t), FRAME_SIZE + FRAME_SIZE / 2, fp) !=
      FRAME_SIZE + FRAME_SIZE / 2) {
    if (feof(fp)) {
      printf("Premature end of file (read_file).\n");
    } else
      printf("File read error.\n");
  }
}

/*
 * Квантование блоков 16x16
 */
void quatizeBlock(int *block) {
  for (int i = 0; i < 16 * 16; i++) {
    block[i] /= QUANTIZATION_LEVEL;
  }
}
/*
 * Деквантование блоков 16x16
 */
void dequatizeBlock(int *block) {
  for (int i = 0; i < 16 * 16; i++) {
    block[i] *= QUANTIZATION_LEVEL;
  }
}

void copy_iblock_to_line(int *iblock, int blockNum, uint8_t *line) {
  int offset = blockNum * 16;
  for (int i = 0; i < 16 * 16; i++) {
    line[offset + i % 16 + i / 16 * 1920] = (uint8_t)iblock[i];
  }
}

void copy_half_iblock_to_line(int *iblock, int blockNum, uint8_t *line) {
  int offset = blockNum * 16;
  for (int i = 0; i < 16 * 8; i++) {
    line[offset + i % 16 + i / 16 * 1920] = (uint8_t)iblock[i];
  }
}

void writter_coder() {
  FILE *fp;
  fp = fopen(INPUT_FILE, "rb");

  if (fp == NULL) {
    perror("Error while opening the file video1.yuv.\n");
    return;
  }

  Frame *frame = (Frame *)malloc(sizeof(Frame));
  readFrame(fp, frame);

  fclose(fp);

  FILE *out;
  out = fopen(DATA_FILE, "wb");

  if (out == NULL) {
    perror("Error while opening the file data.\n");
    return;
  }

  int block[16 * 16];
  int iblock[16 * 16];

  int k = 0;
  int fl = 0;

  FILE *test = fopen("../files/test.yuv", "wb");
  // Одна линия из 120 блоков
  uint8_t *line = (uint8_t *)malloc(sizeof(uint8_t) * 16 * VIDEO_WIDTH);

  for (int j = 0; j < 1080 / 16; j++) {
    for (int i = 0; i < 1920 / 16; i++) {
      int start = i * 16 + j * 16 * 1920;

      for (int k = 0; k < (16 * 16); k++) {
        block[k] = frame->y[start + (k % 16) + 1920 * (k / 16)];
      }

      // Кодируем блок и пишем его в файл data
      DCT_16x16(block);
      quatizeBlock(block);

      dequatizeBlock(block);
      IDCT_16x16(block, iblock);

      copy_iblock_to_line(iblock, i, line);
      fwrite(block, sizeof(int), 16 * 16, out);
    }

    fwrite(line, sizeof(uint8_t), 16 * VIDEO_WIDTH, test);
  }

  // Дозаполняем оставшиеся блоки кодируем их и пишем в файл
  int startLast = 1920 * (1080 - 8);

  for (int i = 0; i < 1920 / 16; i++) {
    int start = startLast + i * 16;

    // копируем половину блока
    int k;
    for (k = 0; k < (16 * 8); k++) {
      block[k] = frame->y[start + (k % 16) + 1920 * (k / 16)];
    }

    int startI = k - 16;
    // дублируем последнюю строку блока пока не заполним блок
    for (int i = 0; i < (16 * 8); i++) {
      block[k++] = block[startI + i % 16];
    }

    DCT_16x16(block);

    quatizeBlock(block);

    IDCT_16x16(block, iblock);

    copy_iblock_to_line(iblock, i, line);

    fwrite(block, sizeof(int), 16 * 16, out);
  }

  fwrite(line, sizeof(uint8_t), 16 * VIDEO_WIDTH / 2, test);

  // Записываем цветовые компоненты видео в файл
  fwrite(frame->u, sizeof(int), FRAME_SIZE / 4, out);
  fwrite(frame->v, sizeof(int), FRAME_SIZE / 4, out);

  fclose(out);

  // Записываем цветовые составляещие в тестовый файл
  fwrite(frame->u, sizeof(int), FRAME_SIZE / 4, test);
  fwrite(frame->v, sizeof(int), FRAME_SIZE / 4, test);
  fclose(test);

  // Сохраняем кадр как отдельное видео для теста
  FILE *file = fopen("../files/video.yuv", "wb");
  int written =
      fwrite(frame, sizeof(uint8_t), FRAME_SIZE + FRAME_SIZE / 2, file);
  printf("Written %d\n\n", written);
  fclose(file);

  free(line);
  free(frame);
  printf("\n--------------\n");
}
