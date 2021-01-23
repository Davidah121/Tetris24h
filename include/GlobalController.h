#pragma once
#include "ParentGameObject.h"
#include "Player.h"
#include <vector>

class GlobalController : public ParentGameObject
{
public:
    GlobalController(int amountOfPlayers, int amountOfAI);
    ~GlobalController();

    void update();
    void render();

    bool getDone();
private:

    void moveBlock();
    void incrementTimer();
    void incrementLevel();

    int timer = 0;
    int timeTillMove = 0;
    int amountOfPlayers = 0;
    int amountTillNextLevel = 30;
    int levelTimer = 0;
    int level = 1;
    std::vector<Player*> players;

    bool done = false;
};