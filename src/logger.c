#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "logger.h"

// Логирование ошибок
void logger_log(const char* message) {
    printf("[LOG] %s\n", message);
}

void logger_log_and_exit(const char* message) {
    printf("[ERR] %s: %s\n", strerror(errno), message);
    exit(errno);
}
