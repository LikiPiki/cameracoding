#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "logger.h"

// Логирование программных сообщений
void logger_log(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char log_format[LOGGER_MESSAGE_SIZE];
    snprintf(log_format, LOGGER_MESSAGE_SIZE, "[LOG] %s\n", format);
    vprintf(log_format, args);

    va_end(args);
}

// Логирование ошибок с последующим выходом
void logger_log_and_exit(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char log_format[LOGGER_MESSAGE_SIZE];
    snprintf(log_format, LOGGER_MESSAGE_SIZE, "[LOG] %s: %s\n", strerror(errno), format);
    vprintf(log_format, args);

    va_end(args);
    exit(errno);
}

// Логирование степени сжатия кадра
void logger_log_compressed_frame(size_t frame_number, size_t frame_size, size_t compressed_size) {
    double percent = 100.F - compressed_size / ((double) frame_size) * 100.F;
    logger_log("[%3ld] %8ld -> %7ld (%3.2f%%)", frame_number, frame_size, compressed_size, percent);
}

