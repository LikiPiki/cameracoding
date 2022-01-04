#include "file_manager.h"

#include "logger.h"

FILE* file_manager_open_file(const char* filename, const char* mode) {
    FILE *file = fopen(filename, mode);

    if (file == NULL) {
        char error_message[LOGGER_MESSAGE_SIZE];
        snprintf(error_message, LOGGER_MESSAGE_SIZE, "Cannot open %s file", filename);

        logger_log_and_exit(error_message);
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

void file_manager_read_frame(FILE* file, data_frame* frame) {
    if (fread(frame, sizeof(data_frame), 1, file) != 1) {
        if (feof(file)) {
            logger_log("Premature end of file while reading frame");
        } else {
            logger_log_and_exit("File read frame error");
        }
    }
}

void file_manager_write_frame(FILE* file, data_frame* frame) {
    if (fwrite(frame, sizeof(data_frame), 1, file) != 1) {
        logger_log_and_exit("Write to output file frame error");
    }
}
