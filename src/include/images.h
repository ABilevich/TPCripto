#ifndef IMG_H
#define IMG_H
#include <stdint.h>

typedef struct IMAGEDATA 
{
    uint8_t * bitmapImage;     // The image data section as an array
    uint32_t    biWidth;         // The width of the image (18-21 bytes)
    uint32_t    biHeight;        // The height of the image (22-25 bytes)
    uint32_t    biSize;
    uint32_t   bfOffBits;
} IMAGEDATA;

IMAGEDATA* analizeImage(char *path);

int updateImageData(char *path, IMAGEDATA * imageData);
uint8_t * calculateOfuscatedValues( uint8_t w,  uint8_t v, uint8_t u, uint8_t y );

#endif