#include <stdlib.h>

#include "data_frame.h"
#include "logger.h"

data_frame* data_frame_init() {
    data_frame* frame = (data_frame*)malloc(sizeof(data_frame));

    if (frame == NULL) {
        logger_log_and_exit("Cannot allocate memory for data_frame");
    }

    return frame;
}

void data_frame_destroy(data_frame* frame) {
    if (frame == NULL) {
        logger_log("Cannot destroy empty data_frame");
        return;
    }

    free(frame);
}
