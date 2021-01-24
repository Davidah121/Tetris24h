#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "Vec2f.h"


class Input
{
public:
    Input();
    ~Input();

    static const unsigned char A_BUTTON = 0;
    static const unsigned char B_BUTTON = 1;
    static const unsigned char X_BUTTON = 2;
    static const unsigned char Y_BUTTON = 3;

    static const unsigned char LB_BUTTON = 5;
    static const unsigned char RB_BUTTON = 6;

    static const unsigned char START_BUTTON = 7;
    static const unsigned char BACK_BUTTON = 8;

    static const unsigned char UP_DPAD = 9;
    static const unsigned char DOWN_DPAD = 10;
    static const unsigned char LEFT_DPAD = 11;
    static const unsigned char RIGHT_DPAD = 12;

    static const unsigned char LSTICK_BUTTON = 13;
    static const unsigned char RSTICK_BUTTON = 14;
    
    static void init();

    static bool getKeyDown(unsigned char c);
    static bool getKeyUp(unsigned char c);
    static bool getKeyPressed(unsigned char c);

    static Vec2f getJoystickLeftStick(int id);
    static Vec2f getJoystickRightStick(int id);
    
    static bool getJoystickDown(int id, unsigned char but);
    static bool getJoystickUp(int id, unsigned char but);
    static bool getJoystickPressed(int id, unsigned char but);

    static bool getJoystickExists(int id);


    //update stuff
    static void update();
    static void setKeyDownFunction(unsigned char c);
    static void setKeyUpFunction(unsigned char c);
    static void setJoystickFunction(int id);

    static void setJoystickButton(int id, unsigned char c, bool value);

    static void denyInputs();
private:
    static bool keyboardDown[256];
    static bool keyboardUp[256];
    static bool keyboardPressed[256];

    static bool nextKeyboard[256];

    static bool joysticks[4];
    static bool joysticksDown[4][15];
    static bool joysticksUp[4][15];
    static bool joysticksPressed[4][15];
    static Vec2f joystickAxis[4][3];

    static bool deny;
};