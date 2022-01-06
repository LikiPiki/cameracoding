#include "core.h"

#include "zigzag.h"
#include "quantize.h"
#include "logger.h"

void core_process_encode(data_frame* frame, run_level_int_blocks* rlibls) {
    blocks* bls = blocks_init();
    int_blocks* ibls = blocks_int_init();

    data_frame_to_blocks(frame, bls);

    // Main coder functions
    blocks_run_dct(bls, ibls);
    quantize_int_blocks(ibls, QUANTIZE_DEFAULT_LEVEL);
    zigzag_int_blocks(ibls);
    run_level_run_blocks(ibls, rlibls);

    blocks_destroy(bls);
    blocks_int_destroy(ibls);
}

void core_process_decode(run_level_int_blocks* rlibls, data_frame* frame) {
    blocks* bls = blocks_init();
    int_blocks* ibls = blocks_int_init();

    // Main decoder functions
    run_level_inverse_run_blocks(rlibls, ibls);
    zigzag_inverse_int_blocks(ibls);
    quantize_inverse_int_blocks(ibls, QUANTIZE_DEFAULT_LEVEL);
    blocks_run_idct(ibls, bls);

    data_frame_from_blocks(bls, frame);

    blocks_destroy(bls);
    blocks_int_destroy(ibls);
}

