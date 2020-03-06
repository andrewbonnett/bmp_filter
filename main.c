//NAME: Andrew Bonnett
//SECTION: 001
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pixel.h"


typedef struct BMPHEADER {
    unsigned short bmptype;  //2 bytes
    unsigned int bmpsize;    //4 bytes
    unsigned short reservedbytes;//skip 4 reserved bytes
    unsigned int offset; //offset from pixel data from beginning of file
} BMPHEADER;


typedef struct DIBHEADER {
    unsigned int size; //(should be 40?) 4 bytes
    int width; //width of bmp pixels  4 bytes
    int height; //heigth of bmp pixels 4 bytes
    unsigned short colorplanes; //number of colorplanes 2 bytes
    unsigned short bitsperpixel; //number of bits per pixel 2 bytes
    unsigned int compressionMethod; //4 bytes
    unsigned int imageSizeInBytes; //4 bytes
    unsigned int horizontalRes;  //(measured in pixels per meter) 4 bytes
    unsigned int verticalRes;    //(measured in pixels per meter) 4 bytes
    unsigned int colorsInPalette;    //defaults to 2^n   4 bytes
    unsigned int numImportantColors; //4 bytes
} DIBHEADER;


int main(int argC, char** argV) {
    //check argument count...
    if(argC != 3){
        printf("Usage: %s <filename> <filterflag>\n", argV[0]);
        return 1;
    }
    //make sure we can open the file... (first argument will be the name of a bmp input file containing an image to be modified.)
    FILE* inputFile = fopen(argV[1], "r");
    if(inputFile == NULL){
        printf("Could not open %s!!\n", argV[1]);
        return 2;
    }
    //The second argument will be a flag that indicates which filter to apply: T, G, or S
    if (argV[2][0] == 'T') { printf("Threshold for file %s has been selected\n", argV[1]); }
    else if(argV[2][0] == 'G') { printf("Greyscale for file %s has been selected\n", argV[1]); }
    else if(argV[2][0] == 'S') { printf("Scale-down for file %s has been selected\n", argV[1]); }
    else if(argV[2][0] == 'N') { printf("No filter for file %s has been selected\n", argV[1]); }
    else {
        printf("Usage: %s <filename> <filterflag>\n", argV[0]);
        printf("Possible Filter Flags:\nT - threshold\nG - greyscale\nS - scale-down\n");
        return 1;
    }
    
    
    //create BMPHEADER struct and read in the header
    BMPHEADER header1;
    size_t result;
    
    //set the pointer to the beginning of the file
    fseek(inputFile,0,SEEK_SET);
    
    result = fread(&header1.bmptype, 2, 1, inputFile);
        if (result != 1) { printf( "Error reading in the BMPHeader (bmptype)\n"); }
    result = fread(&header1.bmpsize, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the BMPHeader (bmpsize)\n"); }
    result = fread(&header1.reservedbytes, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the BMPHeader (reservedbytes)\n"); }
    result = fread(&header1.offset, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the BMPHeader (offset)\n"); }
        
    //create DIBHEADER struct and read in the header
    
    DIBHEADER header2;
    
    result = fread(&header2.size, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (size)\n"); }
    result = fread(&header2.width, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (width)\n"); }
    result = fread(&header2.height, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (height)\n"); }
    result = fread(&header2.colorplanes, 2, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (colorplanes)\n"); }
    result = fread(&header2.bitsperpixel, 2, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (bitsperpixel)\n"); }
    result = fread(&header2.compressionMethod, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (compressionMethod\n"); }
    result = fread(&header2.imageSizeInBytes, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (imageSizeInBytes)\n"); }
    result = fread(&header2.horizontalRes, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (horizontalRes)\n"); }
    result = fread(&header2.verticalRes, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (verticalRes)\n"); }
    result = fread(&header2.colorsInPalette, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (colorsInPalette)\n"); }
    result = fread(&header2.numImportantColors, 4, 1, inputFile);
        if (result != 1) { printf( "Error reading in the DIBHeader (numImportantColors)\n"); }
    
    //begin reading in the pixels, from bottom left using the offset function
    printf ("size: %d\nwidth: %d\nheight: %d\n", header1.bmpsize, header2.width, header2.height);
    
    fseek(inputFile,header1.offset,SEEK_SET);
    
    //account for junk bytes at the end of each row for padding
    int numPaddedBytes = ((3 * header2.width) % 4) % 4;
    unsigned char junkbyte;
    printf("NumPaddedBytes: %d\n", numPaddedBytes);
    int i;
    int y;
    int x;
    
    
     //allocate memory for (1) an array of pixel pointers for each row and (2) for each pixel pointer an array size of the row of pixels
    PIXEL** original = (PIXEL**)malloc(header2.height * sizeof(PIXEL*));
    for (i = 0; i < header2.height; i++) {
        original[i] =  malloc(header2.width*sizeof(PIXEL));
    }

    //read pixels from the file and place them in the array
    for (y = 0; y < header2.height; y++) {
        for (x = 0; x < header2.width; x++) {
            //read the Pixel
            readPixelFromFile(inputFile, &original[y][x]);
            //account for padding
            if (x == header2.width-1 ) { //&& numPaddedBytes != 0) {
                for (i = 0; i < numPaddedBytes; i++) {
                    fread(&junkbyte, 1, 1, inputFile);
                }
                //fseek(inputFile, header1.offset + (header2.width * sizeof(PIXEL)+numPaddedBytes) * (y+1), SEEK_SET);
            }
        }
    }
    //print the R,G,B elements of each pixel
    /*
    printf("ORIGINAL\n");
    for (y = 0; y < header2.height; y++) {
        for (x = 0; x < header2.width; x++) {
            printf("%u " ,original[y][x].r);
            printf("%u " ,original[y][x].g);
            printf("%u\n" ,original[y][x].b);
        }
    }
    
    fclose (inputFile);
    */
    
    if(argV[2][0] == 'G' || argV[2][0] == 'T' || argV[2][0] == 'N') {
            
            //create array of filtered pixels
            PIXEL** filtered = (PIXEL**)malloc(header2.height * sizeof(PIXEL*));
            for (i = 0; i < header2.height; i++) {
                filtered[i] =  malloc(header2.width*sizeof(PIXEL));
            }
            
            //copy pixels from the old array and and place them in the new array
            for (y = 0; y < header2.height; y++) {
                for (x = 0; x < header2.width; x++) {
                    filtered[y][x].r = original[y][x].r;
                    filtered[y][x].g = original[y][x].g;
                    filtered[y][x].b = original[y][x].b;
                }
            }
            //edit the pixels to print grayscale
            if (argV[2][0] == 'G') {
                for (y = 0; y < header2.height; y++) {
                    for (x = 0; x < header2.width; x++) {
                        filtered[y][x].r = (original[y][x].b + original[y][x].g + original[y][x].r)/3;
                        filtered[y][x].g = filtered[y][x].r;
                        filtered[y][x].b = filtered[y][x].r;
                    }
                }
            }
            //edit the pixels to print threshold
            else if (argV[2][0] == 'T') {
                for (y = 0; y < header2.height; y++) {
                    for (x = 0; x < header2.width; x++) {
                            //Pixels in the original image with an average intensity of 128 or more will become white;
                            //those with average intensities below 128 will become black.
                            if ((original[y][x].b + original[y][x].g + original[y][x].r)/3 > 128) {
                                filtered[y][x].r = 255;
                            }
                            else {
                                filtered[y][x].r = 0;
                            }
                            filtered[y][x].g = filtered[y][x].r;
                            filtered[y][x].b = filtered[y][x].r;
                    }
                }
            }
            
            /*
            //print the R,G,B elements of each pixel
            printf("FILTERED\n");
            for (y = 0; y < header2.height; y++) {
                for (x = 0; x < header2.width; x++) {
                    printf("%u " ,filtered[y][x].r);
                    printf("%u " ,filtered[y][x].g);
                    printf("%u\n" ,filtered[y][x].b);
                }
            }
            */
            
            //rewrite header and pixels to outputfile
            FILE * outputFile;
            outputFile = fopen("output.bmp", "wb");
            fseek(outputFile,0,SEEK_SET);
            fwrite(&header1.bmptype, 2, 1, outputFile);
            fwrite(&header1.bmpsize, 4, 1, outputFile);
            fwrite(&header1.reservedbytes, 4, 1, outputFile);
            fwrite(&header1.offset, 4, 1, outputFile);
            
            
            fwrite(&header2.size, 4, 1, outputFile);
            fwrite(&header2.width, 4, 1, outputFile);
            fwrite(&header2.height, 4, 1, outputFile);
            fwrite(&header2.colorplanes, 2, 1, outputFile);
            fwrite(&header2.bitsperpixel, 2, 1, outputFile);
            fwrite(&header2.compressionMethod, 4, 1, outputFile);
            fwrite(&header2.imageSizeInBytes, 4, 1, outputFile);
            fwrite(&header2.horizontalRes, 4, 1, outputFile);
            fwrite(&header2.verticalRes, 4, 1, outputFile);
            fwrite(&header2.colorsInPalette, 4, 1, outputFile);
            fwrite(&header2.numImportantColors, 4, 1, outputFile);
            
            fseek(outputFile,header1.offset,SEEK_SET);
            for (y = 0; y < header2.height; y++) {
                for (x = 0; x < header2.width; x++) {
                    //write to the file
                    writePixeltoFile(outputFile, &filtered[y][x]);
                    //account for padding
                    if (x == header2.width-1) { //&& numPaddedBytes != 0) {
                        for (i = 0; i < numPaddedBytes; i++) {
                            fwrite(&junkbyte, 1, 1, outputFile);
                        }
                        //fseek(outputFile, header1.offset + (header2.width * sizeof(PIXEL)+numPaddedBytes) * (y+1), SEEK_SET);
                    }
                }
            }
            fclose (outputFile);
    } //end of if statement for grayscale and threshold
    
    
    //scale-down function
    
    else if(argV[2][0] == 'S') {
        //find dimensions of scaled down array
        int scaledwidth = header2.width/2 + header2.width%2;
        int scaledheight = header2.height/2 + header2.height%2;
        
        printf("scaledwidth: %d\nscaledheight: %d\n", scaledwidth, scaledheight);
        
        //create array of scaled down pixels
            PIXEL** scaled = (PIXEL**)malloc(scaledheight * sizeof(PIXEL*));
            for (i = 0; i < scaledheight; i++) {
                scaled[i] =  malloc(scaledwidth*sizeof(PIXEL));
            }
            
        //iterate through the original image and grab the pixels for the scaled image
        int heightcounter = 0;
        int widthcounter = 0;
        for (y = 0; y < header2.height; y = y + 2) {
            for (x = 0; x < header2.width; x = x + 2) {
                //access the rgb pixels for four pixels in the box and take the average of each and store the average in a new pixel
                if (x != header2.width-1 && y != header2.height-1) {
                    //printf("CASE 1: Pixels in row %d are being stored in scaled array of pixels\n", heightcounter);
                    scaled[heightcounter][widthcounter].r = (original[y][x].r + original[y+1][x].r + original[y][x+1].r + original[y+1][x+1].r)/4;
                    scaled[heightcounter][widthcounter].g = (original[y][x].g + original[y+1][x].g + original[y][x+1].g + original[y+1][x+1].g)/4;
                    scaled[heightcounter][widthcounter].b = (original[y][x].b + original[y+1][x].b + original[y][x+1].b + original[y+1][x+1].b)/4;
                }
                else if (x == header2.width-1 && y != header2.height-1) {
                    //printf("CASE 2: Pixels in column %d are being stored in scaled array of pixels\n", widthcounter);
                    scaled[heightcounter][widthcounter].r = (original[y][x].r + original[y+1][x].r)/2;
                    scaled[heightcounter][widthcounter].g = (original[y][x].g + original[y+1][x].g)/2;
                    scaled[heightcounter][widthcounter].b = (original[y][x].b + original[y+1][x].b)/2;
                }
                else if (x != header2.width-1 && y == header2.height-1) {
                    //printf("CASE 3: Pixels in row %d are being stored in scaled array of pixels\n", heightcounter);
                    scaled[heightcounter][widthcounter].r = (original[y][x].r + original[y][x+1].r)/2;
                    scaled[heightcounter][widthcounter].g = (original[y][x].g + original[y][x+1].g)/2;
                    scaled[heightcounter][widthcounter].b = (original[y][x].b + original[y][x+1].b)/2;
                }
                else if (x == header2.width-1 && y == header2.height-1) {
                    //printf("CASE 4: TOP CORNER BABY");
                    scaled[heightcounter][widthcounter].r = original[y][x].r;
                    scaled[heightcounter][widthcounter].g = original[y][x].g;
                    scaled[heightcounter][widthcounter].b = original[y][x].b;
                }
                widthcounter++;
            }
            heightcounter++;
            widthcounter = 0;
        }
        /*
        printf("SCALED\n");
            for (y = 0; y < scaledheight; y++) {
                for (x = 0; x < scaledwidth; x++) {
                    printf("%u " , scaled[y][x].r);
                    printf("%u " , scaled[y][x].g);
                    printf("%u\n" , scaled[y][x].b);
                }
            }
        */
        
        //rewrite header and pixels to outputfile
        int numScaledPaddedBytes = ((3*scaledwidth) % 4);
        if (numScaledPaddedBytes == 3) { numScaledPaddedBytes = 1; }
        else if (numScaledPaddedBytes == 1) { numScaledPaddedBytes = 3; }
        printf("NumScaledPaddedBytes: %d\n", numScaledPaddedBytes);
        unsigned int newbmpsize = header1.offset + (scaledwidth+numScaledPaddedBytes) * scaledheight;
        
            FILE * outputFile2;
            outputFile2 = fopen("scaled.bmp", "wb");
            fseek(outputFile2,0,SEEK_SET);
            fwrite(&header1.bmptype, 2, 1, outputFile2);
            fwrite(&newbmpsize, 4, 1, outputFile2);
            fwrite(&header1.reservedbytes, 4, 1, outputFile2);
            fwrite(&header1.offset, 4, 1, outputFile2);
            
            
            fwrite(&header2.size, 4, 1, outputFile2);
            fwrite(&scaledwidth, 4, 1, outputFile2);
            fwrite(&scaledheight, 4, 1, outputFile2);
            fwrite(&header2.colorplanes, 2, 1, outputFile2);
            fwrite(&header2.bitsperpixel, 2, 1, outputFile2);
            fwrite(&header2.compressionMethod, 4, 1, outputFile2);
            fwrite(&header2.imageSizeInBytes, 4, 1, outputFile2);
            fwrite(&header2.horizontalRes, 4, 1, outputFile2);
            fwrite(&header2.verticalRes, 4, 1, outputFile2);
            fwrite(&header2.colorsInPalette, 4, 1, outputFile2);
            fwrite(&header2.numImportantColors, 4, 1, outputFile2);
            
            fseek(outputFile2,header1.offset,SEEK_SET);
            
            for (y = 0; y < scaledheight; y++) {
                for (x = 0; x < scaledwidth; x++) {
                    //write to the file
                    writePixeltoFile(outputFile2, &scaled[y][x]);
                }
                for (i = 0; i < numScaledPaddedBytes; i++) {
                    fwrite(&junkbyte, 1, 1, outputFile2);
                }
            }
            fclose (outputFile2);
            
        
    } //end of if statement for scaled down
    
    return 0;
}