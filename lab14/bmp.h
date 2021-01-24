#ifndef GAMEOFLIFE_BMP_H
#define GAMEOFLIFE_BMP_H
#pragma pack(2)

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;


typedef struct {
    unsigned dead: 1;
} POINT;

typedef struct {
    unsigned int width;
    unsigned int height;
    POINT **data;
} MATRIX;

typedef struct {
    WORD bfType;
    DWORD bfSize;
    DWORD bfReserved;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFO;

typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFO info;
    unsigned char colorTable[8];
    MATRIX matrix;
} GAME_FRAME;

GAME_FRAME* getFrame(const char *);

void writeFrame(GAME_FRAME *, const char *);

MATRIX newMatrix(unsigned int, unsigned int);

void deleteMatrix(MATRIX *);

void copyMatrix(MATRIX *, MATRIX *);

int isEqualMatrices(MATRIX *, MATRIX *);

#endif //GAMEOFLIFE_BMP_H