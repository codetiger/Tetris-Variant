//
//  Blocks.hpp
//  Tetris
//
//  Created by Harishankar on 15/09/16.
//  Copyright © 2016 Harishankar. All rights reserved.
//

#ifndef Blocks_hpp
#define Blocks_hpp

#include <stdio.h>
#include "Matrix.h"

struct Blocks {
    unsigned int currentOrientation;
    Matrix *orientations;
    
    Blocks() {
        
    }
    
    Blocks(int size, int *cellArray) {
        currentOrientation = 0;
        orientations = new Matrix[4];
        
        orientations[0] = Matrix(size, cellArray);
        for(int i = 1; i < 4; i++) {
            orientations[i] = Matrix(size, cellArray);
            for(int j = 0; j < i; j++)
                orientations[i].rotate();
        }
    }
    
    void rotateCW() {
        currentOrientation++;
        if(currentOrientation == 4)
            currentOrientation = 0;
    }

    void rotateCCW() {
        currentOrientation--;
        if(currentOrientation == -1)
            currentOrientation = 3;
    }
    
    void print() {
        printf("Orientation: %d\n", currentOrientation);
        orientations[currentOrientation].print();
    }
    
    Matrix getMatrix() {
        return orientations[currentOrientation];
    }
    
    unsigned int getSize() {
        return orientations[currentOrientation].size;
    }
};

#endif /* Blocks_hpp */
