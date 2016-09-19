//
//  Tetris.hpp
//  Tetris
//
//  Created by Harishankar on 15/09/16.
//  Copyright © 2016 Harishankar. All rights reserved.
//

#ifndef Tetris_hpp
#define Tetris_hpp

#include <stdio.h>
#include "Blocks.h"

enum GAMESTATE {
    GAMESTATE_PLAYING,
    GAMESTATE_OVER,
};

class Tetris {
private:
    
    int *boardLayout;
    Blocks *blockTypes;
    
    int currentBlockType;
    int posRow, posCol;

    int rowsRemovedInLevel;
    
    void initializeBlockTypes();
    bool canMove(int newPosRow, int newPosCol);
    bool canRotate(bool ccw);
    void pushBlock2Layout();
    void createNewBlock();
    void checkColFull();
    void removeRow(int row);
    void checkGameOver();
    void shootAt(int col);
    
public:
    GAMESTATE gameState;
    int level, score;
    int rowMax, colMax;
    
    Tetris(int row, int col);
    
    void startNewGame();
    void update();
    void rotate();
    void moveRight();
    void moveLeft();
    void moveDown();
    
    void getBoardLayout(int* board);
    void print();
};


#endif /* Tetris_hpp */
