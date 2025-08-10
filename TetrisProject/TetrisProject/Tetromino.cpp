#include "Tetromino.h"
#include "raylib.h"

Tetromino::Tetromino(TetrominoType t) : type(t), rotationIndex(0), posX(3), posY(0) {
    InitShapes();
}

void Tetromino::InitShapes() {
    rotations.clear();
    ShapeRotation rot{};
    switch (type) {
    case I:
        rotations = {
            {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}},
            {{{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}}}
        };
        break;
    case O:
        rotations = {
            {{{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}}
        };
        break;
    case T:
        rotations = {
            {{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}},
            {{{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}},
            {{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}}},
            {{{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}}
        };
        break;
    case S:
        rotations = {
            {{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}},
            {{{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}}
        };
        break;
    case Z:
        rotations = {
            {{{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}},
            {{{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}}
        };
        break;
    case J:
        rotations = {
            {{{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}},
            {{{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}},
            {{{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}}},
            {{{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}}
        };
        break;
    case L:
        rotations = {
            {{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}},
            {{{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}},
            {{{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}}},
            {{{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}}
        };
        break;
    }
}

void Tetromino::RotateCW() {
    rotationIndex = (rotationIndex + 1) % rotations.size();
}

void Tetromino::Move(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Tetromino::SetPosition(int x, int y) {
    posX = x;
    posY = y;
}

int Tetromino::GetX() const { return posX; }
int Tetromino::GetY() const { return posY; }

const ShapeRotation& Tetromino::GetShape() const {
    return rotations[rotationIndex];
}

void Tetromino::Draw(int cellSize) const {
    const auto& shape = GetShape();
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape.cells[row][col]) {
                DrawRectangle((posX + col) * cellSize + 100,
                    (posY + row) * cellSize + 50,
                    cellSize - 1, cellSize - 1, RED);
            }
        }
    }
}