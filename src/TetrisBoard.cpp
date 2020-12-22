#include "TetrisBoard.h"
#include "Game.h"

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

    if(xLoc >= 10 || xLoc < 0)
    {
        return true;
    }

    if(yLoc >= 24 || yLoc < 0)
    {
        return true;
    }


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
    for(int i=0; i<10; i++)
    {
        img->drawLine(x + i*16, y, x + i*16, y + 320, {0,0,0,255});
    }

    //horizontal
    for(int i=0; i<20; i++)
    {
        img->drawLine(x, y + i*16, x + 160, y + i*16, {0,0,0,255});
    }

    for(int i=0; i<20; i++)
    {
        for(int i2=0; i2<10; i2++)
        {
            BoardPiece p = boardArray[i2 + i*10];

            if(p.filled)
                img->drawImage(x + i2*16, y + (320 - i*16), p.img);
        }
    }
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
        }
    }
}

void TetrisBoard::clearLines()
{
    bool canClear = true;
    for(int y=23; y>=0; y--)
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
            
            for(int y2=y+1; y2<24; y2++)
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
    for(int y=20; y<24; y++)
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