#include <stdio.h>
#include <stdlib.h>

#include "file_manager.h"
#include "data_frame.h"
#include "core.h"
#include "logger.h"

#define MAIN_INPUT_VIDEO_FILENAME ("../files/bosphorus.yuv")
#define MAIN_OUTPUT_VIDEO_FILENAME ("../files/out.yuv")
#define MAIN_COMPRESSED_FILENAME ("../files/compressed")

int main(int argc, char *argv[]) {
    FILE* file_in = file_manager_open_file(MAIN_INPUT_VIDEO_FILENAME, FILE_MANAGER_MODE_READ_BINARY);
    FILE* file_out = file_manager_open_file(MAIN_OUTPUT_VIDEO_FILENAME, FILE_MANAGER_MODE_WRITE_BINARY);
    FILE* file_compressed = file_manager_open_file(MAIN_COMPRESSED_FILENAME, FILE_MANAGER_MODE_WRITE_BINARY);

    data_frame* frame = data_frame_init();
    run_level_int_blocks* rlibls = run_level_int_blocks_init();
    compressed_buffer *buffer = file_manager_compressed_buffer_init(FILE_MANAGER_BUFFER_DEFAULT_SIZE);

    size_t frame_counter = 0;
    logger_log("Start encoding video file");
    while (file_manager_read_frame(file_in, frame)) {
        core_process_encode(frame, rlibls);

        // Логирование сжатия файла
        size_t compressed_frame_size = file_manager_write_compressed(file_compressed, rlibls);
        logger_log_compressed_frame(++frame_counter, sizeof(data_frame), compressed_frame_size);
    }

    file_manager_close_file(file_compressed);
    file_compressed = file_manager_open_file(MAIN_COMPRESSED_FILENAME, FILE_MANAGER_MODE_READ_BINARY);
    logger_log("Start decoding video file");

    frame_counter = 0;
    while (file_manager_read_compressed(file_compressed, rlibls, buffer)) {
        core_process_decode(rlibls, frame);
        file_manager_write_frame(file_out, frame);
        logger_log("[%3ld] frame decoded", ++frame_counter);
    }

    data_frame_destroy(frame);
    run_level_int_blocks_destroy(rlibls);
    file_manager_compressed_buffer_destroy(buffer);

    file_manager_close_file(file_in);
    file_manager_close_file(file_out);
    file_manager_close_file(file_compressed);

    return 0;
}

