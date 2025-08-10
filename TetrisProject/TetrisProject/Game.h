#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Tetromino.h"

class Game {
public:
    Game();
    void Update(float dt);
    void Draw();

private:
    Board board;
    Tetromino current;
    Tetromino next;
    float fallTimer;
    float fallInterval;
    bool gameOver;

    void SpawnTetromino();
    void LockPiece();
};

#endif