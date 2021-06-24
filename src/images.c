#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/images.h"

// referencia: https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure

typedef struct tagBITMAPFILEHEADER 
{
    //unsigned short bfType; // 19778, must be a BM string, the corresponding hexadecimal is 0x4d42, and the decimal is 19778, otherwise it is not a bmp format file
    uint32_t   bfSize;        // File size in bytes (2-5 bytes)
    uint16_t bfReserved1;   // Reserved, must be set to 0 (6-7 bytes)
    uint16_t bfReserved2;   // Reserved, must be set to 0 (8-9 bytes)
    uint32_t   bfOffBits;     // Offset from file header to pixel data (10-13 bytes)
} BITMAPFILEHEADER;

//Image information header structure
typedef struct tagBITMAPINFOHEADER 
{
    uint32_t    biSize;          // The size of this structure (14-17 bytes)
    uint32_t    biWidth;         // The width of the image (18-21 bytes)
    uint32_t    biHeight;        // The height of the image (22-25 bytes)
    uint16_t  biPlanes;        // Indicates the plane attribute of the bmp picture, obviously the display has only one plane, so it is always equal to 1 (26-27 bytes)
    uint16_t  biBitCount;      // The number of bits occupied by a pixel, generally 24 (28-29 bytes)
    uint32_t    biCompression;   // Describe the type of image data compression, 0 means no compression. (30-33 bytes)
    uint32_t    biSizeImage;     // The size of pixel data, this value should be equal to bfSize-bfOffBits (34-37 bytes) in the file header structure above
    uint32_t    biXPelsPerMeter; // Describe the horizontal resolution, expressed in pixels/meter. Generally 0 (38-41 bytes)
    uint32_t    biYPelsPerMeter; // Describe the vertical resolution, expressed in pixels/meter. Generally 0 (42-45 bytes)
    uint32_t    biClrUsed;       // Describe the number of color indexes in the color table actually used by the bitmap (if set to 0, all palette items are used). (46-49 bytes)
    uint32_t    biClrImportant;  // Explain the number of color indexes that have an important influence on the image display. If it is 0, it means all are important. (50-53 bytes)
} BITMAPINFOHEADER;




IMAGEDATA * analizeImage(char *path, int flip){


    uint8_t *bitmapImage;  //store image data


    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    FILE* fp;    
    fp = fopen(path, "rb");//Read the image.bmp file in the same directory.
    if(fp == NULL)
    {
        fprintf(stderr, "No se pudo abrir la imagen: '%s'!\n",path);
        exit(EXIT_FAILURE);
    }
    //If you do not read the bifType first, according to the C language structure Sizeof operation rule-the whole is greater than the sum of the parts, resulting in misalignment of the read file
    uint16_t  fileType;   
    fread(&fileType,1,sizeof (uint16_t), fp);  
    if (fileType == 0x4d42)   
    {   

        fread(&fileHeader, 1, sizeof(BITMAPFILEHEADER), fp);
        fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), fp);
    }

    //move file point to the beginning of bitmap data
    fseek(fp, fileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    IMAGEDATA * imageData = (IMAGEDATA*) malloc(sizeof(IMAGEDATA));

    bitmapImage = (uint8_t*) malloc(infoHeader.biWidth * infoHeader.biHeight);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(fp);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,1,infoHeader.biWidth * infoHeader.biHeight,fp);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(fp);
        return NULL;
    }

    if(flip){
        uint8_t * bitmapImageInverted = invertImage(bitmapImage, infoHeader.biWidth, infoHeader.biHeight);
        free(bitmapImage);
        imageData->bitmapImage = bitmapImageInverted;
    }else{
        imageData->bitmapImage = bitmapImage;
    }
    
    imageData->biWidth = infoHeader.biWidth;
    imageData->biHeight = infoHeader.biHeight;
    imageData->biSize = infoHeader.biWidth * infoHeader.biHeight;
    imageData->bfOffBits = fileHeader.bfOffBits;

    //close file and return bitmap iamge data
    fclose(fp);
    return imageData; 
}

uint8_t * invertImage(uint8_t *bitmapImage, uint32_t width, uint32_t heigth){
    uint8_t * bitmapImageInverted = (uint8_t*) malloc(width * heigth);

    int total = width*heigth;
    for(uint32_t i=0; i<width; i++){
        for(uint32_t j=0; j<heigth; j++){
            bitmapImageInverted[i*width+j] = bitmapImage[total-width*(i+1) + j];
        }
    }

    return bitmapImageInverted;
}

int updateImageData(char *path, IMAGEDATA * imageData, int flip){
    uint8_t * bitmapImageInverted;
    if(flip){
        bitmapImageInverted = invertImage(imageData->bitmapImage, imageData->biWidth, imageData->biHeight);
    }else{
        bitmapImageInverted = imageData->bitmapImage;
    }
    FILE* fp;
    fp = fopen(path, "rb+");
    fseek(fp, imageData->bfOffBits, SEEK_SET);
    fwrite(bitmapImageInverted, 1, imageData->biWidth * imageData->biHeight, fp);
    fclose(fp);
    if(flip){
        free(bitmapImageInverted);
    }
    return 0;
}

uint8_t * calculateOfuscatedValues(uint8_t w, uint8_t v,  uint8_t u, uint8_t y ){
    uint8_t * newValues = malloc(3*sizeof(uint8_t));
    uint8_t parity = 0;
    for (uint8_t i = 0; i < 8;i++){
        parity = parity ^ ((y >> i) & 1);                                                          
    } 

    uint8_t aux = (y >> 5) & 7;
    newValues[0] = (w & 248) ^ aux;

    aux = (y >> 2) & 7;
    newValues[1] = (v & 248) ^ aux;
    
    aux = ((parity << 2) ^ (y & 3)) & 7;
    newValues[2] = (u & 248) ^ aux;

    return newValues;
}

uint8_t reconstructY(uint8_t w, uint8_t v,  uint8_t u){

    uint8_t y = 0;
    uint8_t mask = 7;
    y = y ^ ((w & mask) << 5);
    y = y ^ ((v & mask) << 2);
    y = y ^ (u&3);

    return y;
}

int createImage(char * inputPath, IMAGEDATA * input_image_data, char * outputPath ,uint8_t* bitmapImage){
    uint8_t header[input_image_data->bfOffBits];
    FILE* fp_read;    
    fp_read = fopen(inputPath, "rb");//Read the image.bmp file in the same directory.
    fread(header, 1, input_image_data->bfOffBits, fp_read);
    FILE* fp_write;    
    fp_write = fopen(outputPath, "wb");//Read the image.bmp file in the same directory.
    if(!fp_write){
        fprintf(stderr,"El directorio no existe\n");
        exit(EXIT_FAILURE);
    }
    fwrite(header, 1,input_image_data->bfOffBits,fp_write);
    IMAGEDATA image_data = {
        bitmapImage,
        input_image_data->biWidth,         
        input_image_data->biHeight,       
        input_image_data->biSize,
        input_image_data->bfOffBits
    };

    //close file 
    fclose(fp_read);
    fclose(fp_write);
    updateImageData(outputPath, &image_data, 0);

    
    return 0;
}
