#include <stdlib.h>

#include "file_manager.h"
#include "logger.h"

compressed_buffer* file_manager_compressed_buffer_init(size_t size) {
    compressed_buffer* cb = (compressed_buffer*)malloc(sizeof(compressed_buffer));

    if (cb == NULL) {
        logger_log_and_exit("Cannot allocate memory for compressed buffer");
    }

    cb->size = size;
    cb->buffer = (int*)malloc(sizeof(int) * size);

    if (cb->buffer == NULL) {
        logger_log_and_exit("Cannot allocate memory for compressed buffer");
    }

    return cb;
}

void file_manager_compressed_buffer_destroy(compressed_buffer* cb) {
    if (cb == NULL || cb->buffer == NULL) {
        logger_log("cannot deallocate empty compressed buffer");
        return;
    }

    free(cb->buffer);
    free(cb);
}

FILE* file_manager_open_file(const char* filename, const char* mode) {
    FILE *file = fopen(filename, mode);

    if (file == NULL) {
        logger_log_and_exit("Cannot open %s file", filename);
    }

    return file;
}

void file_manager_close_file(FILE* file) {
    if (file == NULL) {
        logger_log("Cannot close empty file");
        return;
    }

    fclose(file);
}

uint8_t file_manager_read_frame(FILE* file, data_frame* frame) {
    if (fread(frame, sizeof(data_frame), 1, file) != 1) {
        if (feof(file)) {
            return 0;
        } else {
            logger_log_and_exit("File read frame error");
        }
    }

    return 1;
}

void file_manager_write_frame(FILE* file, data_frame* frame) {
    if (fwrite(frame, sizeof(data_frame), 1, file) != 1) {
        logger_log_and_exit("Write to output file frame error");
    }
}

size_t file_manager_write_compressed(FILE* file, run_level_int_blocks* rlibls) {
    size_t buffer_size = 0;
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        buffer_size += rlibls->line[i].size;
    }

    int* buffer = (int*)malloc(sizeof(int) * buffer_size);
    size_t pos = 0;
    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++) {
        for (size_t j = 0; j < rlibls->line[i].size; j++) {
            buffer[pos++] = rlibls->line[i].line[j];
        }
    }

    if (fwrite(buffer, sizeof(int), buffer_size, file) != buffer_size) {
        logger_log_and_exit("Write compressed buffer to file error");
    }

    free(buffer);

    return buffer_size * sizeof(int);
}

uint8_t file_manager_read_compressed(FILE* file, run_level_int_blocks* rlibls, compressed_buffer *cb) {
    ssize_t pos = 0;
    ssize_t max = 0;

    ssize_t cur_size = 0;
    ssize_t cur_index = 0;

    for (size_t i = 0; i < BLOCKS_YUV_SIZE; i++)
        rlibls->line[i].size = 0;

    while (cur_index != BLOCKS_YUV_SIZE) {
        pos = 0;
        max = fread(cb->buffer, sizeof(int), cb->size, file);

        if (!max) {
            if (!pos) 
                return 0;
            else 
                logger_log_and_exit("End of file. but run blocks not filled");
        }
        
        while (pos != max && cur_index != BLOCKS_YUV_SIZE) {
            int value = cb->buffer[pos];
            int zero_count = cb->buffer[pos + 1];

            size_t cur_pos = rlibls->line[cur_index].size;
            rlibls->line[cur_index].line[cur_pos] = value;
            rlibls->line[cur_index].line[cur_pos + 1] = zero_count;
            rlibls->line[cur_index].size += 2;

            cur_size += 1 + zero_count;

            if (cur_size == BLOCK_SIZE)
                cur_index++, cur_size = 0;

            pos += 2;
        }
    }

    fseek(file, sizeof(int) * (pos - max), SEEK_CUR);
    return 1;
}

