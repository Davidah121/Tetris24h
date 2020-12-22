#include "GlobalController.h"
#include "Game.h"

GlobalController::GlobalController(int amountOfPlayers, int amountOfAI) : ParentGameObject()
{
    this->amountOfPlayers = amountOfPlayers;
    srand(NULL);
    LCG lcg = LCG(rand(), 48271, 571, 2<<31 );

    for(int i=0; i<amountOfPlayers; i++)
    {
        Player* p = new Player(Player::KEYBOARD, -1);
        p->setLCG(lcg);
        Game::getCurrentGame()->addGameObject(p);
        players.push_back( p );
    }

    timeTillMove = 30;
}

GlobalController::~GlobalController()
{

}

void GlobalController::update()
{
    incrementTimer();
}

void GlobalController::render()
{

}

void GlobalController::moveBlock()
{
    for(int i=0; i<players.size(); i++)
    {
        players[i]->moveDown();
    }
}

void GlobalController::incrementTimer()
{
    timer++;

    if(timer>=timeTillMove)
    {
        timer = 0;
        moveBlock();
    }
}

void GlobalController::incrementLevel()
{

}