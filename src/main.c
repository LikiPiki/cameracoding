#include <stdio.h>
#include <stdint.h>

#include "writter.h"
#include "reader.h"
#include "dct.h"

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
