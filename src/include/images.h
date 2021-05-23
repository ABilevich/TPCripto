#ifndef IMG_H
#define IMG_H

typedef struct IMAGEDATA 
{
    unsigned char * bitmapImage;     // The image data section as an array
    unsigned int    biWidth;         // The width of the image (18-21 bytes)
    unsigned int    biHeight;        // The height of the image (22-25 bytes)
    unsigned int    biSize;
} IMAGEDATA;

IMAGEDATA* analizeImage(char *path);

#endif