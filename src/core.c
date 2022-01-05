#include "core.h"

#include "zigzag.h"
#include "quantize.h"

void core_process_frame(data_frame* frame) {
    blocks* bls = blocks_init();
    int_blocks* ibls = blocks_int_init();

    data_frame_to_blocks(frame, bls);

    // Primary
    blocks_run_dct(bls, ibls);
    quantize_int_blocks(ibls, QUANTIZE_DEFAULT_LEVEL);
    zigzag_int_blocks(ibls);

    // Inverse
    zigzag_inverse_int_blocks(ibls);
    quantize_inverse_int_blocks(ibls, QUANTIZE_DEFAULT_LEVEL);
    blocks_run_idct(ibls, bls);

    data_frame_from_blocks(bls, frame);

    blocks_destroy(bls);
    blocks_int_destroy(ibls);
}
