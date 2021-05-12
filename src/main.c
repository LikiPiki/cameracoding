#include <stdio.h>
#include <stdint.h>

// simple DCT transformations
#include "sampleDCT.h"

// print inline int16_t * matrix
void printLinearMatrix(int16_t *matrix, int len) {
    for (int i = 0; i < len; i++) {
        printf("%3.d ", matrix[i]);
    }
    printf("\n");
}

// code from https://github.com/videolan/x265/blob/master/source/common/dct.cpp
// Fast DST Algorithm. Full matrix multiplication for DST and Fast DST algorithm
// give identical results
void fastForwardDst(const int16_t* block, int16_t* coeff, int shift)  // input block, output coeff
{
    int c[4];
    int rnd_factor = 1 << (shift - 1);

    for (int i = 0; i < 4; i++)
    {
        // Intermediate Variables
        c[0] = block[4 * i + 0] + block[4 * i + 3];
        c[1] = block[4 * i + 1] + block[4 * i + 3];
        c[2] = block[4 * i + 0] - block[4 * i + 1];
        c[3] = 74 * block[4 * i + 2];

        coeff[i] =      (int16_t)((29 * c[0] + 55 * c[1]  + c[3] + rnd_factor) >> shift);
        coeff[4 + i] =  (int16_t)((74 * (block[4 * i + 0] + block[4 * i + 1] - block[4 * i + 3]) + rnd_factor) >> shift);
        coeff[8 + i] =  (int16_t)((29 * c[2] + 55 * c[0]  - c[3] + rnd_factor) >> shift);
        coeff[12 + i] = (int16_t)((55 * c[2] - 29 * c[1] + c[3] + rnd_factor) >> shift);
    }
}

int16_t x265_min(int16_t a, int16_t b) { return a < b ? a : b; }
int16_t x265_max(int16_t a, int16_t b) { return a > b ? a : b; }
int16_t x265_clip3(int minVal, int maxVal, int a) {
    return x265_min(
            x265_max(minVal, a),
            maxVal);
}

void inversedst(const int16_t* tmp, int16_t* block, int shift)  // input tmp, output block
{
    int i, c[4];
    int rnd_factor = 1 << (shift - 1);

    for (i = 0; i < 4; i++)
    {
        // Intermediate Variables
        c[0] = tmp[i] + tmp[8 + i];
        c[1] = tmp[8 + i] + tmp[12 + i];
        c[2] = tmp[i] - tmp[12 + i];
        c[3] = 74 * tmp[4 + i];

        block[4 * i + 0] = (int16_t)x265_clip3(-32768, 32767, (29 * c[0] + 55 * c[1]     + c[3]               + rnd_factor) >> shift);
        block[4 * i + 1] = (int16_t)x265_clip3(-32768, 32767, (55 * c[2] - 29 * c[1]     + c[3]               + rnd_factor) >> shift);
        block[4 * i + 2] = (int16_t)x265_clip3(-32768, 32767, (74 * (tmp[i] - tmp[8 + i]  + tmp[12 + i])      + rnd_factor) >> shift);
        block[4 * i + 3] = (int16_t)x265_clip3(-32768, 32767, (55 * c[0] + 29 * c[2]     - c[3]               + rnd_factor) >> shift);
    }
}

void testFDCT() {
    int16_t coeffs[16] = {0};
    int16_t testMatrix[16] = {
        -76, -73, -67, -62, -58, -67, -64, -55, -76, -73, -67, -62, -58, -67, -64, -55
    };
    int16_t result[16] = {0};

    printLinearMatrix(testMatrix, 16);
    printLinearMatrix(coeffs, 16);

    fastForwardDst(testMatrix, coeffs, 1);
    printLinearMatrix(coeffs, 16);

    inversedst(coeffs, result, 1);
    printLinearMatrix(result, 16);
}

int main(int argc, char *argv[]) {
    testFDCT();

    printf("\n---------------\n");

    testDCT();
    
    return 0;
}
