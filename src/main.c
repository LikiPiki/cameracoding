#include <stdint.h>
#include <stdio.h>

#include "dct.h"
#include "reader.h"
#include "writter.h"

int main() {
  /*
   * Чтение бинарного видеофайла -- video.yuv
   * Bosphorus 1080ю 8bit, YUV, RAW
   * Кодирование, квантование и запись в data файл
   */
  writter_coder();

  // Пример работы с блоком
  int block[16 * 16];
  int iblock[16 * 16];
  int i;

  printf("The simple block DCT, IDCT example.\n");
  for (i = 0; i < 16 * 16; i++) {
    block[i] = i;
  }

  show(block);

  DCT_16x16(block);

  IDCT_16x16(block, iblock);

  show(iblock);

  return 0;
}
