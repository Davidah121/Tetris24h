#include "GlobalController.h"
#include "Game.h"

GlobalController::GlobalController(int amountOfPlayers, int amountOfAI) : ParentGameObject()
{
    this->amountOfPlayers = amountOfPlayers;
    srand(NULL);
    LCG lcg = LCG(rand(), 48271, 571, 1<<31 );

    for(int i=0; i<amountOfPlayers; i++)
    {
        Player* p = new Player(Player::GAME_CONTROLLER, i);
        p->setLCG(lcg);
        p->init();
        Game::getCurrentGame()->addGameObject(p);
        players.push_back( p );
    }

    timeTillMove = 30;
}

GlobalController::~GlobalController()
{

}

bool GlobalController::getDone()
{
    return done;
}

void GlobalController::update()
{
    if(!done)
    {
        incrementTimer();

        int amount = 0;
        Player* p;
        for(int i=0; i<players.size(); i++)
        {
            TetrisBoard* b = players[i]->getBoard();
            if(b->getFailed())
            {
                amount++;
                players[i]->setLose();
            }
            else
            {
                p = players[i];
            }
        }

        if(players.size()>1)
        {
            if(amount == players.size()-1)
            {
                if(p!=nullptr)
                {
                    p->setWin();
                    done = true;
                }
            }
        }
        else
        {
            if(amount == 1)
            {
                done = true;
            }
        }
    }
}

void GlobalController::render()
{

}

void GlobalController::moveBlock()
{
    for(int i=0; i<players.size(); i++)
    {
        if(!players[i]->getBoard()->getFailed())
            players[i]->moveDown();
    }
}

void GlobalController::incrementTimer()
{
    timer++;

    if(timer>=timeTillMove)
    {
        timer = 0;
        levelTimer++;
        if(levelTimer >= amountTillNextLevel)
        {
            incrementLevel();
            levelTimer = 0;
        }

        moveBlock();
    }
}

void GlobalController::incrementLevel()
{
    timeTillMove = timeTillMove / 1.25;
}