#include "TetrisBoard.h"
#include "Game.h"
#include "AssetManager.h"

TetrisBoard::TetrisBoard() : ParentGameObject()
{
    std::memset(boardArray, 0, sizeof(bool)*240);
}

TetrisBoard::~TetrisBoard()
{

}

bool TetrisBoard::checkCollision(TetrisBlock b)
{
    bool* array = b.getBlockArray();
    int xLoc = (b.getX() - this->x)/16;
    int yLoc = (b.getY() - this->y)/16;

    int index = 0;

    bool col = false;

    for(int yv = yLoc; yv < yLoc+4; yv++)
    {
        for(int xv = xLoc; xv < xLoc+4; xv++)
        {
            if(array[index])
            {
                if(xv >= 10 || xv < 0)
                {
                    col = true;
                    break;
                }

                if(yv >= 24 || yv < 0)
                {
                    col = true;
                    break;
                }

                if(this->boardArray[xv + yv*10].filled)
                {
                    col = true;
                    break;
                }
            }
            index++;
        }
    }

    return col;
}

void TetrisBoard::update()
{
    clearLines();
    checkIfFailed();
}

void TetrisBoard::render()
{
    Image* img = Game::getCurrentGame()->getGameImg();
    //vertical
    for(int i=0; i<11; i++)
    {
        img->drawLine(x + i*16, y + 64, x + i*16, y + 320 + 64, {0,0,0,255});
    }

    //horizontal
    for(int i=0; i<21; i++)
    {
        img->drawLine(x, y + 64 + i*16, x + 160, y + 64 + i*16, {0,0,0,255});
    }

    for(int i=0; i<24; i++)
    {
        for(int i2=0; i2<10; i2++)
        {
            BoardPiece p = boardArray[i2 + i*10];

            if(p.filled)
                img->drawImage(x + i2*16, y + i*16, p.img);
        }
    }

    //game over area
    img->drawRect(x, y, 160, 64, false, {128,0,0,64});
}

void TetrisBoard::addPiece(TetrisBlock b)
{
    bool* array = b.getBlockArray();
    int xLoc = (b.getX() - this->x)/16;
    int yLoc = (b.getY() - this->y)/16;

    int index = 0;

    for(int yv = yLoc; yv < yLoc+4; yv++)
    {
        for(int xv = xLoc; xv < xLoc+4; xv++)
        {
            if(array[index])
            {
                this->boardArray[xv + yv*10].filled = true;
                this->boardArray[xv + yv*10].img = b.getImagePointer();
            }
            index++;
        }
    }

    clearLines();
    checkIfFailed();
}

void TetrisBoard::clearLines()
{
    bool canClear = true;
    for(int y=0; y<24; y++)
    {
        canClear = true;
        for(int x=0; x<10; x++)
        {
            if(boardArray[x + y*10].filled == false)
            {
                canClear = false;
                break;
            }
        }

        if(canClear)
        {
            for(int x=0; x<10; x++)
            {
                boardArray[x + y*10].filled = false;
                boardArray[x + y*10].img = nullptr;
            }
            for(int y2=y; y2>0; y2--)
            {
                for(int x=0; x<10; x++)
                {
                    boardArray[x + y2*10].filled = boardArray[x + (y2-1)*10].filled;
                    boardArray[x + y2*10].img = boardArray[x + (y2-1)*10].img;
                }
            }
        }
    }
}

void TetrisBoard::setX(int x)
{
    this->x = x;
}

int TetrisBoard::getX()
{
    return x;
}

void TetrisBoard::setY(int y)
{
    this->y = y;
}

int TetrisBoard::getY()
{
    return y;
}

bool TetrisBoard::getFailed()
{
    return failed;
}

void TetrisBoard::checkIfFailed()
{
    for(int y=0; y<4; y++)
    {
        for(int x=0; x<10; x++)
        {
            if(boardArray[x + y*10].filled)
            {
                failed = true;
                break;
            }
        }
    }
}