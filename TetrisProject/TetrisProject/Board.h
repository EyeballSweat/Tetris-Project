#ifndef BOARD_H
#define BOARD_H

#include "Tetromino.h"

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

class Board {
public:
    Board();
    bool IsCollision(const Tetromino& tetro, int x, int y) const;
    void PlaceTetromino(const Tetromino& tetro);
    int ClearFullLines();
    void Draw(int cellSize) const;

private:
    int grid[BOARD_HEIGHT][BOARD_WIDTH];
};

#endif