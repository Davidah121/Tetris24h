#include "MenuController.h"
#include "Input.h"
#include "Game.h"
#include "GlobalController.h"
#include "AssetManager.h"
#include "Audio.h"
#include <string>


MenuController::MenuController() : ParentGameObject()
{
    this->depth = -1000;
    Audio::playAudio();
}

MenuController::~MenuController()
{
    Audio::stopAudio();
}

void MenuController::update()
{
    if(gc!=nullptr)
    {
        if(gc->getDone())
        {
            timeWaited++;
        }

        if(timeWaited >= timeToWait)
        {
            menuNumber = TITLEMENU;
            Game::getCurrentGame()->clear();
            Game::getCurrentGame()->addGameObject( (ParentGameObject*)this);

            delete gc;
            gc = nullptr;
        }
    }

    if(menuNumber == -1)
    {
        return;
    }

    if(Input::getJoystickPressed(0, Input::A_BUTTON) || Input::getKeyPressed('A'))
    {
        if(menuNumber == TITLEMENU)
        {
            switch(selection)
            {
                case 0:
                    //start button
                    menuNumber = 1;
                    selection = 0;
                    break;
                case 1:
                    //options button
                    menuNumber = 2;
                    selection = 0;
                    break;
                case 2:
                    //quit button
                    selection = 0;
                    Audio::stopAudio();
                    Game::getCurrentGame()->closeGame();
                    break;
                default:
                    break;
            }
        }
        else if(menuNumber == PLAYERMENU)
        {
            switch(selection)
            {
                case 0:
                    //1 Player button
                    menuNumber = -1;
                    selection = 0;

                    gc = new GlobalController(1, 0);
                    Game::getCurrentGame()->addGameObject( gc );
                    break;
                case 1:
                    //2 Players button
                    menuNumber = -1;
                    selection = 0;

                    gc = new GlobalController(2, 0);
                    Game::getCurrentGame()->addGameObject( gc );
                    break;
                default:
                    break;
            }
        }
        else if(menuNumber == OPTIONSMENU)
        {
            int v = 0;
            switch(selection)
            {
                case 0:
                    //up button
                    v = Audio::getVolume() * 10;
                    v++;
                    Audio::setVolume( (double)v / 10.0);
                    break;
                case 1:
                    //down button
                    v = Audio::getVolume() * 10;
                    v--;
                    Audio::setVolume( (double)v / 10.0);
                    break;
                default:
                    break;
            }
        } 
    }

    if(Input::getJoystickPressed(0, Input::UP_DPAD) || Input::getKeyPressed(0x26))
    {
        selection-=1;
        if(selection<0)
        {
            selection = 0;
        }
    }
    else if(Input::getJoystickPressed(0, Input::DOWN_DPAD) || Input::getKeyPressed(0x28))
    {
        selection+=1;
        switch (menuNumber)
        {
        case TITLEMENU:
            if(selection>2)
                selection=2;
            break;
        case PLAYERMENU:
            if(selection>1)
                selection=1;
            break;
        case OPTIONSMENU:
            if(selection>1)
                selection=1;
            break;
        default:
            selection = 0;
            break;
        }
    }

    if(Input::getJoystickPressed(0, Input::B_BUTTON) || Input::getKeyPressed('D'))
    {
        menuNumber = TITLEMENU;
        selection = 0;
    }
}

void MenuController::render()
{
    //draw background
    AssetManager* assets = AssetManager::getAssetManager();

    Image* backgroundImage = assets->getAsset("background");
    Image* titleText = assets->getAsset("titleText");
    
    Image* renderSurf = Game::getCurrentGame()->getGameImg();

    renderSurf->drawImage(0, 0, backgroundImage);

    if(menuNumber>=0)
        renderSurf->drawImage(0, 0, titleText);

    if(menuNumber == TITLEMENU)
    {
        Image* selection1 = assets->getAsset("startText");
        Image* selection2 = assets->getAsset("optionsText");
        Image* selection3 = assets->getAsset("quitText");

        renderSurf->drawImage(640/2 - 96, 240-32, selection1);
        renderSurf->drawImage(640/2 - 96, 304-32, selection2);
        renderSurf->drawImage(640/2 - 96, 368-32, selection3);

        switch(selection)
        {
            case 0:
                renderSurf->drawRect(640/2 - 80, 240-32, 160, 56, true, {0,0,0,255});
                break;
            case 1:
                renderSurf->drawRect(640/2 - 80, 304-32, 160, 56, true, {0,0,0,255});
                break;
            case 2:
                renderSurf->drawRect(640/2 - 80, 368-32, 160, 56, true, {0,0,0,255});
                break;
            default:
                break;
        }
    }
    else if(menuNumber == PLAYERMENU)
    {
        Image* selection1 = assets->getAsset("1PlayerText");
        Image* selection2 = assets->getAsset("2PlayerText");

        renderSurf->drawImage(640/2 - 96, 240 - 32, selection1);
        renderSurf->drawImage(640/2 - 96, 304 - 32, selection2);

        switch(selection)
        {
            case 0:
                renderSurf->drawRect(640/2 - 96, 240-32, 192, 56, true, {0,0,0,255});
                break;
            case 1:
                renderSurf->drawRect(640/2 - 96, 304-32, 192, 56, true, {0,0,0,255});
                break;
            default:
                break;
        }
    }
    else if(menuNumber == OPTIONSMENU)
    {
        Image* volumeText = assets->getAsset("volumeText");
        Image* selection2 = assets->getAsset("upArrow");
        Image* selection3 = assets->getAsset("downArrow");

        int v = Audio::getVolume() * 10;
        std::string vAsText = std::to_string(v);

        vAsText += "Text";
        Image* volumeValue = assets->getAsset(vAsText);

        renderSurf->drawImage(640/2 - 96 - 128, 240 - 32, volumeText);
        renderSurf->drawImage(640/2 - 96 + 64, 240 - 32 - 8, selection2);
        renderSurf->drawImage(640/2 - 96 + 64, 240 - 32 + 24, selection3);

        if(volumeValue!=nullptr)
            renderSurf->drawImage(640/2 - 96 + 128, 240 - 24, volumeValue);

        switch(selection)
        {
            case 0:
                renderSurf->drawRect(640/2 - 96 + 64, 240-32-8, 32, 32, true, {0,0,0,255});
                break;
            case 1:
                renderSurf->drawRect(640/2 - 96 + 64, 240-32+24, 32, 32, true, {0,0,0,255});
                break;
            default:
                break;
        }
    }
    
}

