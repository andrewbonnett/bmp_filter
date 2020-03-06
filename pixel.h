//NAME: Andrew Bonnett
//SECTION: 001
#ifndef PIXEL_STRUCT
#define PIXEL_STRUCT

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} PIXEL;

bool readPixelFromFile(FILE* inputFile, PIXEL* thePixel);

bool writePixeltoFile(FILE* outputFile, PIXEL* thePixel);

#endif