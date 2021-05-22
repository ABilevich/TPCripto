#ifndef IMG_H
#define IMG_H

// referencia: https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure

typedef struct tagBITMAPFILEHEADER 
{
    //unsigned short bfType; // 19778, must be a BM string, the corresponding hexadecimal is 0x4d42, and the decimal is 19778, otherwise it is not a bmp format file
    unsigned int   bfSize;        // File size in bytes (2-5 bytes)
    unsigned short bfReserved1;   // Reserved, must be set to 0 (6-7 bytes)
    unsigned short bfReserved2;   // Reserved, must be set to 0 (8-9 bytes)
    unsigned int   bfOffBits;     // Offset from file header to pixel data (10-13 bytes)
} BITMAPFILEHEADER;

//Image information header structure
typedef struct tagBITMAPINFOHEADER 
{
    unsigned int    biSize;          // The size of this structure (14-17 bytes)
    unsigned int    biWidth;         // The width of the image (18-21 bytes)
    unsigned int    biHeight;        // The height of the image (22-25 bytes)
    unsigned short  biPlanes;        // Indicates the plane attribute of the bmp picture, obviously the display has only one plane, so it is always equal to 1 (26-27 bytes)
    unsigned short  biBitCount;      // The number of bits occupied by a pixel, generally 24 (28-29 bytes)
    unsigned int    biCompression;   // Describe the type of image data compression, 0 means no compression. (30-33 bytes)
    unsigned int    biSizeImage;     // The size of pixel data, this value should be equal to bfSize-bfOffBits (34-37 bytes) in the file header structure above
    unsigned int    biXPelsPerMeter; // Describe the horizontal resolution, expressed in pixels/meter. Generally 0 (38-41 bytes)
    unsigned int    biYPelsPerMeter; // Describe the vertical resolution, expressed in pixels/meter. Generally 0 (42-45 bytes)
    unsigned int    biClrUsed;       // Describe the number of color indexes in the color table actually used by the bitmap (if set to 0, all palette items are used). (46-49 bytes)
    unsigned int    biClrImportant;  // Explain the number of color indexes that have an important influence on the image display. If it is 0, it means all are important. (50-53 bytes)
} BITMAPINFOHEADER;

unsigned char * analizeImage(char *path);

#endif