#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "data_frame.h"

#define FILE_MANAGER_MODE_READ_BINARY ("rb")
#define FILE_MANAGER_MODE_WRITE_BINARY ("wb")

// Открывает бинарный видеофайл .yuv на чтение или запись
FILE* file_manager_open_file(const char* filename, const char* mode);

void file_manager_close_file(FILE* file);

uint8_t file_manager_read_frame(FILE* file, data_frame* frame);

void file_manager_write_frame(FILE* file, data_frame* frame);

