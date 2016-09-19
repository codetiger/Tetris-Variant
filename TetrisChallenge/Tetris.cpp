//
//  Tetris.cpp
//  Tetris
//
//  Created by Harishankar on 15/09/16.
//  Copyright © 2016 Harishankar. All rights reserved.
//

#include "Tetris.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#define totalBlockTypes 8
#define GunBlock 7


Tetris::Tetris(int row, int col) {
    gameState = GAMESTATE_OVER;
    rowMax = row;
    colMax = col;
    initializeBlockTypes();
    
    int boardSize = row * col;
    boardLayout = (int*)malloc(boardSize * sizeof(int));
    memset(boardLayout, 0, boardSize * sizeof(int));
}

void Tetris::startNewGame() {
    int boardSize = rowMax * colMax;
    memset(boardLayout, 0, boardSize * sizeof(int));
    srand((unsigned int)time(0));
    createNewBlock();
    level = 1;
    score = 0;
    rowsRemovedInLevel = 0;
    gameState = GAMESTATE_PLAYING;
}

void Tetris::update() {
    if(canMove(posRow+1, posCol)) {
        posRow += 1;
        checkColFull();
    } else {
        pushBlock2Layout();
        createNewBlock();
    }
    
    if(rowsRemovedInLevel == 5) {
        level++;
        rowsRemovedInLevel = 0;
    }
    
    checkGameOver();
}

void Tetris::checkGameOver() {
    bool isGameOver = false;
    for (int c = 0; c < colMax; c++) {
        if(boardLayout[c] != 0) {
            isGameOver = true;
            break;
        }
    }

    if(isGameOver)
        gameState = GAMESTATE_OVER;
}

void Tetris::moveRight() {
    if(canMove(posRow, posCol+1))
        posCol++;
}

void Tetris::moveLeft() {
    if(canMove(posRow, posCol-1))
        posCol--;
}

void Tetris::moveDown() {
    while (canMove(posRow+1, posCol)) {
        posRow++;
    }
    pushBlock2Layout();
    createNewBlock();
}

void Tetris::rotate() {
    if(currentBlockType != GunBlock) {
        blockTypes[currentBlockType].rotateCW();
    } else {
        shootAt(posCol + 1);
    }
}

void Tetris::shootAt(int col) {
    for (int r = rowMax - 1; r >= 0; r--) {
        if(boardLayout[r * colMax + col] == 0) {
            boardLayout[r * colMax + col] = GunBlock;
            return;
        }
    }
}

void Tetris::pushBlock2Layout() {
    if(currentBlockType != GunBlock) {
        Matrix blockMatrix = blockTypes[currentBlockType].getMatrix();
        
        for (int r = 0; r < blockMatrix.size; r++) {
            for (int c = 0; c < blockMatrix.size; c++) {
                if(c + posCol >= 0 && c + posCol < colMax && r + posRow < rowMax && boardLayout[(r + posRow) * colMax + (c + posCol)] == 0) {
                    boardLayout[(r + posRow) * colMax + (c + posCol)] = blockMatrix.cells[r * blockMatrix.size + c];
                }
            }
        }
    }
}

void Tetris::removeRow(int row) {
    for (int r = row - 1; r >= 0; r--) {
        for (int c = 0; c < colMax; c++) {
            boardLayout[(r + 1) * colMax + c] = boardLayout[r * colMax + c];
        }
    }
}

void Tetris::checkColFull() {
    int rowsRemovedInSingleBlock = 0;
    for (int r = rowMax - 1; r >= 0; r--) {
        bool isFull = true;
        for (int c = 0; c < colMax; c++) {
            if(boardLayout[r * colMax + c] == 0) {
                isFull = false;
            }
        }
        if(isFull) {
            removeRow(r);
            r++;
            rowsRemovedInSingleBlock++;
        }
    }
    
    if(rowsRemovedInSingleBlock > 0) {
        rowsRemovedInLevel += rowsRemovedInSingleBlock;
        score += rowsRemovedInSingleBlock * rowsRemovedInSingleBlock * level * 2;
    }
}

void Tetris::createNewBlock() {
    posCol = (colMax / 2) - 2;
    posRow = 0;
    currentBlockType = (rand() % totalBlockTypes);
}

bool Tetris::canMove(int newPosRow, int newPosCol) {
    Matrix blockMatrix = blockTypes[currentBlockType].getMatrix();
    
    for (int r = 0; r < blockMatrix.size; r++) {
        for (int c = 0; c < blockMatrix.size; c++) {
            if(c + newPosCol >= 0 && c + newPosCol < colMax && r + newPosRow < rowMax) {
                if(blockMatrix.cells[r * blockMatrix.size + c] > 0 && boardLayout[(r + newPosRow) * colMax + (c + newPosCol)] > 0)
                    return false;
            } else {
                if(blockMatrix.cells[r * blockMatrix.size + c] > 0)
                    return false;
            }
        }
    }
    
    return true;
}

bool Tetris::canRotate(bool ccw) {
    Blocks block = blockTypes[currentBlockType];
    
    if(ccw)
        block.rotateCCW();
    else
        block.rotateCW();
    
    Matrix blockMatrix = block.getMatrix();

    for (int r = 0; r < blockMatrix.size; r++) {
        for (int c = 0; c < blockMatrix.size; c++) {
            if(c + posCol >= 0 && c + posCol < colMax) {
                if(blockMatrix.cells[r * blockMatrix.size + c] > 0 && boardLayout[(r + posRow) * colMax + (c + posCol)] > 0)
                    return false;
            } else {
                if(blockMatrix.cells[r * blockMatrix.size + c] > 0)
                    return false;
            }
        }
    }
    
    return true;
}

void Tetris::print() {
    Matrix blockMatrix = blockTypes[currentBlockType].getMatrix();

    printf("Board Layout\n");
    for (int r = 0; r < rowMax; r++) {
        for (int c = 0; c < colMax; c++) {
            if(r >= posRow && r < posRow + blockMatrix.size && c >= posCol && c < posCol + blockMatrix.size)
                printf("%d ", blockMatrix.cells[(r - posRow) * blockMatrix.size + (c - posCol)]);
            else
                printf("%d ", boardLayout[r * colMax + c]);
        }
        printf("\n");
    }
}

void Tetris::getBoardLayout(int* board) {
    memcpy(board, boardLayout, rowMax * colMax * sizeof(int));

    Matrix blockMatrix = blockTypes[currentBlockType].getMatrix();
    
    for (int r = 0; r < blockMatrix.size; r++) {
        for (int c = 0; c < blockMatrix.size; c++) {
            if(c + posCol >= 0 && c + posCol < colMax && r + posRow < rowMax && board[(r + posRow) * colMax + (c + posCol)] == 0) {
                board[(r + posRow) * colMax + (c + posCol)] = blockMatrix.cells[r * blockMatrix.size + c];
            }
        }
    }
}

void Tetris::initializeBlockTypes() {
    blockTypes = new Blocks[totalBlockTypes];
    
    int cell1[] = {
        0, 0, 0, 0,
        1, 1, 1, 1,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    blockTypes[0] = Blocks(4, cell1);
    
    int cell2[] = {
        2, 0, 0,
        2, 2, 2,
        0, 0, 0
    };
    blockTypes[1] = Blocks(3, cell2);
    
    int cell3[] = {
        0, 0, 3,
        3, 3, 3,
        0, 0, 0
    };
    blockTypes[2] = Blocks(3, cell3);
    
    int cell4[] = {
        4, 4,
        4, 4
    };
    blockTypes[3] = Blocks(2, cell4);
    
    int cell5[] = {
        0, 5, 5,
        5, 5, 0,
        0, 0, 0
    };
    blockTypes[4] = Blocks(3, cell5);
    
    int cell6[] = {
        0, 6, 0,
        6, 6, 6,
        0, 0, 0
    };
    blockTypes[5] = Blocks(3, cell6);
    
    int cell7[] = {
        7, 7, 0,
        0, 7, 7,
        0, 0, 0
    };
    blockTypes[6] = Blocks(3, cell7);

    // Not a standard Block Type, This is a gun and rotating the block will shoot a miniblock in the gap below.
    int cell8[] = {
        8, 8, 8,
        8, 8, 8,
        0, 8, 0
    };
    blockTypes[7] = Blocks(3, cell8);
}

