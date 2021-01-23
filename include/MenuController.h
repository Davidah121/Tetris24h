#pragma once
#include "ParentGameObject.h"
#include "GlobalController.h"

class MenuController : public ParentGameObject
{
public:
    MenuController();
    ~MenuController();

    void update();
    void render();

    static const int TITLEMENU = 0;
    static const int PLAYERMENU = 1;
    static const int OPTIONSMENU = 2;
    
private:
    int menuNumber = 0;
    int selection = 0;

    GlobalController* gc = nullptr;
    int timeWaited = 0;
    int timeToWait = 120;
};