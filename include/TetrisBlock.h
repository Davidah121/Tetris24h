#pragma once

#include "ParentGameObject.h"
#include "Vec2f.h"
#include "Image.h"

class TetrisBlock : public ParentGameObject
{
public:
    static const unsigned char TYPE_I = 0;
    static const unsigned char TYPE_J = 1;
    static const unsigned char TYPE_L = 2;
    static const unsigned char TYPE_O = 3;
    static const unsigned char TYPE_S = 4;
    static const unsigned char TYPE_Z = 5;
    static const unsigned char TYPE_T = 6;

    static Image O_BLOCK_IMG;
    static Image I_BLOCK_IMG;
    static Image J_BLOCK_IMG;
    static Image L_BLOCK_IMG;
    static Image S_BLOCK_IMG;
    static Image Z_BLOCK_IMG;
    static Image T_BLOCK_IMG;
    static Image INVALID_IMG;

    TetrisBlock(unsigned char type);
    ~TetrisBlock();

    void update();
    void render();

    void rotateClockwise();
    void rotateCounterClockwise();

    void moveDown();
    void moveLeft();
    void moveRight();

    bool* getBlockArray();

    int getX();
    int getY();

    Image* getImagePointer();
private:
    bool blockArray[16];

    int x = 0;
    int y = 0;

    Vec2f axisOffset = Vec2f(-0.5,-0.5);
    Vec2f axisOfRotation = Vec2f(1.5, 1.5);

    Image* img;
};