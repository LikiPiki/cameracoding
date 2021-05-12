/* 
 * This file include the basic DCT transformations with matrix [SIZE]x[SIZE]
 */
#include <stdio.h>
#include <math.h>

#define SIZE 4 
#define LINE_SIZE SIZE * SIZE

#define PI 3.14159265358979323846

// print [SIZE]x[SIZE] matrix
void printMatrix(int matrix[][SIZE], int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
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


void testDCT() {
    int matrix[SIZE][SIZE] = {
        { -76, -73, -67, -62},
        { -65, -69, -73, -38},
        { -66, -69, -60, -15},
        { -65, -70, -57,  -6},
    };


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
}

