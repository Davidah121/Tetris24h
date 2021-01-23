#include <iostream>
#include "Window.h"
#include "Input.h"
#include "System.h"
#include "Game.h"
#include "GlobalController.h"
#include "MenuController.h"

void testWindow()
{
    Window wnd(320, 240, "title1");

    Image img = Image(320, 240);
    img.clearImage({255,0,0,255});

    Input::init();

    wnd.setKeyboardDownFunction( Input::setKeyDownFunction );
    wnd.setKeyboardUpFunction( Input::setKeyUpFunction );
    
    Image* otherImg = Image::loadBMP("Untitled.bmp");

    if(otherImg!=nullptr)
    {
        img.drawImage(32,32,otherImg);
        img.drawImageExt(32,32,2,2,20,0,0,otherImg);
    }

    while(wnd.getRunning())
    {
        unsigned long startTime = System::getNanoTime();
        Input::update();
        wnd.render(&img);
        
        while( (System::getNanoTime() - startTime) < 16666666)
        {

        }
    }

    if(otherImg!=nullptr)
        delete otherImg;

    system("pause");
}

void startGame()
{
    Game g = Game();
    g.addGameObject(new MenuController());
    g.run();
}

int main()
{
    //testWindow();
    startGame();
    std::cout<<"Hello world" << std::endl;
    return 0;
}