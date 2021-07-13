#include <reader.h>
#include <stdlib.h>

void read_data_file(FILE *fp, data_frame *frame) {
	if (fread(frame, sizeof(int), FRAME_SIZE + FRAME_SIZE / 2, fp) != FRAME_SIZE + FRAME_SIZE / 2) {
		if (feof(fp)) {
			printf("Premature end of file.");
		} else printf("File read error.");
	}
}

void reader_decode() {
	data_frame *frame = (data_frame *) malloc(sizeof(data_frame));
	FILE *fp;
	fp = fopen(OUTPUT_FILE, "rb");

	if (fp == NULL) {
	  perror("Error while opening the file video1.yuv.\n");
	  return;
	}

	read_data_file(fp, frame);

	fclose(fp);
	free(frame);
}