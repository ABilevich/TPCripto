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
    // BITMAPFILEHEADER fileHeader;
    // BITMAPINFOHEADER infoHeader;
} IMAGEDATA;

IMAGEDATA * analizeImage(char *path, int flip);

int updateImageData(char *path, IMAGEDATA * imageData, int flip);
uint8_t reconstructY(uint8_t w, uint8_t v,  uint8_t u);
uint8_t * calculateOfuscatedValues( uint8_t w,  uint8_t v, uint8_t u, uint8_t y );
int createImage(char * inputPath, IMAGEDATA * input_image_data, char * outputPath ,uint8_t* bitmapImage);
uint8_t * invertImage(uint8_t *bitmapImage, uint32_t width, uint32_t heigth);

#endif