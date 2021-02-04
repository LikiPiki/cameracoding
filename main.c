#include <stdio.h>
#include <math.h>

#define SIZE 4
#define PI 3.14159265358979323846

// вывод матрицы
void printMatrix(double matrix[][SIZE], int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%3.3f ", matrix[i][j]);
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

double getDCT(double u, double v, double matrix[][SIZE], int len) {
    double sum = 0;
    for (double x = 0; x < len; x++) {
        for (double y = 0; y < len; y++) {
            sum += matrix[(int) x][(int)y] * cos(((2 * x + 1) * u * PI) / (2 * len)) * cos(((2 * y + 1) * v * PI) / (2 * len));
        }
    }
    return sum;
}

void DCT(double result[][SIZE], double matrix[][SIZE], int len) {
    for (int u = 0; u < len; u++) {
        for (int v = 0; v < len; v++) {
            result[u][v] = 0.25 * alpha(u) * alpha(v) * getDCT((double) u, (double) v, matrix, len);
        }
    }
}

double getIDCT(double DCTMatrix[][SIZE], double x, double y, int len) {
    double sum = 0;
    for (int u = 0; u < len; u++) {
        for (int v = 0; v < len; v++) {
            sum += alpha(u) * alpha(v)  * DCTMatrix[(int) u][(int) v] * cos(((2 * x + 1) * u * PI) / (2 * len)) * cos(((2 * y + 1) * v * PI) / (2 * len));
        }
    }
    return sum;
}

void IDCT(double result[][SIZE], double matrix[][SIZE], int len) {
    for (int x = 0; x < len; x++) {
        for (int y = 0; y < len; y++) {
            result[x][y] = 0.25 * getIDCT(matrix, x, y, len);
        }
    }
}


int main(int argc, char *argv[]) {

    double matrix[SIZE][SIZE] = {
        { -76, -73, -67, -62 },
        { -65, -69, -73, -38 },
        { -66, -69, -60, -15 },
        { -65, -70, -57,  -6 },
    };

    printf("The Base matrix:\n");
    printMatrix(matrix, SIZE);

    double result[SIZE][SIZE] = {0};
    DCT(result, matrix, SIZE);

    printf("The DCT:\n");
    printMatrix(result, SIZE);

    double idcMatrix[SIZE][SIZE] = {0};
    IDCT(idcMatrix, result, SIZE);
    printf("The IDCT:\n");
    printMatrix(idcMatrix, SIZE);
    
    return 0;
}
