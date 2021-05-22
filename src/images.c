#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/images.h"


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

unsigned char * analizeImage(char *path){

	printf("path is %s\n", path);

    unsigned char *bitmapImage;  //store image data
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
    unsigned short  fileType;   
    fread(&fileType,1,sizeof (unsigned short), fp);  
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
    bitmapImage = (unsigned char*) malloc(infoHeader.biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(fp);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,1,infoHeader.biSizeImage,fp);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(fp);
        return NULL;
    }

//////////////////////////////////////////


	for(int i =0; i< (int) (infoHeader.biWidth * infoHeader.biHeight); i++){
		if(i%infoHeader.biWidth == 0){
			printf("\n");
		}
		printf("[%x]", *(bitmapImage+i));
	}

/////////////////////////////////////////

    //close file and return bitmap iamge data
    fclose(fp);
    return bitmapImage; 
}
