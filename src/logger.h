#pragma once

// Длина сообщения которое выводится логгером
#define LOGGER_MESSAGE_SIZE (128)

// Логирование ошибок
void logger_log(const char* message);

// Логирование ошибки и выход
void logger_log_and_exit(const char* message);
