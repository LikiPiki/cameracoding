#include <stdlib.h>

#include "data_frame.h"
#include "logger.h"

typedef struct data_frame_filler_params_s {
    data_frame* frame;
    blocks* bls;
    size_t i;
    size_t j;
    size_t k;
    size_t l;
    uint8_t* value;
    uint8_t* out_block_value;
} data_frame_filler_params;

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

/**
 * Разбиение кадра на блоки
 */
void data_frame_copy_component_to_block(
    data_frame* frame,
    blocks* bls,
    size_t block_height,
    size_t block_width,
    size_t data_frame_height,
    uint8_t* (*filler)(data_frame_filler_params params)
) {
    uint8_t value, out_block_value;
    for (size_t j = 0; j < block_height; j++) {
        for (size_t l = 0; l < BLOCK_WIDTH; l++) {
            for (size_t i = 0; i < block_width; i++) {
                for (size_t k = 0; k < BLOCK_WIDTH; k++) {
                    data_frame_filler_params params = {frame, bls, i, j, k, l, &value, &out_block_value};
                    uint8_t* current = filler(params);
                    if (j * BLOCK_WIDTH + l >= data_frame_height) {
                        *current = out_block_value;
                    } else {
                        *current = value;
                    }
                }
            }
        }
    }
}

uint8_t* data_frame_fill_y_component_to_block(data_frame_filler_params params) {
    *params.out_block_value = params.frame->y.matrix[DATA_FRAME_Y_HEIGHT - 1][params.i * BLOCK_WIDTH + params.k];
    *params.value = params.frame->y.matrix[params.j * BLOCK_WIDTH + params.l][params.i * BLOCK_WIDTH + params.k];
    return &params.bls->y.matrix[params.j][params.i].matrix[params.l][params.k];
}

uint8_t* data_frame_fill_u_component_to_block(data_frame_filler_params params) {
    *params.out_block_value = params.frame->u.matrix[DATA_FRAME_U_HEIGHT - 1][params.i * BLOCK_WIDTH + params.k];
    *params.value = params.frame->u.matrix[params.j * BLOCK_WIDTH + params.l][params.i * BLOCK_WIDTH + params.k];
    return &params.bls->u.matrix[params.j][params.i].matrix[params.l][params.k];
}

uint8_t* data_frame_fill_v_component_to_block(data_frame_filler_params params) {
    *params.out_block_value = params.frame->v.matrix[DATA_FRAME_V_HEIGHT - 1][params.i * BLOCK_WIDTH + params.k];
    *params.value = params.frame->v.matrix[params.j * BLOCK_WIDTH + params.l][params.i * BLOCK_WIDTH + params.k];
    return &params.bls->v.matrix[params.j][params.i].matrix[params.l][params.k];
}

void data_frame_to_blocks(data_frame* frame, blocks* bls) {
    // Копируем Y компоненты по блокам
    data_frame_copy_component_to_block(
        frame,
        bls,
        BLOCKS_Y_HEIGHT,
        BLOCKS_Y_WIDTH,
        DATA_FRAME_Y_HEIGHT,
        data_frame_fill_y_component_to_block
    );

    // Копируем U компоненты по блокам
    data_frame_copy_component_to_block(
        frame,
        bls,
        BLOCKS_U_HEIGHT,
        BLOCKS_U_WIDTH,
        DATA_FRAME_U_HEIGHT,
        data_frame_fill_u_component_to_block
    );

    // Копируем V компоненты по блокам
    data_frame_copy_component_to_block(
        frame,
        bls,
        BLOCKS_V_HEIGHT,
        BLOCKS_V_WIDTH,
        DATA_FRAME_V_HEIGHT,
        data_frame_fill_v_component_to_block
    );
}


void data_frame_copy_component_from_block(
    data_frame* frame,
    blocks* bls,
    size_t block_height,
    size_t block_width,
    size_t data_frame_height,
    void (*filler)(data_frame_filler_params params)
) {
    for (size_t j = 0; j < block_height; j++) {
        for (size_t l = 0; l < BLOCK_WIDTH; l++) {
            for (size_t i = 0; i < block_width; i++) {
                for (size_t k = 0; k < BLOCK_WIDTH; k++) {
                    if (j * BLOCK_WIDTH + l >= data_frame_height) {
                        return;
                    }
                    data_frame_filler_params params = {frame, bls, i, j, k, l, NULL, NULL};

                    filler(params);
                }
            }
        }
    }
}

void data_frame_fill_y_component_from_block(data_frame_filler_params params) {
    uint8_t result = params.bls->y.matrix[params.j][params.i].matrix[params.l][params.k];
    params.frame->y.matrix[params.j * BLOCK_WIDTH + params.l][params.i * BLOCK_WIDTH + params.k] = result;
}

void data_frame_fill_u_component_from_block(data_frame_filler_params params) {
    uint8_t result = params.bls->u.matrix[params.j][params.i].matrix[params.l][params.k];
    params.frame->u.matrix[params.j * BLOCK_WIDTH + params.l][params.i * BLOCK_WIDTH + params.k] = result;
}

void data_frame_fill_v_component_from_block(data_frame_filler_params params) {
    uint8_t result = params.bls->v.matrix[params.j][params.i].matrix[params.l][params.k];
    params.frame->v.matrix[params.j * BLOCK_WIDTH + params.l][params.i * BLOCK_WIDTH + params.k] = result;
}

/**
 * Обратное преобразование блоков в кадр
 */
void data_frame_from_blocks(blocks* bls, data_frame* frame) {
    // Копируем Y компоненты по блокам
    data_frame_copy_component_from_block(
        frame,
        bls,
        BLOCKS_Y_HEIGHT,
        BLOCKS_Y_WIDTH,
        DATA_FRAME_Y_HEIGHT,
        data_frame_fill_y_component_from_block
    );

    // Копируем U компоненты по блокам
    data_frame_copy_component_from_block(
        frame,
        bls,
        BLOCKS_U_HEIGHT,
        BLOCKS_U_WIDTH,
        DATA_FRAME_U_HEIGHT,
        data_frame_fill_u_component_from_block
    );

    // Копируем V компоненты по блокам
    data_frame_copy_component_from_block(
        frame,
        bls,
        BLOCKS_V_HEIGHT,
        BLOCKS_V_WIDTH,
        DATA_FRAME_V_HEIGHT,
        data_frame_fill_v_component_from_block
    );

}

