#include "TetrisBlock.h"
#include "Mat3f.h"
#include "Game.h"

Image TetrisBlock::O_BLOCK_IMG = Image::loadBMP("Assets/Yellow_Block.bmp")[0];
Image TetrisBlock::I_BLOCK_IMG = Image::loadBMP("Assets/Blue_Block.bmp")[0];
Image TetrisBlock::J_BLOCK_IMG = Image::loadBMP("Assets/DarkPurple_Block.bmp")[0];
Image TetrisBlock::L_BLOCK_IMG = Image::loadBMP("Assets/Orange_Block.bmp")[0];
Image TetrisBlock::S_BLOCK_IMG = Image::loadBMP("Assets/Green_Block.bmp")[0];
Image TetrisBlock::Z_BLOCK_IMG = Image::loadBMP("Assets/Red_Block.bmp")[0];
Image TetrisBlock::T_BLOCK_IMG = Image::loadBMP("Assets/Magenta_Block.bmp")[0];
Image TetrisBlock::INVALID_IMG = Image::loadBMP("Assets/Gray_Block.bmp")[0];

TetrisBlock::TetrisBlock(unsigned char type) : ParentGameObject()
{
    std::memset(blockArray, 0, sizeof(bool)*16);

    switch (type)
    {
    case TYPE_I:
        blockArray[4] = true;
        blockArray[5] = true;
        blockArray[6] = true;
        blockArray[7] = true;
        axisOffset = Vec2f(0,0);
        img = &TetrisBlock::I_BLOCK_IMG;
        break;
    case TYPE_J:
        blockArray[2] = true;
        blockArray[4] = true;
        blockArray[5] = true;
        blockArray[6] = true;
        //c = {0,255,0,255};
        img = &TetrisBlock::J_BLOCK_IMG;
        break;
    case TYPE_L:
        blockArray[0] = true;
        blockArray[4] = true;
        blockArray[5] = true;
        blockArray[6] = true;
        //c = {255,165,0,255};
        img = &TetrisBlock::L_BLOCK_IMG;
        break;
    case TYPE_O:
        blockArray[5] = true;
        blockArray[6] = true;
        blockArray[9] = true;
        blockArray[10] = true;
        axisOffset = Vec2f(0,0);
        //c = {255,255,0,255};
        img = &TetrisBlock::O_BLOCK_IMG;
        break;
    case TYPE_S:
        blockArray[1] = true;
        blockArray[2] = true;
        blockArray[4] = true;
        blockArray[5] = true;
        //c = {0,255,0,255};
        img = &TetrisBlock::S_BLOCK_IMG;
        break;
    case TYPE_Z:
        blockArray[0] = true;
        blockArray[1] = true;
        blockArray[5] = true;
        blockArray[6] = true;
        //c = {255,0,0,255};
        img = &TetrisBlock::Z_BLOCK_IMG;
        break;
    case TYPE_T:
        blockArray[1] = true;
        blockArray[4] = true;
        blockArray[5] = true;
        blockArray[6] = true;
        //c = {255,0,255,255};
        img = &TetrisBlock::T_BLOCK_IMG;
        break;
    default:
        //c = {0,0,0,255};
        img = nullptr;
        break;
    }
}

void TetrisBlock::rotateClockwise()
{
    Vec2f finalAxis = axisOfRotation + axisOffset;

    Mat3f transformMatrix = Mat3f::getTranslationMatrix(finalAxis.x, finalAxis.y) * ( Mat3f::getRotationMatrix(90) * Mat3f::getTranslationMatrix(-finalAxis.x, -finalAxis.y) );
    
    bool newArray[16];
    std::memset(newArray, 0, sizeof(bool)*16);

    for(int i=0; i<16; i++)
    {
        Vec3f point = Vec3f(i%4, i/4, 1);

        Vec3f newPoint = transformMatrix*point;
        newPoint.x = std::round(newPoint.x);
        newPoint.y = std::round(newPoint.y);

        int newIndex = (int)newPoint.x + (int)newPoint.y*4;
        if(newIndex>=0 && newIndex <=15)
            newArray[newIndex] = blockArray[i];
    }

    for(int i=0; i<16; i++)
    {
        blockArray[i] = newArray[i];
    }
}

void TetrisBlock::rotateCounterClockwise()
{
    Vec2f finalAxis = axisOfRotation + axisOffset;

    Mat3f transformMatrix = Mat3f::getTranslationMatrix(finalAxis.x, finalAxis.y) * ( Mat3f::getRotationMatrix(-90) * Mat3f::getTranslationMatrix(-finalAxis.x, -finalAxis.y) );
    bool newArray[16];
    std::memset(newArray, 0, sizeof(bool)*16);

    for(int i=0; i<16; i++)
    {
        Vec3f point = Vec3f(i%4, i/4, 1);

        Vec3f newPoint = transformMatrix*point;
        newPoint.x = std::round(newPoint.x);
        newPoint.y = std::round(newPoint.y);

        int newIndex = (int)newPoint.x + (int)newPoint.y*4;
        if(newIndex>=0 && newIndex <=15)
            newArray[newIndex] = blockArray[i];
    }

    for(int i=0; i<16; i++)
    {
        blockArray[i] = newArray[i];
    }
}

void TetrisBlock::update()
{
}

void TetrisBlock::moveDown()
{
    y += 16;
}

void TetrisBlock::moveLeft()
{
    x -= 16;
}

void TetrisBlock::moveRight()
{
    x += 16;
}

void TetrisBlock::render()
{
    //draw tetris block

    Game* g = Game::getCurrentGame();
    Image* gameScreen = g->getGameImg();

    for(int i=0; i<16; i++)
    {
        if(blockArray[i])
        {
            //gameScreen->drawRect(x + 16*(i%4), y + 16*(i/4), 16, 16, false, c);
            gameScreen->drawImage(x + 16*(i%4), y + 16*(i/4), this->img);
        }
    }

}

int TetrisBlock::getX()
{
    return x;
}

int TetrisBlock::getY()
{
    return y;
}

Image* TetrisBlock::getImagePointer()
{
    return img;
}