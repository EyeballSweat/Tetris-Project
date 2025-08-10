#include "Board.h"
#include "raylib.h"

static const int BOARD_X = 100;
static const int BOARD_Y = 50;

Board::Board() {
    for (int y = 0; y < BOARD_HEIGHT; y++)
        for (int x = 0; x < BOARD_WIDTH; x++)
            grid[y][x] = 0;
}

bool Board::IsCollision(const Tetromino& tetro, int x, int y) const {
    const auto& shape = tetro.GetShape();
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape.cells[row][col]) {
                int boardX = x + col;
                int boardY = y + row;
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT)
                    return true;
                if (boardY >= 0 && grid[boardY][boardX] != 0)
                    return true;
            }
        }
    }
    return false;
}

void Board::PlaceTetromino(const Tetromino& tetro) {
    const auto& shape = tetro.GetShape();
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape.cells[row][col]) {
                int boardX = tetro.GetX() + col;
                int boardY = tetro.GetY() + row;
                if (boardY >= 0 && boardX >= 0 && boardX < BOARD_WIDTH && boardY < BOARD_HEIGHT)
                    grid[boardY][boardX] = 1; // simple color/index
            }
        }
    }
}

int Board::ClearFullLines() {
    int cleared = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (grid[y][x] == 0) { full = false; break; }
        }
        if (full) {
            for (int row = y; row > 0; row--) {
                for (int col = 0; col < BOARD_WIDTH; col++)
                    grid[row][col] = grid[row - 1][col];
            }
            for (int col = 0; col < BOARD_WIDTH; col++) grid[0][col] = 0;
            cleared++;
            y++; // recheck this row after shift
        }
    }
    return cleared;
}

void Board::Draw(int cellSize) const {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int screenX = BOARD_X + x * cellSize;
            int screenY = BOARD_Y + y * cellSize;
            if (grid[y][x]) {
                DrawRectangle(screenX, screenY, cellSize - 1, cellSize - 1, BLUE);
            }
            else {
                DrawRectangleLines(screenX, screenY, cellSize, cellSize, DARKGRAY);
            }
        }
    }
}