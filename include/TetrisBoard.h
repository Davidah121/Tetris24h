#pragma once
#include "TetrisBlock.h"

struct BoardPiece
{
    bool filled = false;
    Image* img = nullptr;
};

class TetrisBoard : public ParentGameObject
{
public:
    TetrisBoard();
    ~TetrisBoard();

    bool checkCollision(TetrisBlock b);

    void update();
    void render();

    void addPiece(TetrisBlock b);

    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    bool getFailed();

private:
    void clearLines();
    void checkIfFailed();

    BoardPiece boardArray[240];
    bool failed = false;

    int x = 0;
    int y = 0;
};