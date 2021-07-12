#pragma once
#include <stdio.h>

#define shiftr( x, n ) (( x + (1 << (n-1) )) >> n)

#define SHIFT_X 3
#define SHIFT_Y 10
#define INV_SHIFT_Y 7
#define INV_SHIFT_X 12

void DCT_16x16( int *x );
void IDCT_16x16( int *coef, int *block );
void show( int *x );