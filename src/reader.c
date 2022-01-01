#include <stdlib.h>

#include "reader.h"

void read_data_file(FILE *fp, data_frame *frame) {
  int test = fread(frame, sizeof(int), FRAME_SIZE + FRAME_SIZE / 2, fp);

  printf("test is %d %d\n\n", test, FRAME_SIZE + FRAME_SIZE / 2);

  if (test != FRAME_SIZE + FRAME_SIZE / 2) {
    if (feof(fp)) {
      printf("Premature end of file (read_data_file).\n");
    } else
      printf("File read error.\n");
  }
}

