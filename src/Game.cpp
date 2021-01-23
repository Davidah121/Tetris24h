#include "Game.h"
#include "Audio.h"

Game* Game::currentGame = nullptr;
Game::Game()
{
    wnd = new Window(640, 480, "TETRIS_24HR");
    currentFPS = 0;
    targetFPS = 60;
    gameImg = new Image(640, 480);
    gameObjects = std::vector<ParentGameObject*>();
    currentGame = this;
    running = true;

    Input::init();
    Audio::init();
    wnd->setKeyboardDownFunction(Input::setKeyDownFunction);
    wnd->setKeyboardUpFunction(Input::setKeyUpFunction);
}

Game::~Game()
{
    Game::clear();
    Audio::dispose();
    std::cout << "CLOSED GAME" << std::endl;
    if(wnd!=nullptr)
        delete wnd;
    if(gameImg!=nullptr)
        delete gameImg;
}

Game* Game::getCurrentGame()
{
    return Game::currentGame;
}

Image* Game::getGameImg()
{
    return gameImg;
}

void Game::run()
{
    unsigned long targetTime = SecondInNanos / targetFPS;
    unsigned long totalTime = 0;
    int tempFPS = 0;
    while(running)
    {
        unsigned long startTime = System::getNanoTime();
        update();
        sort();
        render();
        Input::update();

        if(wnd->getRunning())
        {
            wnd->render(gameImg);
        }
        else
        {
            closeGame();
            break;
        }
        
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
            tempFPS = 0;
            totalTime = 0;
            std::cout << "FPS: " << currentFPS << std::endl;
        }

        if(Input::getKeyDown('Q'))
            closeGame();
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

void Game::clear()
{
    gameObjects.clear();
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
    gameImg->clearImage({255,255,255,255});
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