#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Tetromino.h"
#include <string>

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

    // scoring / level
    int score;
    int linesCleared;
    int level;

    void SpawnTetromino();
    void LockPiece(int dropDistance = 0);
    void IncreaseLevelIfNeeded();
    int PointsForClear(int cleared) const;
    void HardDrop();
    void DrawControls() const;
};

#endif