#pragma once

#include "Window.h"
#include "System.h"
#include "Image.h"
#include "Input.h"
#include "ParentGameObject.h"
#include <vector>

class Game
{
public:
    Game();
    ~Game();

    void run();
    void addGameObject(ParentGameObject* obj);
    void removeGameObject(ParentGameObject* obj);

    void closeGame();
private:
    int currentFPS;
    int targetFPS;
    double deltaTime = 1.0;
    bool running;

    void update();
    void render();
    void sort();

    Window* wnd;

    Image gameImg;

    std::vector<ParentGameObject*> gameObjects;
};