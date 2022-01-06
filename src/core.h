#pragma once

#include "data_frame.h"
#include "run_level.h"


void core_process_encode(data_frame* frame, run_level_int_blocks* rlibls);

void core_process_decode(run_level_int_blocks* rlibls, data_frame* frame);
