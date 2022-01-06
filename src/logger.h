#pragma once

#include <stddef.h>

// Длина сообщения которое выводится логгером
#define LOGGER_MESSAGE_SIZE (128)

// Логирование ошибок
void logger_log(const char* format, ...);

// Логирование ошибки и выход
void logger_log_and_exit(const char* format, ...);

// Логирование сжатия кадра
void logger_log_compressed_frame(size_t frame_number, size_t frame_size, size_t compressed_size);

