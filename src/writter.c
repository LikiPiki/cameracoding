#include <stdio.h>

#include "writter.h"
#include "dct.h"

#define INPUT_FILE "../files/video1.yuv"
#define OUTPUT_FILE "../files/data"

/*
 * Чтение одного кадра
 */
void readFrame(FILE *fp, Frame *frame) {
	if (fread(frame, sizeof(uint8_t), FRAME_SIZE + FRAME_SIZE / 2, fp) != FRAME_SIZE + FRAME_SIZE / 2) {
		if (feof(fp)) {
			printf("Premature end of file.");
		} else printf("File read error.");
	}
}

/*
 * Квантование блоков 16x16
 */
void quatizeBlock(int *block) {
	for (int i = 0; i < 16*16; i++) {
		block[i] /= QUANTIZATION_LEVEL;
	}
}

void writter_coder() {
	FILE *fp;
	fp = fopen(INPUT_FILE, "rb");

	if (fp == NULL) {
	  perror("Error while opening the file video1.yuv.\n");
	  return;
	}

	Frame frame;
	readFrame(fp, &frame);

	fclose(fp);

	FILE *out;
	out = fopen(OUTPUT_FILE, "wb");

	if (out == NULL) {
	  perror("Error while opening the file data.\n");
	  return;
	}

	int block[16*16];
	int iblock[16*16];

	int k = 0;
	int fl = 0;

	for (int j = 0; j < 1080 / 16; j++) {
		for (int i = 0; i < 1920 / 16; i++) {
			int start = i * 16 + j * 16 * 1920;

			for (int k = 0; k < (16 * 16); k++) {
				block[k] = frame.y[start + (k % 16) + 1920 * (k / 16)];
			}

			// Кодируем блок и пишем его в файл data
			DCT_16x16(block);
			fwrite(block, sizeof(int), 16*16, out);
		}
	}

	// Дозаполняем оставшиеся блоки кодируем их и пишем в файл
	int startLast = 1920 * (1080 - 8);

	for (int i = 0; i < 1080 / 16; i++) {
		int start = startLast + i * 16;

		// копируем половину блока
		int k;
		for (k = 0; k < (16 * 8); k++) {
			block[k] = frame.y[start + (k % 16) + 1920 * (k / 16)];
		}

		int startI = k - 16;
		// дублируем последнюю строку блока пока не заполним блок
		for (int i = 0; i < (16 * 8); i++) {
			block[k++] = block[startI + i % 16];
		}

		DCT_16x16(block);
		fwrite(block, sizeof(int), 16*16, out);
	}


	// Записываем цветовые компоненты видео в файл
	fwrite(frame.u, sizeof(int), FRAME_SIZE / 4, out);
	fwrite(frame.v, sizeof(int), FRAME_SIZE / 4, out);

	fclose(out);

	printf("\n--------------\n");
}