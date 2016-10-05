// Name: Neil Barooah

#include "mylib.h"
#include "text.h"

const int stage1[24] = {
        58,	15,	1,	1,
	75,	15,	1,	2,
	92,	15,	1,	3,
	109,	15,	1,	3,
	126,	15,	1,	2,
	143,	15,	1,	1
};

const int stage2[32] = {
	75,	15,	1,	3,
	98,	15,	1,	1,
	121,	15,	1,	3,
	144,	15,	1,	1,
	75,	32,	1,	2,
	98,	32,	1,	1,
	121,	32,	1,	2,
	143,	32,	1,	1
};

const int stage3[36] = {
	38,	20,	1,	1,
	68,	20,	1,	3,
	98,	20,	1,	1,
	128,	20,	1,	3,
	158,	20,	1,	1,
	53,	35,	1,	2,
	83,	35,	1,	1,
	113,	35,	1,	1,
	143,	35,	1,	2
};

const int stage4[40] = {
	75,	15,	1,	1,
	92,	15,	1,	3,
	109,	15,	1,	1,
	126,	15,	1,	3,
	143,	15,	1,	1,
	75,	32,	1,	2,
	92,	32,	1,	3,
	109,	32,	1,	2,
	126,	32,	1,	1,
	143,	32,	1,	2,
};
	

u16* videoBuffer = (u16*) 0x6000000;

void setPixel(int x, int y, u16 color) {
	videoBuffer[x * 240 + y] = color;
}

void drawImage3(int x, int y, int width, int height, const u16 *image)
{
    for(;height >0; height--)
    {
        DMA[3].src = image;
        DMA[3].dst = videoBuffer + 240*x + y;
        DMA[3].cnt = width | DMA_ON;
        image = image + width;
        x++; 
    }
}

void clearScreen() {
	for(int r=0; r<160; r++)
	{

		DMA[3].src = 0x0000;
		DMA[3].dst = &videoBuffer[OFFSET(0+r, 0, 240)];
		DMA[3].cnt = 240 | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void removeShip(int x, int y, int width, int height, u16 color) {
	for(int r=0; r<height; r++)
	{

		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(x+r, y, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawChar(int x, int y, char letter, u16 color) {
	for (int h = 0; h < 8; h++)
		for (int w = 0; w < 6; w++)
			if (fontdata_6x8[h * 6 + w + 48 * letter])
				setPixel(x + h, y + w, color);
}

void drawText(int x, int y, char *string, u16 color) {
	while (*string) {
        	drawChar(x, y, *string++, color);
        	y += 5;
    	}
}

void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}


void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*10000; i++)
	{
		x++;
	}
}
