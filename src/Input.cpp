#include "Input.h"
#include <iostream>

bool Input::keyboardDown[255];
bool Input::keyboardUp[255];
bool Input::keyboardPressed[255];
bool Input::joysticks[4];
bool Input::joysticksDown[4][15];
bool Input::joysticksUp[4][15];
bool Input::joysticksPressed[4][15];
Vec2f Input::joystickAxis[4][3];

Input::Input()
{

}

Input::~Input()
{

}

void Input::init()
{
    XInputEnable(true);
}

bool Input::getKeyDown(unsigned char c)
{
    return keyboardDown[c];
}

bool Input::getKeyUp(unsigned char c)
{
    return keyboardUp[c];
}

bool Input::getKeyPressed(unsigned char c)
{
    return keyboardPressed[c];
}

Vec2f Input::getJoystickLeftStick(int id)
{
    return joystickAxis[id][0];
}

Vec2f Input::getJoystickRightStick(int id)
{
    return joystickAxis[id][1];
}

bool Input::getJoystickDown(int id, unsigned char but)
{
    return joysticksDown[id][but];
}

bool Input::getJoystickUp(int id, unsigned char but)
{
    return joysticksUp[id][but];
}

bool Input::getJoystickPressed(int id, unsigned char but)
{
    return joysticksPressed[id][but];
}

bool Input::getJoystickExists(int id)
{
    return joysticks[id];
}

void Input::update()
{
    //update keyboards
    for(int i=0; i<255; i++)
    {
        if(keyboardUp[i])
        {
            keyboardDown[i] = false;
            keyboardPressed[i] = false;
            keyboardUp[i] = false;
        }
        if(keyboardDown[i])
        {
            keyboardPressed[i] = false;
            keyboardUp[i] = false;
        }
    }

    for(int i=0; i<4; i++)
    {
        setJoystickFunction(i);
    }
}

void Input::setKeyDownFunction(unsigned char c)
{
    if(keyboardDown[c] == false)
    {
        keyboardPressed[c] = true;
    }
    keyboardDown[c] = true;
    keyboardUp[c] = false;
}

void Input::setKeyUpFunction(unsigned char c)
{
    if(keyboardDown[c] == true)
    {
        keyboardDown[c] = false;
        keyboardPressed[c] = false;
        keyboardUp[c] = true;
    }
}

void Input::setJoystickButton(int id, unsigned char c, bool value)
{
    if(value)
    {
        joysticksPressed[id][c] = !joysticksDown[id][c];
        joysticksDown[id][c] = true;
        joysticksUp[id][c] = false;
    }
    else
    {
        if(joysticksDown[id][c])
            joysticksUp[id][c] = true;
        else
            joysticksUp[id][c] = false;
            
        joysticksPressed[id][c] = false;
        joysticksDown[id][c] = false;
    }
}

void Input::setJoystickFunction(int id)
{
    XINPUT_STATE state = { };
    int value = XInputGetState(id, &state);

    joysticks[id] = (value == ERROR_SUCCESS);

    if(joysticks[id])
    {
        joystickAxis[id][0] = Vec2f( (32767.5 - (unsigned short)state.Gamepad.sThumbLX) / 32767.5, (32767.5 - (unsigned short)state.Gamepad.sThumbLY) / 32767.5);
        joystickAxis[id][1] = Vec2f( (32767.5 - (unsigned short)state.Gamepad.sThumbRX) / 32767.5, (32767.5 - (unsigned short)state.Gamepad.sThumbRY) / 32767.5);
        joystickAxis[id][2] = Vec2f( (127.5 - (unsigned char)state.Gamepad.bLeftTrigger) / 127.5, (127.5 - (unsigned char)state.Gamepad.bRightTrigger) / 127.5);
        
        bool butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
        setJoystickButton(id, A_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_B;
        setJoystickButton(id, B_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_X;
        setJoystickButton(id, X_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
        setJoystickButton(id, Y_BUTTON, butVal);

        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        setJoystickButton(id, LB_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
        setJoystickButton(id, RB_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
        setJoystickButton(id, START_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
        setJoystickButton(id, BACK_BUTTON, butVal);

        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
        setJoystickButton(id, UP_DPAD, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        setJoystickButton(id, DOWN_DPAD, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        setJoystickButton(id, LEFT_DPAD, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        setJoystickButton(id, RIGHT_DPAD, butVal);

        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
        setJoystickButton(id, LSTICK_BUTTON, butVal);
        butVal = state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
        setJoystickButton(id, RSTICK_BUTTON, butVal);
    }
    
}
