# bmp_filter
CS 224 (Computer Systems) Lab

## Background: 
Note: This section is a digest of the wikipedia entry on the bmp file format, which can be found [here](https://en.wikipedia.org/wiki/BMP_file_format)

The bmp image format has been in use for several decades and is a portable method of representing digital image data on virtually any modern computing system. For this lab you will build a command line tool for applying filters to simple bitmap (bmp) images. Though bmp is capable of numerous features that build upon the original specification, we will concentrate on very basic images that are comprised of two dimensional arrays of pixels - with each pixel represented by three bytes of information. These three bytes represent the Red, Green, and Blue (RGB) color intensity values for each pixel. A value of zero indicates that the color is absent, a value of 255 indicates the maximum intensity for that color for the given pixel. By combining these three "primary color" values nearly 17 million unique colors can be described and rendered. 

bmp files contain more than simply the RGB values for each pixel in an image. In order to recognize and interpret a bmp file properly certain things must be known, such as the width and height of the image being represented. Such information is contained in a set of informative bytes located at the beginning of a bmp file. We refer to these bytes a "headers".  Ultimately a valid bmp file must contain AT LEAST three things - the bmp header, a "Device-Independent Bitmap" header (DIB header for short), and the pixel array that encodes the image.

![bmplayout.png](/readme_images/bmplayout.png)

Let's look at each of these three items...

### Bitmap Header:
The bitmap header consists of 14 bytes divided into five fields as shown below. The header field specifies the type of bitmap the file contains. For our purposes these two bytes will always contain the ascii characters "BM". The next two fields that we are concerned with are the size of the file in bytes, and the offset of the first byte in the pixel array. In this lab we will not concern ourselves with the contents of the reserved fields.

![bmpheader.png](/readme_images/bmpheader.png)

### DIB Header:
In addition to the bitmap file header, a valid bmp must contain a "Device-Independent Bitmap" header (DIB header for short). This header is utilized by an application to properly interpret the bytes of the file that encode the image. Though there are several different DIB header formats to choose from, for this lab we will limit ourselves to the BITMAPINFOHEADER.  It's fields, their purposes, and our usage of them is outlined in the following image...


![dibheader.png](/readme_images/dibheader.png)
 

### Pixel Array:
The final piece of the bmp file format that you will need to be able to work with for this lab is the pixel array. This is where the information describing the pixels in the image are located. As noted earlier, the images we will be working with represent single pixels with trios of bytes that store numeric values between 0 and 255 that indicate the intensity of the red, green, and blue components of the pixel's color. Though we often refer to these values as RGB - please note that in the bmp format they are actually stored in the order Blue, Green, Red - probably just to keep life interesting...

Pixels are stored row by row, starting in the lower left corner of the image reading left to right and then bottom to top as shown in the following figure.

![pixelarray.png](/readme_images/pixelarray.png)

### Pixel Row Padding:
An important point to note about the pixel array is that each row stored in the file is required to occupy a multiple of 4 bytes. If the number of bytes needed to represent the pixels of a row does not add up to a multiple of 4 bytes then padding (extra bytes that are not part of the image) must be inserted. Thus, if an image were 8 pixels wide (as shown above), 3 X 8 = 24 bytes, which IS a multiple of 4 so no padding would be needed.

On the other hand if an image were 2 pixels wide then 3 X 2 = 6 bytes, which is NOT a multiple of 4, so padding is needed. In this case two extra bytes would need to inserted (as shown in the image below), bringing the number to 8. The value stored in these bytes isn't important and can be set to zero.

![pixelarraypadding.png](/readme_images/pixelarraypadding.png)

You can read more about the bmp standard here: https://en.wikipedia.org/wiki/BMP_file_format (Links to an external site.)

## Your Task:
For this lab you will build a command line tool called bmpFilter that applies simple filters to an input image and generates an output image from the result. Your tool should take 2 required command line arguments (in addition to the program's name). The first argument will be the name of a bmp input file containing an image to be modified. The second argument will be a flag that indicates which filter to apply. The flags that you must support and the filters they will invoke are as follows:

T - threshold: Each pixel in the image is converted to either white or black based on the average intensity of its three color values. Pixels in the original image with an average intensity of 128 or more will become white; those with average intensities below 128 will become black. Remember that integer arithmetic can/will result in overflow and truncation errors when performing  addition and division (especially when working with 8 bit variables). Consider steps you should take to eliminate overflow and minimize truncation error when computing averages. Hint: consider the order of operations and when/how to store intermediate values/sums...
G - greyscale: Each pixel in the image is converted to a "grey" pixel by setting all three color values equal to the average of the three original values. For example, if a pixel's RGB values are 100, 55, and 200; the average integer value of which is 118, this pixel's new RGB values would be set to 118, 118, and 118. Note: Pay attention to the warning concerning arithmetic at the end of the description of the threshold filter above.
S - scale-down: This filter will scale an image down to half its dimensions in both width and height. In other words, if the original image is 100 pixels wide and 50 pixels tall, then the resulting image should be 50 pixels wide and 25 pixels tall. In general this will mean that four pixels in a square pattern from the original image become a single pixel in the filtered image. Determine the RGB values of this "composite" pixel by averaging the values of the four original pixels together on a per-color basis (i.e. compute the average of the red values, the average of the green values, and the average of the blue values and use these as the RGB values for the new pixel). Start in the bottom left corner of an image and work row by row, left to right and bottom to top to perform the scaling. Depending on the dimensions of the original image you may not have four pixels to work with when you reach the right and top edges of the image. In such cases simply compute the averages based on the pixels that are available (i.e. on the right edge you may only have two pixels stacked vertically to work with and on the top edge of the image you may only have two pixels sitting horizontally side-by-side to work with. It is also possible to end up with a single pixel left in the upper-right corner of the image after processing all the other pixels. In such a case this pixel would simply end up copied unchanged to the new image.
As an example of usage of these flags, if I wanted to apply the greyscale filter to an image in a file called "hat.bmp" I would invoke the finished program as follows:
```
./bmpFilter hat.bmp G
```
The output file from bmpFilter should be named "filtered.bmp".

### Hints:
As always, don't try to complete the entire lab at once. Start by verifying that you are able to open a bmp file and correctly read and identify the different pieces of the headers. Next attempt implementing one of the simpler filters (greyscale or threshold), then the other. Save scale-down for last. 
Consider creating a struct for storing the three byte RGB values for a single pixel vs trying to keep track of everything as individual bytes.
It is probably a good idea to have two 2-D arrays to work with. One containing the original image data, the other the result of applying the filter.
Though possible, it is not advised that you use exclusively fscanf() and fprintf() to complete this lab. We suggest using fread() and fwrite(), which are better suited for reading groups of bytes from a file into memory. Here is an example usage of both of these functions:
```
numberOfElementsRead = fread(&aVariable, sizeOfElementInBytes, numberOfElementsToRead, filePointer);
```
```
numberOfElementsWritten = fwrite(&aVariable, sizeOfElementInBytes, numberOfElementsToWrite, filePointer);
```
Don't try to do everything in a single function - create functions that perform useful tasks such as reading or writing pixel array data, applying specific filters, etc.
Spend some time thinking about how to approach these problems before you start coding. Draw pictures and take notes of your thoughts - THEN try coding things up.
Start early.
Ask for clarification if/when you need it!
Check back here later for additional information and hints.
