//
//  GameScene.m
//  TetrisChallenge
//
//  Created by Harishankar on 15/09/16.
//  Copyright © 2016 Harishankar. All rights reserved.
//

#import "GameScene.h"
#include "Tetris.h"

#define rowCount 20
#define colCount 10


@implementation GameScene {
    Tetris *tetris;
    int *boardLayout;
    SKShapeNode *_boardNodes[rowCount][colCount];
    UIColor *_colorArray[9];
    float cummulativeTime;
    int bestScore;
    
    SKLabelNode *_scoreLabel;
    SKLabelNode *_bestScoreLabel;
    SKLabelNode *_levelLabel;
    SKNode *_hintsViewNode;
}

- (void)didMoveToView:(SKView *)view {
    boardLayout = (int*)malloc(rowCount * colCount * sizeof(int));
    
    _colorArray[0] = [UIColor clearColor];
    _colorArray[1] = [UIColor redColor];
    _colorArray[2] = [UIColor yellowColor];
    _colorArray[3] = [UIColor magentaColor];
    _colorArray[4] = [UIColor blueColor];
    _colorArray[5] = [UIColor cyanColor];
    _colorArray[6] = [UIColor greenColor];
    _colorArray[7] = [UIColor orangeColor];
    _colorArray[8] = [UIColor grayColor];
    
    int blockWidth = self.size.width / colCount;
    int blockHeight = self.size.height / rowCount;

    CGFloat w = (self.size.width + self.size.height) * 0.02;
    
    SKShapeNode *_masterCopyNode = [SKShapeNode shapeNodeWithRectOfSize:CGSizeMake(blockWidth - 2, blockHeight - 2) cornerRadius:w * 0.2];
    _masterCopyNode.lineWidth = 2.5;
    _masterCopyNode.strokeColor = [UIColor darkGrayColor];
    
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            _boardNodes[r][c] = [_masterCopyNode copy];
            _boardNodes[r][c].position = CGPointMake(c * blockWidth + blockWidth / 2, r * blockHeight + blockHeight / 2);
            [self addChild:_boardNodes[r][c]];
        }
    }
    
    tetris = new Tetris(rowCount, colCount);
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapGestureAction:)];
    tapGesture.delegate = self;
    [self.view addGestureRecognizer:tapGesture];
    
    UISwipeGestureRecognizer *swipeGestureLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureActionLeft:)];
    swipeGestureLeft.delegate = self;
    swipeGestureLeft.direction = UISwipeGestureRecognizerDirectionLeft;
    [self.view addGestureRecognizer:swipeGestureLeft];

    UISwipeGestureRecognizer *swipeGestureRight = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureActionRight:)];
    swipeGestureRight.delegate = self;
    swipeGestureRight.direction = UISwipeGestureRecognizerDirectionRight;
    [self.view addGestureRecognizer:swipeGestureRight];

    UISwipeGestureRecognizer *swipeGestureDown = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureActionDown:)];
    swipeGestureDown.delegate = self;
    swipeGestureDown.direction = UISwipeGestureRecognizerDirectionDown;
    [self.view addGestureRecognizer:swipeGestureDown];
    
    _scoreLabel = (SKLabelNode *)[self childNodeWithName:@"//scoreLabel"];
    _bestScoreLabel = (SKLabelNode *)[self childNodeWithName:@"//bestScoreLabel"];
    _levelLabel = (SKLabelNode *)[self childNodeWithName:@"//levelLabel"];
    _hintsViewNode = (SKNode *)[self childNodeWithName:@"//hintsView"];

    bestScore = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"bestScore"];
    _bestScoreLabel.text = [NSString stringWithFormat:@"Best: %d", bestScore];

    // TODO: Find the right solution & remove this hack
    // Brings Nodes to Front
    [_hintsViewNode removeFromParent];
    [self addChild:_hintsViewNode];
    [_bestScoreLabel removeFromParent];
    [self addChild:_bestScoreLabel];
    [_hintsViewNode removeFromParent];
    [self addChild:_hintsViewNode];
    [_levelLabel removeFromParent];
    [self addChild:_levelLabel];
}

-(void)resetGame {
    tetris->startNewGame();
    [_hintsViewNode removeFromParent];
    bestScore = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"bestScore"];
    _bestScoreLabel.text = [NSString stringWithFormat:@"Best: %d", bestScore];
}

-(void)update:(CFTimeInterval)currentTime {
    if(tetris->gameState != GAMESTATE_PLAYING) {
        if(!_hintsViewNode.parent)
            [self addChild:_hintsViewNode];
        return;
    }
    
    cummulativeTime += (currentTime / 1000000);
    
    float speed = 2.0 - tetris->level * 0.25;
    if(cummulativeTime > speed) {
        cummulativeTime -= speed;
        tetris->update();
        tetris->getBoardLayout(boardLayout);
        for (int r = 0; r < rowCount; r++) {
            for (int c = 0; c < colCount; c++) {
                _boardNodes[rowCount - r - 1][c].fillColor = _colorArray[boardLayout[r * colCount + c]];
            }
        }
        _scoreLabel.text = [NSString stringWithFormat:@"Score: %d", tetris->score];
        _levelLabel.text = [NSString stringWithFormat:@"Level: %d", tetris->level];
        if(bestScore < tetris->score) {
            bestScore = tetris->score;
            _bestScoreLabel.text = [NSString stringWithFormat:@"Best: %d", bestScore];
            [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithInt:bestScore] forKey:@"bestScore"];
            [[NSUserDefaults standardUserDefaults] synchronize];
        }
    }
}

-(void)tapGestureAction: (UIPanGestureRecognizer *)gesture {
    if(tetris->gameState != GAMESTATE_PLAYING)
        [self resetGame];
    else
        tetris->rotate();
}

-(void)swipeGestureActionLeft: (UISwipeGestureRecognizer *)gesture {
    if(tetris->gameState != GAMESTATE_PLAYING)
        return;
    
    tetris->moveLeft();
}

-(void)swipeGestureActionDown: (UISwipeGestureRecognizer *)gesture {
    if(tetris->gameState != GAMESTATE_PLAYING)
        return;
    
    tetris->moveDown();
}

-(void)swipeGestureActionRight: (UISwipeGestureRecognizer *)gesture {
    if(tetris->gameState != GAMESTATE_PLAYING)
        return;
    
    tetris->moveRight();
}


@end
