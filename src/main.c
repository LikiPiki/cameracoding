#include <stdio.h>

#include "file_manager.h"
#include "data_frame.h"


#define MAIN_INPUT_VIDEO_FILENAME ("../files/bosphorus.yuv")
#define MAIN_OUTPUT_VIDEO_FILENAME ("../files/out.yuv")

int main(int argc, char *argv[]) {
    printf("Hello cameracoding\n");

    FILE* file_in = file_manager_open_file(MAIN_INPUT_VIDEO_FILENAME, FILE_MANAGER_MODE_READ_BINARY);
    FILE* file_out = file_manager_open_file(MAIN_OUTPUT_VIDEO_FILENAME, FILE_MANAGER_MODE_WRITE_BINARY);

    data_frame* frame = data_frame_init();

    file_manager_read_frame(file_in, frame);

    file_manager_write_frame(file_out, frame);

    data_frame_destroy(frame);

    file_manager_close_file(file_in);
    file_manager_close_file(file_out);


    return 0;
}

