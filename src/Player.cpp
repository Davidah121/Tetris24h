#include "Player.h"
#include "Input.h"
#include "Game.h"
#include "AssetManager.h"

Player::Player(int x, int y, unsigned char controlScheme, int controllerID) : ParentGameObject()
{
    board = new TetrisBoard();

    this->x = x;
    this->y = y;

    board->setX(x);
    board->setY(y);

    Game::getCurrentGame()->addGameObject(board);

    this->controlScheme = controlScheme;
    this->controllerID = controllerID;

    depth = 100;
}

Player::~Player()
{
    delete board;
    if(currentPiece!=nullptr)
        delete currentPiece;
    if(holdPiece!=nullptr)
        delete holdPiece;
}

void Player::init()
{
    addPiece();
    getPiece();
}

void Player::update()
{
    if(board->getFailed())
    {
        return;
    }

    if(won)
    {
        return;
    }

    if(lose)
    {
        return;
    }

    if(currentPiece!=nullptr)
    {
        if(controlScheme==KEYBOARD)
        {
            if(Input::getKeyPressed(0x25))
            {
                //left arrow
                moveLeft();
            }
            else if(Input::getKeyPressed(0x27))
            {
                //right arrow
                moveRight();
            }
            else if(Input::getKeyDown(0x28))
            {
                //down arrow
                moveDown();
            }
            else if(Input::getKeyPressed('A'))
            {
                //rotate counter clockwise
                rotateCounterClock();
            }
            else if(Input::getKeyPressed('D'))
            {
                //rotate clockwise
                rotateClock();
            }
            else if(Input::getKeyPressed(' '))
            {
                //hold
                hold();
            }
        }
        else if(controlScheme==GAME_CONTROLLER)
        {
            if(Input::getJoystickPressed(controllerID, Input::LEFT_DPAD))
            {
                //left arrow
                moveLeft();
            }
            else if(Input::getJoystickPressed(controllerID, Input::RIGHT_DPAD))
            {
                //right arrow
                moveRight();
            }
            else if(Input::getJoystickDown(controllerID, Input::DOWN_DPAD))
            {
                //down arrow
                moveDown();
            }
            else if(Input::getJoystickPressed(controllerID, Input::LB_BUTTON))
            {
                //rotate counter clockwise
                rotateCounterClock();
            }
            else if(Input::getJoystickPressed(controllerID, Input::RB_BUTTON))
            {
                //rotate clockwise
                rotateClock();
            }
            else if(Input::getJoystickPressed(controllerID, Input::B_BUTTON))
            {
                //hold
                hold();
            }
        }
    }
}

void Player::moveLeft()
{
    if(currentPiece!=nullptr)
    {
        currentPiece->moveLeft();
        if(board->checkCollision(*currentPiece))
            currentPiece->moveRight();
    }
}
void Player::moveRight()
{
    if(currentPiece!=nullptr)
    {
        currentPiece->moveRight();
        if(board->checkCollision(*currentPiece))
            currentPiece->moveLeft();
    }
}
void Player::moveDown()
{
    if(currentPiece!=nullptr)
    {
        currentPiece->moveDown();
        if(board->checkCollision(*currentPiece))
        {
            currentPiece->moveUp();
            board->addPiece(*currentPiece);

            Game::getCurrentGame()->removeGameObject(currentPiece);
            delete currentPiece;

            getPiece();
        }
    }
}
void Player::rotateClock()
{
    if(currentPiece!=nullptr)
    {
        currentPiece->rotateClockwise();
        if(board->checkCollision(*currentPiece))
            currentPiece->rotateCounterClockwise();
    }
}
void Player::rotateCounterClock()
{
    if(currentPiece!=nullptr)
    {
        currentPiece->rotateCounterClockwise();
        if(board->checkCollision(*currentPiece))
            currentPiece->rotateClockwise();
    }
}
void Player::hold()
{
    if(currentPiece!=nullptr)
    {
        if(holdPiece!=nullptr)
        {
            TetrisBlock* temp = holdPiece;
            holdPiece = currentPiece;
            currentPiece = temp;

            currentPiece->setX( holdPiece->getX());
            currentPiece->setY( holdPiece->getY());

            currentPiece->setRenderStartX(0);
            currentPiece->setRenderStartY(0);

            holdPiece->setX(0);
            holdPiece->setY(0);
        }
        else
        {
            holdPiece = currentPiece;
            getPiece();

            currentPiece->setX( holdPiece->getX());
            currentPiece->setY( holdPiece->getY());

            currentPiece->setRenderStartX(0);
            currentPiece->setRenderStartY(0);

            holdPiece->setX(0);
            holdPiece->setY(0);
        }

        if(board->checkCollision(*currentPiece))
        {
            //revert
            TetrisBlock* temp = holdPiece;
            holdPiece = currentPiece;
            currentPiece = temp;

            currentPiece->setX( holdPiece->getX());
            currentPiece->setY( holdPiece->getY());

            currentPiece->setRenderStartX(0);
            currentPiece->setRenderStartY(0);

            holdPiece->setX(0);
            holdPiece->setY(0);
        }
    }
}

void Player::render()
{
    Image* gameImg = Game::getCurrentGame()->getGameImg();
    
    Image* imgText = nullptr;
    if(won==true)
    {
        imgText = AssetManager::getAssetManager()->getAsset("winText");
    }
    else if (lose==true)
    {
        imgText = AssetManager::getAssetManager()->getAsset("loseText");
    }

    if(imgText!=nullptr)
    {
        gameImg->drawImage(x, y, imgText);
    }

    if(holdPiece!=nullptr)
    {
        holdPiece->setRenderStartX(x + 192);
        holdPiece->setRenderStartY(y);

        holdPiece->render();
    }

    if(currentPiece!=nullptr)
    {
        currentPiece->render();
    }

}

void Player::setWin()
{
    won = true;
}

void Player::setLose()
{
    lose = true;
}

void Player::getPiece()
{
    if(board->getFailed())
    {
        currentPiece = nullptr;
        return;
    }
    currentPiece = blockQueue.front();
    currentPiece->setX( board->getX() + 80);
    currentPiece->setY( board->getY());
    
    blockQueue.pop();
    addPiece();
}

void Player::addPiece()
{
    int value = -1;

    do
    {
        value = lcg.get() % 7;
    } while (value==lastBlockAdded);

    lastBlockAdded = value;
    
    TetrisBlock* c = new TetrisBlock(value);

    c->setX( board->getX() + 80);
    c->setX( board->getY());
    
    blockQueue.push(c);
}

TetrisBoard* Player::getBoard()
{
    return board;
}

TetrisBlock* Player::getCurrentPiece()
{
    return currentPiece;
}

void Player::setLCG(LCG lcg)
{
    this->lcg = lcg;
}