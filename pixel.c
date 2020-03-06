//NAME: Andrew Bonnett
//SECTION: 001
#include "pixel.h"

bool readPixelFromFile(FILE* inputFile, PIXEL* thePixel) {
    
    fscanf(inputFile, "%c%c%c", &thePixel->b, &thePixel->g, &thePixel->r);
    //printf("%u%u%u", thePixel->b, thePixel->g, thePixel->r);
    return true;
}

bool writePixeltoFile(FILE* outputFile, PIXEL* thePixel) {
    //printf("%u%u%u", thePixel->b, thePixel->g, thePixel->r);
    fprintf(outputFile, "%c%c%c", thePixel->b, thePixel->g, thePixel->r);
    return true;
}