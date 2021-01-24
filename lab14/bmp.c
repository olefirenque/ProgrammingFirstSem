#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include "bmp.h"


GAME_FRAME* getFrame(const char *fname) {
    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_SET);
    GAME_FRAME* object = malloc(sizeof(*object));

    fread(&object->fileHeader, sizeof(object->fileHeader), 1, file);
    fread(&object->info, sizeof(object->info), 1, file);
    fread(object->colorTable, sizeof(char), 8, file);

    fseek(file, object->fileHeader.bfOffBits, SEEK_SET);
    unsigned char c[object->info.biSizeImage];
    fread(c, sizeof(char), object->info.biSizeImage, file);
    object->matrix = newMatrix(object->info.biWidth, object->info.biHeight);
    for (unsigned int i = 0; i < object->matrix.height; ++i) {
        int offset = (int) (ceil((double) object->matrix.width / 32)) * 4;
        for (unsigned int j = 0; j < offset; ++j) {
            for (unsigned int k = 0; k + (j * 8) < object->matrix.width && k < 8; ++k) {
                object->matrix.data[i][k + j * 8].dead = (c[i * offset + j] & (1 << (7 - k))) >> (7 - k);
            }
        }
    }
    fclose(file);
    return object;
}

void writeFrame(GAME_FRAME *object, const char *name) {
    FILE *file = fopen(name, "wb");
    fseek(file, 0, SEEK_SET);
    fwrite(&object->fileHeader, sizeof(object->fileHeader), 1, file);
    fwrite(&object->info, sizeof(object->fileHeader), 1, file);
    fwrite(object->colorTable, sizeof(char), 8, file);

    fseek(file, object->fileHeader.bfOffBits, SEEK_SET);
    for (int i = 0; i < object->matrix.height; ++i) {
        for (int j = 0; j < (int) ceil((float) object->matrix.width / 32); ++j) {
            unsigned char nextBlock[4] = {0, 0, 0, 0};
            for (int k = 0; k < 4; ++k) {
                for (int b = 0; b + (k * 8) + j * 32 < object->matrix.width && b < 8; ++b) {
                    nextBlock[k] += object->matrix.data[i][b + k * 8 + j * 32].dead << (7 - b);
                }
            }
            fwrite(nextBlock, sizeof(unsigned char), 4, file);
        }
    }
    fclose(file);
}

MATRIX newMatrix(unsigned int x, unsigned int y) {
    MATRIX matrix;
    matrix.data = (POINT **) malloc(sizeof(POINT *) * y);
    for (int i = 0; i < y; ++i) {
        matrix.data[i] = (POINT *) malloc(sizeof(POINT) * x);
    }
    matrix.width = x;
    matrix.height = y;
    return matrix;
}

void deleteMatrix(MATRIX *mat) {
    free(mat->data);
}

void copyMatrix(MATRIX *src, MATRIX *dest) {
    for (unsigned int i = 0; i < src->height; ++i) {
        for (unsigned int j = 0; j < src->width; ++j) {
            dest->data[i][j].dead = src->data[i][j].dead;
        }
    }
}

int isEqualMatrices(MATRIX *mat1, MATRIX *mat2) {
    for (unsigned int i = 0; i < mat1->height; ++i) {
        for (unsigned int j = 0; j < mat1->width; ++j) {
            if (mat1->data[i][j].dead != mat2->data[i][j].dead) {
                return 0;
            }
        }
    }
    return 1;
}