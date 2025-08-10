#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>

enum TetrominoType { I, O, T, S, Z, J, L };

struct ShapeRotation {
    int cells[4][4];
};

class Tetromino {
public:
    Tetromino(TetrominoType t);
    void RotateCW();
    void Move(int dx, int dy);
    void SetPosition(int x, int y);
    int GetX() const;
    int GetY() const;
    const ShapeRotation& GetShape() const;
    void Draw(int cellSize) const;              // draw on main board (uses hardcoded board origin)
    void DrawAt(int originX, int originY, int cellSize) const; // draw at arbitrary screen origin (preview)

private:
    TetrominoType type;
    std::vector<ShapeRotation> rotations;
    int rotationIndex;
    int posX, posY;
    void InitShapes();
};

#endif