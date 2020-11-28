#pragma once

#include <Windows.h>
#include <iostream>
#include <thread>
#include <mutex>

#include "Image.h"

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();

    void render(Image* img);
    
    int getWidth();
    int getHeight();
    std::string getTitle();

    bool getRunning();
    bool getValid();

    void setKeyboardDownFunction( void (*keyboardDown)(unsigned char) );
    void setKeyboardUpFunction( void (*keyboardUp)(unsigned char) );

private:
    int width = 0;
    int height = 0;
    std::string title = "";
    unsigned char* pixels;
    int padding = 0;

    void initWindow();
    void createBitmapInfo();
    void run();

    void paintFunction();
    
    void setValid(bool value);
    void setRunning(bool value);

    void destroy();

    void (*keyDownFunction)(unsigned char);
    void (*keyUpFunction)(unsigned char);
    
    static LRESULT __stdcall WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    std::thread* wndThread;
    std::mutex wndMutex;
    bool valid = false;
    bool running = false;

    WNDCLASSEXA wndClass;
    HWND windowHandle;
    HBITMAP bitmapHandle;
    BITMAPINFO bitInfo;
    HINSTANCE hins;
    HDC myHDC;

    static Window* mainWindow;
};