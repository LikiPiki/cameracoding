#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "data_frame.h"
#include "run_level.h"

#define FILE_MANAGER_MODE_READ_BINARY ("rb")
#define FILE_MANAGER_MODE_WRITE_BINARY ("wb")

#define FILE_MANAGER_BUFFER_DEFAULT_SIZE (128ULL * 1024ULL)

typedef struct {
    int* buffer;
    size_t size;
} compressed_buffer;

compressed_buffer* file_manager_compressed_buffer_init(size_t size);

void file_manager_compressed_buffer_destroy(compressed_buffer* cb);

// Открывает бинарный видеофайл .yuv на чтение или запись
FILE* file_manager_open_file(const char* filename, const char* mode);

void file_manager_close_file(FILE* file);

uint8_t file_manager_read_frame(FILE* file, data_frame* frame);

void file_manager_write_frame(FILE* file, data_frame* frame);

void file_manager_write_compressed(FILE* file, run_level_int_blocks* rlibls);

uint8_t file_manager_read_compressed(FILE* file, run_level_int_blocks* rlibls, compressed_buffer* buffer);
