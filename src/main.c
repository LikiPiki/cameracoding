#include <stdio.h>
#include <stdint.h>

#define shiftr( x, n ) (( x + (1 << (n-1) )) >> n)

#define SHIFT_X 3
#define SHIFT_Y 10
#define INV_SHIFT_Y 7
#define INV_SHIFT_X 12

#define INPUT_FILE "../files/video1.yuv"
#define OUTPUT_FILE "../files/out.yuv"

#define VIDEO_WIDTH 1920
#define VIDEO_HEIGHT 1080
#define FRAME_SIZE VIDEO_WIDTH * VIDEO_HEIGHT
#define QUANTIZATION_LEVEL 64


short g_aiT16[16][16] = {
	{ 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64},
	{ 90, 87, 80, 70, 57, 43, 25,  9, -9,-25,-43,-57,-70,-80,-87,-90},
	{ 89, 75, 50, 18,-18,-50,-75,-89,-89,-75,-50,-18, 18, 50, 75, 89},
	{ 87, 57,  9,-43,-80,-90,-70,-25, 25, 70, 90, 80, 43, -9,-57,-87},
	{ 83, 36,-36,-83,-83,-36, 36, 83, 83, 36,-36,-83,-83,-36, 36, 83},
	{ 80,  9,-70,-87,-25, 57, 90, 43,-43,-90,-57, 25, 87, 70, -9,-80},
	{ 75,-18,-89,-50, 50, 89, 18,-75,-75, 18, 89, 50,-50,-89,-18, 75},
	{ 70,-43,-87,  9, 90, 25,-80,-57, 57, 80,-25,-90, -9, 87, 43,-70},
	{ 64,-64,-64, 64, 64,-64,-64, 64, 64,-64,-64, 64, 64,-64,-64, 64},
	{ 57,-80,-25, 90, -9,-87, 43, 70,-70,-43, 87,  9,-90, 25, 80,-57},
	{ 50,-89, 18, 75,-75,-18, 89,-50,-50, 89,-18,-75, 75, 18,-89, 50},
	{ 43,-90, 57, 25,-87, 70,  9,-80, 80, -9,-70, 87,-25,-57, 90,-43},
	{ 36,-83, 83,-36,-36, 83,-83, 36, 36,-83, 83,-36,-36, 83,-83, 36},
	{ 25,-70, 90,-80, 43,  9,-57, 87,-87, 57, -9,-43, 80,-90, 70,-25},
	{ 18,-50, 75,-89, 89,-75, 50,-18,-18, 50,-75, 89,-89, 75,-50, 18},
	{  9,-25, 43,-57, 70,-80, 87,-90, 90,-87, 80,-70, 57,-43, 25, -9}
};

void DCT_16x16( int *x );
void IDCT_16x16( int *coef, int *block );
void show( int *x );


/*
 * Структура одного видеокадра
 */
typedef struct {
	uint8_t y[FRAME_SIZE];
	uint8_t u[FRAME_SIZE / 4];
	uint8_t v[FRAME_SIZE / 4];
} Frame;

/*
 * Чтение одного кадра
 */
void readFrame(FILE *fp, Frame *frame) {
	if (fread(frame, sizeof(uint8_t), FRAME_SIZE + FRAME_SIZE / 2, fp) != FRAME_SIZE + FRAME_SIZE / 2) {
		if (feof(fp)) {
			printf("Premature end of file.");
		} else printf("File read error.");
	}
}

/*
 * Квантование блоков 16x16
 */
void quatizeBlock(int *block) {
	for (int i = 0; i < 16*16; i++) {
		block[i] /= QUANTIZATION_LEVEL;
	}
}

/*
 * Сохранение кадра в файл
 */
void saveToFile() {

}

void readFromFile() {
	FILE *fp;
	fp = fopen(INPUT_FILE, "rb");

	if (fp == NULL) {
	  perror("Error while opening the file.\n");
	  return;
	}

	Frame frame;
	readFrame(fp, &frame);

	fclose(fp);

	int block[16*16];
	int iblock[16*16];

	int k = 0;
	int fl = 0;
	int lastk = 0;

	for (int i = 0; i < 1920 / 16; i++) {
		for (int j = 0; j < 1080 / 16; j++) {
			int start = i * 16 + j * 16 * 1920;

			for (int k = 0; k < (16 * 16); k++) {
				block[k] = frame.y[start + (k % 16) + 1920 * (k / 16)];
				lastk = start + (k % 16) + 1920 * (k / 16);
			}

		}
	}
	printf("last is %d", lastk);

	// DCT_16x16(block);

	// show(block);
	// saveTofile(out, block);

	// quatizeBlock(block);

	// show(block);
	// IDCT_16x16( block, iblock );

	// show( iblock );
	printf("--------------");

}

int main( void )
{
	/*
	 * Чтение бинарного видеофайла -- http://ultravideo.fi/#testsequences
	 * Bosphorus 1080ю 8bit, YUV, RAW
	 */
	readFromFile();

	int block[16*16];
	int iblock[16*16];
	int i;


	for( i = 0; i < 16*16; i++ )
		block[i] = i;

	// show( block );

	DCT_16x16( block );

	/* show( block ); */

	IDCT_16x16( block, iblock );

	// show( iblock );

	return 0;
}

void DCT16x(int *data, int *spec)
{
	int k;
	int E[8], O[8];
	int EE[4], EO[4];
	int EEE[2], EEO[2];
	const int add = 1 << (SHIFT_X - 1);
	int op1, op2;

	/* E and O*/
	for (k = 0; k < 8; ++k)
	{
		op1 = data[k];
		op2 = data[15 - k];

		E[k] = op1 + op2;
		O[k] = op1 - op2;
	} 
	/* EE and EO */
	for (k = 0; k < 4; ++k)
	{
		EE[k] = E[k] + E[7-k];
		EO[k] = E[k] - E[7-k];
	}
	/* EEE and EEO */
	EEE[0] = EE[0] + EE[3];    
	EEO[0] = EE[0] - EE[3];
	EEE[1] = EE[1] + EE[2];
	EEO[1] = EE[1] - EE[2];

	spec[0]  = (int)((g_aiT16[ 0][0] * EEE[0] + g_aiT16[ 0][1] * EEE[1] + add) >> SHIFT_X);
	spec[8]  = (int)((g_aiT16[ 8][0] * EEE[0] + g_aiT16[ 8][1] * EEE[1] + add) >> SHIFT_X);
	spec[4]  = (int)((g_aiT16[ 4][0] * EEO[0] + g_aiT16[ 4][1] * EEO[1] + add) >> SHIFT_X);
	spec[12] = (int)((g_aiT16[12][0] * EEO[0] + g_aiT16[12][1] * EEO[1] + add) >> SHIFT_X);

	for(k = 2; k < 16; k += 4)
	{
		spec[k] = (int)((g_aiT16[k][0] * EO[0] + g_aiT16[k][1] * EO[1] + g_aiT16[k][2] * EO[2] + g_aiT16[k][3] * EO[3] + add) >> SHIFT_X);
	}

	for (k = 1; k < 16; k += 2)
	{
		spec[k] = (int)((g_aiT16[k][0] * O[0] + g_aiT16[k][1] * O[1] + g_aiT16[k][2] * O[2] + g_aiT16[k][3] * O[3] + 
					g_aiT16[k][4] * O[4] + g_aiT16[k][5] * O[5] + g_aiT16[k][6] * O[6] + g_aiT16[k][7] * O[7] + add) >> SHIFT_X);
	}
}


void DCT16y( int *data, int *spec )
{
	int k;
	int E[8], O[8];
	int EE[4], EO[4];
	int EEE[2], EEO[2];
	const int add = 1 << (SHIFT_Y - 1);
	int op1, op2;

	/* E and O*/
	for (k = 0; k < 8; ++k)
	{
		op1 = data[k*16];
		op2 = data[15*16 - k*16];

		E[k] = op1 + op2;
		O[k] = op1 - op2;
	} 
	/* EE and EO */
	for (k = 0; k < 4; ++k)
	{
		EE[k] = E[k] + E[7-k];
		EO[k] = E[k] - E[7-k];
	}
	/* EEE and EEO */
	EEE[0] = EE[0] + EE[3];    
	EEO[0] = EE[0] - EE[3];
	EEE[1] = EE[1] + EE[2];
	EEO[1] = EE[1] - EE[2];

	spec[0*16]  = (int)((g_aiT16[ 0][0] * EEE[0] + g_aiT16[ 0][1] * EEE[1] + add) >> SHIFT_Y);
	spec[8*16]  = (int)((g_aiT16[ 8][0] * EEE[0] + g_aiT16[ 8][1] * EEE[1] + add) >> SHIFT_Y);
	spec[4*16]  = (int)((g_aiT16[ 4][0] * EEO[0] + g_aiT16[ 4][1] * EEO[1] + add) >> SHIFT_Y);
	spec[12*16] = (int)((g_aiT16[12][0] * EEO[0] + g_aiT16[12][1] * EEO[1] + add) >> SHIFT_Y);

	for(k = 2; k < 16; k += 4)
	{
		spec[k*16] = (int)((g_aiT16[k][0] * EO[0] + g_aiT16[k][1] * EO[1] + g_aiT16[k][2] * EO[2] + g_aiT16[k][3] * EO[3] + add) >> SHIFT_Y);
	}

	for (k = 1; k < 16; k += 2)
	{
		spec[k*16] = (int)((g_aiT16[k][0] * O[0]	+ g_aiT16[k][1] * O[1] + g_aiT16[k][2] * O[2] + g_aiT16[k][3] * O[3] + 
						g_aiT16[k][4] * O[4]	+ g_aiT16[k][5] * O[5] + g_aiT16[k][6] * O[6] + g_aiT16[k][7] * O[7] + add) >> SHIFT_Y);
	}
}

void DCT_16x16( int *block )
{
	int i;

	// over rows
	for( i = 0; i < 16; i++ )
		DCT16x( &block[i * 16], &block[i * 16] );


	// over columns
	for(i = 0; i < 16; ++i)
		DCT16y( &block[i],  &block[i] );
}


void IDCT16x( int *tmp, int *block )
{
	int k;  
	int E[8], O[8];
	int EE[4], EO[4];
	int EEE[2], EEO[2];

	/* Utilizing symmetry properties to the maximum to minimize the number of multiplications */
	for( k = 0; k < 8; k++ )
	{
		O[k] = 
		  g_aiT16[ 1][k] * tmp[ 1] 
		+ g_aiT16[ 3][k] * tmp[ 3] 
		+ g_aiT16[ 5][k] * tmp[ 5]
		+ g_aiT16[ 7][k] * tmp[ 7] 
		+ g_aiT16[ 9][k] * tmp[ 9]
		+ g_aiT16[11][k] * tmp[11] 
		+ g_aiT16[13][k] * tmp[13]
		+ g_aiT16[15][k] * tmp[15];
	}

	for( k = 0; k < 4; k++ )
	{
		EO[k] = 
		  g_aiT16[ 2][k] * tmp[ 2] 
		+ g_aiT16[ 6][k] * tmp[ 6] 
		+ g_aiT16[10][k] * tmp[10] 
		+ g_aiT16[14][k] * tmp[14];
	}

	EEO[0] = g_aiT16[4][0] * tmp[4] + g_aiT16[12][0] * tmp[12];
	EEE[0] = g_aiT16[0][0] * tmp[0] + g_aiT16[ 8][0] * tmp[ 8];
	EEO[1] = g_aiT16[4][1] * tmp[4] + g_aiT16[12][1] * tmp[12];
	EEE[1] = g_aiT16[0][1] * tmp[0] + g_aiT16[ 8][1] * tmp[ 8];

	/* Combining even and odd terms at each hierarchy levels to calculate the final spatial domain vector */ 
	for( k = 0; k < 2; k++ )
	{
		EE[k]   = EEE[k] + EEO[k];
		EE[k+2] = EEE[1-k] - EEO[1-k];
	}    

	for( k = 0; k < 4; k++ )
	{
		E[k]   = EE[k] + EO[k];
		E[k+4] = EE[3-k] - EO[3-k];
	}    

	for( k = 0; k < 8; k++ )
	{
		block[k]     = (int)shiftr( E[k]   + O[k],   INV_SHIFT_X );
		block[k + 8] = (int)shiftr( E[7-k] - O[7-k], INV_SHIFT_X );
	}        
}

void IDCT16y(int *tmp, int *block )
{
	int k;  
	int E[8], O[8];
	int EE[4], EO[4];
	int EEE[2], EEO[2];

	// Utilizing symmetry properties to the maximum to minimize the number of multiplications 
	for( k = 0; k < 8; k++ )
	{
		O[k] = g_aiT16[ 1][k] * tmp[ 1*16] + g_aiT16[ 3][k] * tmp[ 3*16] + 
			   g_aiT16[ 5][k] * tmp[ 5*16] + g_aiT16[ 7][k] * tmp[ 7*16] + 
			   g_aiT16[ 9][k] * tmp[ 9*16] + g_aiT16[11][k] * tmp[11*16] + 
			   g_aiT16[13][k] * tmp[13*16] + g_aiT16[15][k] * tmp[15*16];
	}

	for( k = 0; k < 4; k++ )
	{
		EO[k] = g_aiT16[ 2][k] * tmp[ 2*16] + g_aiT16[ 6][k] * tmp[ 6*16] + 
				g_aiT16[10][k] * tmp[10*16] + g_aiT16[14][k] * tmp[14*16];
	}

	EEO[0] = g_aiT16[4][0] * tmp[4*16] + g_aiT16[12][0] * tmp[12*16];
	EEE[0] = g_aiT16[0][0] * tmp[0*16] + g_aiT16[ 8][0] * tmp[ 8*16];
	EEO[1] = g_aiT16[4][1] * tmp[4*16] + g_aiT16[12][1] * tmp[12*16];
	EEE[1] = g_aiT16[0][1] * tmp[0*16] + g_aiT16[ 8][1] * tmp[ 8*16];

	// Combining even and odd terms at each hierarchy levels to calculate the final spatial domain vector 
	for( k = 0; k < 2; k++ )
	{
		EE[k]   = EEE[k] + EEO[k];
		EE[k+2] = EEE[1-k] - EEO[1-k];
	}    

	for( k = 0; k < 4; k++ )
	{
		E[k]   = EE[k] + EO[k];
		E[k+4] = EE[3-k] - EO[3-k];
	}    

	for( k = 0; k < 8; k++ )
	{
		block[k*16]        = (int)shiftr( E[k]   + O[k],   INV_SHIFT_Y );
		block[k*16 + 8*16] = (int)shiftr( E[7-k] - O[7-k], INV_SHIFT_Y );
	}        
}


void IDCT_16x16( int *coef, int *block )
{
	int i;

	// over columns 
	for(i = 0; i < 16; ++i)
		IDCT16y( &coef[i], &coef[i] );

	// over rows 
	for( i = 0; i < 16; i++ )
		IDCT16x( &coef[i*16], &block[i*16] );
}

void show( int *x )
{
	int i; 

	for( i = 0; i < 16*16; i++ )
	{
		if( i % 16 == 0 ) 	
			printf("\n");

		printf("%4d", x[i] );
	}
	printf("\n");
}

