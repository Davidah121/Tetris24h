#include "Player.h"
#include "Input.h"
#include "Game.h"

Player::Player(unsigned char controlScheme, int controllerID) : ParentGameObject()
{
    board = new TetrisBoard();
    Game::getCurrentGame()->addGameObject(board);

    this->controlScheme = controlScheme;
    this->controllerID = controllerID;
}

Player::~Player()
{
}

void Player::update()
{
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
            else if(Input::getKeyPressed(0x28))
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
            else if(Input::getJoystickPressed(controllerID, Input::DOWN_DPAD))
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
    currentPiece->moveLeft();
    if(board->checkCollision(*currentPiece))
        currentPiece->moveRight();
}
void Player::moveRight()
{
    currentPiece->moveRight();
    if(board->checkCollision(*currentPiece))
        currentPiece->moveLeft();
}
void Player::moveDown()
{
    currentPiece->moveDown();
    if(board->checkCollision(*currentPiece))
    {
        board->addPiece(*currentPiece);
        getPiece();
    }
}
void Player::rotateClock()
{
    currentPiece->rotateClockwise();
    if(board->checkCollision(*currentPiece))
        currentPiece->rotateCounterClockwise();
}
void Player::rotateCounterClock()
{
    currentPiece->rotateCounterClockwise();
    if(board->checkCollision(*currentPiece))
        currentPiece->rotateClockwise();
}
void Player::hold()
{
    if(holdPiece!=nullptr)
    {
        TetrisBlock* temp = holdPiece;
        holdPiece = currentPiece;
        currentPiece = temp;
    }
    else
    {
        holdPiece = currentPiece;
    }
}

void Player::render()
{

}

void Player::getPiece()
{
    blockQueue.pop();
    addPiece();
}

void Player::addPiece()
{
    int value = -1;

    do
    {
        value = lcg.get() % 7;
    } while (value!=lastBlockAdded);
    
    blockQueue.push(TetrisBlock(value));
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