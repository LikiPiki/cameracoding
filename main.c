#include <stdio.h>
#include <math.h>

#define SIZE 4 
#define LINE_SIZE SIZE * SIZE

#define PI 3.14159265358979323846

// print inline int16_t * matrix
void printLinearMatrix(int16_t *matrix, int len) {
    for (int i = 0; i < len; i++) {
        printf("%3.d ", matrix[i]);
    }
    printf("\n");
}

// вывод матрицы
void printMatrix(int matrix[][SIZE], int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

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
int16_t x265_clip3(int minVal, int maxVal, int a) { return x265_min(x265_max(minVal, a), maxVal); }
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


// CuCv
double alpha(int u) {
    if (u == 0) {
        return 1 / sqrt(2);
    } else {
        return 1;
    }
}

double getDCT(int u, double v, int matrix[][SIZE], int len) {
    double sum = 0;
    for (double x = 0; x < len; x++) {
        for (double y = 0; y < len; y++) {
            sum += matrix[(int) x][(int)y] * cos(((2 * x + 1) * u * PI) / (2 * len)) * cos(((2 * y + 1) * v * PI) / (2 * len));
        }
    }
    return sum;
}

void DCT(int result[][SIZE], int matrix[][SIZE], int len) {
    for (int u = 0; u < len; u++) {
        for (int v = 0; v < len; v++) {
            result[u][v] = 0.25 * alpha(u) * alpha(v) * getDCT((double) u, (double) v, matrix, len);
        }
    }
}

double getIDCT(int DCTMatrix[][SIZE], double x, double y, int len) {
    double sum = 0;
    for (int u = 0; u < len; u++) {
        for (int v = 0; v < len; v++) {
            sum += alpha(u) * alpha(v)  * DCTMatrix[(int) u][(int) v] * cos(((2 * x + 1) * u * PI) / (2 * len)) * cos(((2 * y + 1) * v * PI) / (2 * len));
        }
    }
    return sum;
}

void IDCT(int result[][SIZE], int matrix[][SIZE], int len) {
    for (int x = 0; x < len; x++) {
        for (int y = 0; y < len; y++) {
            result[x][y] = 0.25 * getIDCT(matrix, x, y, len);
        }
    }
}


int main(int argc, char *argv[]) {
    int matrix[SIZE][SIZE] = {
        { -76, -73, -67, -62},
        { -65, -69, -73, -38},
        { -66, -69, -60, -15},
        { -65, -70, -57,  -6},
    };

    const int shift = 0;
    int16_t block[16] = {0};
    int16_t testMatrix[16] = {
        -76, -73, -67, -62, -58, -67, -64, -55, -76, -73, -67, -62, -58, -67, -64, -55
    };
    printf("default matrix:\n");
    printLinearMatrix(testMatrix, LINE_SIZE);
    int16_t coeffs[16] = {0};

    printf("coeffs:\n");
    fastForwardDst((int16_t *) testMatrix, (int16_t *) coeffs, 1);
    for (int i = 0; i < 16; i++) {
        printf(" %d", coeffs[i]);
    }
    printf("\nmatrix:\n");

    inversedst((int16_t *) coeffs, (int16_t *) block, shift);

    for (int i = 0; i < 16; i++) {
        printf(" %d", block[i]);
    }

    printf("\n");
    printf("\n---------------\n");
    printf("The Base matrix:\n");
    printMatrix(matrix, SIZE);

    int result[SIZE][SIZE] = {0};
    DCT(result, matrix, SIZE);

    printf("The DCT:\n");
    printMatrix(result, SIZE);

    int idcMatrix[SIZE][SIZE] = {0};
    IDCT(idcMatrix, result, SIZE);
    printf("The IDCT:\n");
    printMatrix(idcMatrix, SIZE);
    
    return 0;
}
