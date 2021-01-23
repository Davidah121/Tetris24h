#pragma once
#include "TetrisBoard.h"
#include <queue>
#include "LCG.h"

class Player : public ParentGameObject
{
public:
    static const unsigned char AI = 2;
    static const unsigned char GAME_CONTROLLER = 1;
    static const unsigned char KEYBOARD = 0;

    Player(unsigned char controlScheme, int controllerID);
    ~Player();

    void init();

    void update();
    void render();

    TetrisBoard* getBoard();
    TetrisBlock* getCurrentPiece();

    void moveDown();

    void setLCG(LCG lcg);

    void setWin();
    void setLose();
private:
    void moveLeft();
    void moveRight();
    void rotateClock();
    void rotateCounterClock();
    void hold();

    void getPiece();
    void addPiece();
    unsigned char controlScheme = KEYBOARD;
    TetrisBoard* board = nullptr;
    TetrisBlock* currentPiece = nullptr;
    TetrisBlock* holdPiece = nullptr;
    int controllerID = 0;
    LCG lcg;

    bool won = false;
    bool lose = false;

    int x = 0;
    int y = 0;

    std::queue<TetrisBlock*> blockQueue;

    int lastBlockAdded = 0;
};
