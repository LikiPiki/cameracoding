#include <stdio.h>

#include "file_manager.h"
#include "data_frame.h"
#include "block.h"
#include "quantize.h"
#include "zigzag.h"

#define MAIN_INPUT_VIDEO_FILENAME ("../files/bosphorus.yuv")
#define MAIN_OUTPUT_VIDEO_FILENAME ("../files/out.yuv")

int main(int argc, char *argv[]) {
    FILE* file_in = file_manager_open_file(MAIN_INPUT_VIDEO_FILENAME, FILE_MANAGER_MODE_READ_BINARY);
    FILE* file_out = file_manager_open_file(MAIN_OUTPUT_VIDEO_FILENAME, FILE_MANAGER_MODE_WRITE_BINARY);

    data_frame* frame = data_frame_init();

    /*
     * Чтение бинарного видеофайла -- http://ultravideo.fi/#testsequences
     * Bosphorus 1080ю 8bit, YUV, RAW
     * Кодирование, квантование и запись в data файл
     */
    file_manager_read_frame(file_in, frame);

    blocks* bls = blocks_init();
    int_blocks* ibls = blocks_int_init();
    data_frame_to_blocks(frame, bls);

    blocks_run_dct(bls, ibls);

    quantize_int_blocks(ibls, QUANTIZE_DEFAULT_LEVEL);

    zigzag_int_blocks(ibls);

    zigzag_inverse_int_blocks(ibls);

    quantize_inverse_int_blocks(ibls, QUANTIZE_DEFAULT_LEVEL);

    blocks_run_idct(ibls, bls);

    data_frame_from_blocks(bls, frame);

    blocks_destroy(bls);
    blocks_int_destroy(ibls);

    file_manager_write_frame(file_out, frame);
    data_frame_destroy(frame);

    file_manager_close_file(file_in);
    file_manager_close_file(file_out);

    return 0;
}

