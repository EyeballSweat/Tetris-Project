#include "Game.h"
#include "raylib.h"

static const int CELL_SIZE = 24;
static const int BOARD_X = 100;
static const int BOARD_Y = 50;
const Color RAYGRAY = { 150, 150, 150, 255 }; // RGB + Alpha

Game::Game()
    : current(static_cast<TetrominoType>(GetRandomValue(0, 6))),
    next(static_cast<TetrominoType>(GetRandomValue(0, 6))),
    fallTimer(0.0f),
    fallInterval(0.5f),
    gameOver(false),
    score(0),
    linesCleared(0),
    level(0)
{
    current.SetPosition(3, 0);
}

void Game::Update(float dt) {
    if (gameOver) {
        if (IsKeyPressed(KEY_ENTER)) { // restart
            board = Board();
            score = 0; linesCleared = 0; level = 0;
            fallInterval = 0.5f;
            current = Tetromino(static_cast<TetrominoType>(GetRandomValue(0, 6)));
            next = Tetromino(static_cast<TetrominoType>(GetRandomValue(0, 6)));
            current.SetPosition(3, 0);
            gameOver = false;
        }
        return;
    }

    // horizontal movement
    if (IsKeyPressed(KEY_LEFT)) {
        if (!board.IsCollision(current, current.GetX() - 1, current.GetY()))
            current.Move(-1, 0);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        if (!board.IsCollision(current, current.GetX() + 1, current.GetY()))
            current.Move(1, 0);
    }

    // rotate
    if (IsKeyPressed(KEY_UP)) {
        Tetromino test = current;
        test.RotateCW();
        if (!board.IsCollision(test, test.GetX(), test.GetY()))
            current.RotateCW();
    }

    // hard drop
    if (IsKeyPressed(KEY_SPACE)) {
        HardDrop();
        return; // piece locked inside HardDrop
    }

    // soft drop
    float dropMultiplier = IsKeyDown(KEY_DOWN) ? 10.0f : 1.0f;
    fallTimer += dt * dropMultiplier;

    // automatic fall
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
    // draw board
    board.Draw(CELL_SIZE);

    // draw current piece (using board origin)
    // Tetromino::Draw uses posX/posY relative to board origin; we draw board at offset in Board::Draw
    current.Draw(CELL_SIZE);

    // draw next preview area
    DrawText("Next:", BOARD_X + BOARD_WIDTH * CELL_SIZE + 30, BOARD_Y, 20, RAYWHITE);
    // draw next piece at a preview offset using DrawAt
    next.DrawAt(BOARD_X + BOARD_WIDTH * CELL_SIZE + 40, BOARD_Y + 30, CELL_SIZE);

    // score and stats
    DrawText(TextFormat("Score: %d", score), BOARD_X + BOARD_WIDTH * CELL_SIZE + 30, BOARD_Y + 140, 20, RAYWHITE);
    DrawText(TextFormat("Lines: %d", linesCleared), BOARD_X + BOARD_WIDTH * CELL_SIZE + 30, BOARD_Y + 170, 20, RAYWHITE);
    DrawText(TextFormat("Level: %d", level), BOARD_X + BOARD_WIDTH * CELL_SIZE + 30, BOARD_Y + 200, 20, RAYWHITE);

    if (gameOver) {
        DrawText("GAME OVER", 300, 250, 40, RED);
        DrawText("Press ENTER to restart", 280, 300, 20, RAYWHITE);
    }

    DrawControls();
}

void Game::DrawControls() const {
    int x = BOARD_X + BOARD_WIDTH * CELL_SIZE + 30;  // Position controls right of board
    int y = BOARD_Y + 240;

    DrawText("Controls:", x, y, 20, RAYWHITE);
    y += 30;
    DrawText("Left / Right Arrows: Move", x, y, 18, RAYGRAY);
    y += 25;
    DrawText("Up Arrow: Rotate", x, y, 18, RAYGRAY);
    y += 25;
    DrawText("Down Arrow: Soft Drop", x, y, 18, RAYGRAY);
    y += 25;
    DrawText("Space: Hard Drop", x, y, 18, RAYGRAY);
    y += 25;
    DrawText("Enter: Restart (game over)", x, y, 18, RAYGRAY);
}

void Game::SpawnTetromino() {
    current = next;
    current.SetPosition(3, 0);
    next = Tetromino(static_cast<TetrominoType>(GetRandomValue(0, 6)));
    if (board.IsCollision(current, current.GetX(), current.GetY())) {
        gameOver = true;
    }
}

void Game::LockPiece(int dropDistance) {
    board.PlaceTetromino(current);
    int cleared = board.ClearFullLines();
    if (cleared > 0) {
        score += PointsForClear(cleared);
        linesCleared += cleared;
        IncreaseLevelIfNeeded();
    }
    // reward for soft/hard drop distance (simple)
    if (dropDistance > 0) {
        score += dropDistance * 2;
    }
    SpawnTetromino();
}

void Game::IncreaseLevelIfNeeded() {
    int newLevel = linesCleared / 10;
    if (newLevel > level) {
        level = newLevel;
        // speed up: reduce interval but clamp lower bound
        fallInterval = 0.5f * pow(0.9f, level);
        if (fallInterval < 0.05f) fallInterval = 0.05f;
    }
}

int Game::PointsForClear(int cleared) const {
    // classic-ish scoring
    int base = 0;
    if (cleared == 1) base = 100;
    else if (cleared == 2) base = 300;
    else if (cleared == 3) base = 500;
    else if (cleared >= 4) base = 800;
    return base * (level + 1);
}

void Game::HardDrop() {
    // move piece down until collision, count distance
    int dist = 0;
    while (!board.IsCollision(current, current.GetX(), current.GetY() + 1)) {
        current.Move(0, 1);
        dist++;
    }
    // lock and reward points for drop distance
    LockPiece(dist);
}