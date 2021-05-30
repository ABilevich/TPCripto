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



void showBmpHead(BITMAPFILEHEADER pBmpHead) 
{  //Define the function of displaying information, pass in the file header structure
    printf("BMP file size: %dkb\n", pBmpHead.bfSize/1024);
    printf("Reserved words must be 0: %d\n",  pBmpHead.bfReserved1);
    printf("Reserved words must be 0: %d\n",  pBmpHead.bfReserved2);
    printf("Offset bytes of actual bitmap data: %d\n", pBmpHead.bfOffBits);
}

void showBmpInfoHead(BITMAPINFOHEADER pBmpinfoHead) 
{//Define the function to display the information, and the information header structure is passed in
   printf("Bitmap Information Header:\n" );   
   printf("The size of the message header: %d\n" ,pBmpinfoHead.biSize);   
   printf("Bitmap width: %d\n" ,pBmpinfoHead.biWidth);   
   printf("Bitmap height: %d\n" ,pBmpinfoHead.biHeight);   
   printf("The number of planes of the image (the number of planes is the number of palettes, the default is 1 palette): %d\n" ,pBmpinfoHead.biPlanes);   
   printf("Number of bits per pixel: %d\n" ,pBmpinfoHead.biBitCount);   
   printf("Compression method:%d\n" ,pBmpinfoHead.biCompression);   
   printf("Image size: %d\n" ,pBmpinfoHead.biSizeImage);   
   printf("Horizontal resolution: %d\n" ,pBmpinfoHead.biXPelsPerMeter);   
   printf("Vertical resolution: %d\n" ,pBmpinfoHead.biYPelsPerMeter);   
   printf("Number of colors used: %d\n" ,pBmpinfoHead.biClrUsed);   
   printf("Number of important colors: %d\n" ,pBmpinfoHead.biClrImportant);   
}

IMAGEDATA * analizeImage(char *path){

	printf("path is %s\n", path);

    uint8_t *bitmapImage;  //store image data
    // int imageIdx=0;  //image index counter
    // unsigned char tempRGB;  //our swap variable

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    FILE* fp;    
    fp = fopen(path, "rb");//Read the image.bmp file in the same directory.
    if(fp == NULL)
    {
        printf("Failed to open'image.bmp'!\n");
        exit(1);
    }
    //If you do not read the bifType first, according to the C language structure Sizeof operation rule-the whole is greater than the sum of the parts, resulting in misalignment of the read file
    uint16_t  fileType;   
    fread(&fileType,1,sizeof (uint16_t), fp);  
    if (fileType == 0x4d42)   
    {   
        printf("The file type identification is correct!" );  
        printf("\nFile identifier: %d\n", fileType); 
        fread(&fileHeader, 1, sizeof(BITMAPFILEHEADER), fp);
        showBmpHead(fileHeader);
        fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), fp);
        showBmpInfoHead(infoHeader);
        // fclose(fp);        
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

    imageData->bitmapImage = bitmapImage;
    imageData->biWidth = infoHeader.biWidth;
    imageData->biHeight = infoHeader.biHeight;
    imageData->biSize = infoHeader.biWidth * infoHeader.biHeight;
    imageData->bfOffBits = fileHeader.bfOffBits;
    
//////////////////////////////////////////

	// for(int i =0; i< (int) (imageData->biSize); i++){
	// 	if(i%infoHeader.biWidth == 0){
	// 		printf("\n");
	// 	}
	// 	printf("[%x]", *(bitmapImage+i));
	// }

    // for(int i =0; i< 600; i++){
	// 	if(i%infoHeader.biWidth == 0){
	// 		printf("\n");
	// 	}
	// 	printf("[%x]", *(bitmapImage+i));
	// }

/////////////////////////////////////////

    //close file and return bitmap iamge data
    fclose(fp);
    return imageData; 
}


int updateImageData(char *path, IMAGEDATA * imageData){
    //TODO: hacer
    FILE* fp;    
    fp = fopen(path, "rb+");//Read the image.bmp file in the same directory.
    // uint8_t asd[2] = {105,105};
    fseek(fp, imageData->bfOffBits, SEEK_SET);
    // fwrite(asd, 1, 2, fp);
    fwrite(imageData->bitmapImage, 1, imageData->biWidth * imageData->biHeight, fp);
    //close file 
    fclose(fp);
    return 0;
}

uint8_t * calculateOfuscatedValues(uint8_t w, uint8_t v,  uint8_t u, uint8_t y ){
    uint8_t * newValues = malloc(3*sizeof(uint8_t));
    uint8_t parity = 0;
    for (uint8_t i = 0; i < 8;i++){
        parity = parity ^ ((y >> i) & 1);                                                          
    } 
    // printf("y: %u, parity: %u\n",y,parity);

    //para w
    uint8_t aux = (y >> 5) & 7;
    newValues[0] = (w & 248) ^ aux;
    // printf("w was: %u, now is %u, with aux: %u, last 3 bits were: %u\n",w,newValues[0],aux,(w & 7));
    aux = (y >> 2) & 7;
    newValues[1] = (v & 248) ^ aux;
    // printf("v was: %u, now is %u, with aux: %u, last 3 bits were: %u\n",v,newValues[1],aux,(v & 7));
    aux = ((parity << 2) ^ (y & 3)) & 7;
    newValues[2] = (u & 248) ^ aux;
    // printf("u was: %u, now is %u, with aux: %u, last 3 bits were: %u\n",u,newValues[2],aux, (u & 7));
    return newValues;
}