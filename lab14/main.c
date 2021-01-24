#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "bmp.h"

#pragma pack(2)

unsigned int countOfAliveNeighbours(MATRIX *mat, int x, int y) {
    unsigned int count = 0;
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            if ((i == x && j == y) || (i < 0 || j < 0) || (i >= mat->height || j >= mat->width)) {
                continue;
            }
            count += !mat->data[i][j].dead;
        }
    }
    return count;
}

void nextGeneration(MATRIX *prevWorld) {
    MATRIX newWorld = newMatrix(prevWorld->width, prevWorld->height);
    unsigned int count;
    for (int i = 0; i < prevWorld->height; ++i) {
        for (int j = 0; j < prevWorld->width; ++j) {
            count = countOfAliveNeighbours(prevWorld, i, j);
            newWorld.data[i][j].dead = (prevWorld->data[i][j].dead ? (count != 3) : (count != 2 && count != 3));
        }
    }
    *prevWorld = newWorld;
}

int main(int argc, const char *argv[]) {
    const char *inputFile, *dirName;
    int maxIter = 9999, dumpFreq = 1;
    if (argc < 3) {
        printf("Error: input file file and output directory are required");
        return -1;
    }
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--input")) {
            inputFile = argv[++i];
        } else if (!strcmp(argv[i], "--output")) {
            dirName = argv[++i];
        } else if (!strcmp(argv[i], "--max_iter")) {
            maxIter = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--dump_freq")) {
            dumpFreq = atoi(argv[++i]);
        }
    }
    mkdir(dirName);
    GAME_FRAME* frame = getFrame(inputFile);
    if (frame == NULL){
        printf("Error: image is not valid");
        return -1;
    }
    MATRIX last = newMatrix(frame->matrix.width, frame->matrix.height);
    MATRIX temp = newMatrix(frame->matrix.width, frame->matrix.height);
    char fname[100];
    for (int i = 0; i < maxIter; ++i) {
        sprintf(fname, "%s%c%i%i%i%i%s", dirName, '/', i / 1000, i / 100 % 10, i / 10 % 10, i % 10, ".bmp");
        printf("%s\n", fname);
        copyMatrix(&frame->matrix, &temp);
        nextGeneration(&frame->matrix);
        if (i % dumpFreq == 0) {
            writeFrame(frame, fname);
        }
        copyMatrix(&temp, &last);
        if (isEqualMatrices(&frame->matrix, &last)) {
            printf("END");
            break;
        }
    }
    deleteMatrix(&frame->matrix);
    deleteMatrix(&temp);
    deleteMatrix(&last);
    return 0;
}