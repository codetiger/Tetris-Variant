//
//  Matrix.hpp
//  Tetris
//
//  Created by Harishankar on 15/09/16.
//  Copyright © 2016 Harishankar. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <stdio.h>
#include <stdlib.h>

struct Matrix {
    unsigned int size;
    int *cells;

    Matrix() {
        this->size = 0;
    }

    Matrix(int size, int *cellArray) {
        this->size = size;
        cells = (int*)malloc(size * size * sizeof(int));

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                cells[r * size + c] = cellArray[r * size + c];
            }
        }
    }
    
    // Clockwise rotation of Matrix
    void rotate() {
        //Transpose
        for (int r = 0; r < size; r++) {
            for (int c = r; c < size; c++) {
                int temp = cells[r * size + c];
                cells[r * size + c] = cells[c * size + r];
                cells[c * size + r] = temp;
            }
        }

        //Reverse elements on row order
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size/2; c++) {
                int temp = cells[r * size + c];
                cells[r * size + c] = cells[r * size + (size-c-1)];
                cells[r * size + (size-c-1)] = temp;
            }
        }
    }
    
    void print() {
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                printf("%d ", cells[r * size + c]);
            }
            printf("\n");
        }
        printf("\n");
    }
};

#endif /* Matrix5_hpp */
