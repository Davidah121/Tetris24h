#include "Game.h"

Game::Game()
{
    wnd = new Window(320, 240, "TETRIS_24HR");
    currentFPS = 0;
    targetFPS = 60;
    gameImg = Image(320, 240);
    gameObjects = std::vector<ParentGameObject*>();
}

Game::~Game()
{
    if(wnd!=nullptr)
        delete wnd;
}

void Game::run()
{
    unsigned long targetTime = SecondInNanos / targetFPS;
    unsigned long totalTime = 0;
    int tempFPS = 0;
    while(running)
    {
        unsigned long startTime = System::getNanoTime();
        Input::update();
        update();
        sort();
        render();

        if(wnd->getRunning())
            wnd->render(&gameImg);
        else
            running = false;
        
        unsigned long endTime = System::getNanoTime();
        while( (endTime - startTime) < targetTime)
        {
            endTime = System::getNanoTime();
        }
        unsigned long timeElasped = endTime - startTime;

        deltaTime = (double)timeElasped / targetTime;
        totalTime += timeElasped;

        tempFPS++;

        if(totalTime > SecondInNanos)
        {
            currentFPS = tempFPS;
            totalTime = 0;
        }
    }
}

void Game::addGameObject(ParentGameObject* obj)
{
    //first, make sure it doesn't exist in the list
    bool exists = false;
    for(ParentGameObject* o : gameObjects)
    {
        if(o == obj)
        {
            exists = true;
            break;
        }
    }

    if(!exists)
    {
        gameObjects.push_back(obj);
    }
}

void Game::removeGameObject(ParentGameObject* obj)
{
    std::vector<ParentGameObject*> otherList;

    for(ParentGameObject* o : gameObjects)
    {
        if(o != obj)
        {
            otherList.push_back(o);
        }
    }

    gameObjects = otherList;
}

void Game::closeGame()
{
    running = false;
}

void Game::update()
{
    for(int i=0; i<gameObjects.size(); i++)
    {
        gameObjects[i]->update();
    }
}

void Game::render()
{
    for(int i=0; i<gameObjects.size(); i++)
    {
        if(gameObjects[i]->visible)
            gameObjects[i]->render();
    }
}

void Game::sort()
{
    //insertion sort
    for(int i=0; i<gameObjects.size(); i++)
    {
        for(int i2=i; i2>0; i2--)
        {
            if(gameObjects[i2]->depth < gameObjects[i2-1]->depth)
            {
                ParentGameObject* temp = gameObjects[i2-1];
                gameObjects[i2-1] = gameObjects[i2];
                gameObjects[i2] = temp;
            }
            else
            {
                break;
            }
        }
    }
}