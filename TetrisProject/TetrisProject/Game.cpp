#include "Game.h"
#include "raylib.h"

Game::Game()
    : current(static_cast<TetrominoType>(GetRandomValue(0, 6))),
    next(static_cast<TetrominoType>(GetRandomValue(0, 6))),
    fallTimer(0.0f),
    fallInterval(0.5f),
    gameOver(false) {}

void Game::Update(float dt) {
    if (gameOver) return;

    // Move piece left/right
    if (IsKeyPressed(KEY_LEFT)) {
        if (!board.IsCollision(current, current.GetX() - 1, current.GetY()))
            current.Move(-1, 0);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        if (!board.IsCollision(current, current.GetX() + 1, current.GetY()))
            current.Move(1, 0);
    }

    // Rotate
    if (IsKeyPressed(KEY_UP)) {
        Tetromino rotated = current;
        rotated.RotateCW();
        if (!board.IsCollision(rotated, rotated.GetX(), rotated.GetY()))
            current.RotateCW();
    }

    // Soft drop
    if (IsKeyDown(KEY_DOWN)) {
        fallTimer += dt * 10; // faster drop
    }
    else {
        fallTimer += dt;
    }

    // Automatic fall
    if (fallTimer >= fallInterval) {
        fallTimer = 0.0f;
        if (!board.IsCollision(current, current.GetX(), current.GetY() + 1)) {
            current.Move(0, 1);
        }
        else {
            LockPiece();
        }
    }
}

void Game::Draw() {
    board.Draw(24);
    current.Draw(24);
    if (gameOver) {
        DrawText("GAME OVER", 300, 250, 40, RED);
    }
}

void Game::SpawnTetromino() {
    current = next;
    current.SetPosition(3, 0);
    next = Tetromino(static_cast<TetrominoType>(GetRandomValue(0, 6)));
    if (board.IsCollision(current, current.GetX(), current.GetY())) {
        gameOver = true;
    }
}

void Game::LockPiece() {
    board.PlaceTetromino(current);
    board.ClearFullLines();
    SpawnTetromino();
}