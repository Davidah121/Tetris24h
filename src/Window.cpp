#include "Window.h"
#include "System.h"

Window* Window::mainWindow = nullptr;

LRESULT __stdcall Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_KEYDOWN:
            if(mainWindow->keyDownFunction!=nullptr)
                mainWindow->keyDownFunction((unsigned char)wParam);
            break;
        case WM_KEYUP:
            if(mainWindow->keyUpFunction!=nullptr)
                mainWindow->keyUpFunction((unsigned char)wParam);
            break;
        case WM_PAINT:
            mainWindow->paintFunction();
            break;
        case WM_CLOSE:
            std::cout << "CLOSED WINDOW" << std::endl;
            mainWindow->setRunning(false);
            break;
        default:
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window(int width, int height, std::string title)
{
    this->width = width;
    this->height = height;
    this->title = title;
    this->hins = System::getHInstance();

    setRunning(true);
    bool wasVal = false;

    wndThread = new std::thread(&Window::initWindow, this);

    while(getRunning())
    {
        if(getValid())
        {
            wasVal = true;
            break;
        }
    }

    if(!wasVal)
    {
        //not valid
    }

    Window::mainWindow = this;
}

Window::~Window()
{
    destroy();
}

void Window::destroy()
{
    if(getValid())
    {
        setRunning(false);
        setValid(false);
        if(wndThread!=nullptr)
        {
            if(wndThread->joinable())
            {
                wndThread->join();
            }
        }

        DestroyWindow(windowHandle);
        UnregisterClassA(wndClass.lpszClassName, hins);
        DeleteObject(bitmapHandle);
        DeleteObject(myHDC);
    }
}

void Window::initWindow()
{
    wndClass.cbSize = sizeof(WNDCLASSEXA);
    wndClass.style = CS_VREDRAW | CS_HREDRAW;
    wndClass.cbWndExtra = NULL;
    wndClass.cbClsExtra = NULL;
    wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND+1;
    wndClass.hCursor = NULL;
    wndClass.hIcon = NULL;
    wndClass.hIconSm = NULL;
    wndClass.hInstance = hins;
    wndClass.lpfnWndProc = Window::WndProc;
    wndClass.lpszClassName = "ClassName1";
    wndClass.lpszMenuName = "";

    if(RegisterClassExA(&wndClass))
    {
        windowHandle = CreateWindowExA(NULL, "ClassName1", title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hins, NULL);
        if(windowHandle!=NULL)
        {
            myHDC = GetDC(windowHandle);
            ShowWindow(windowHandle, SW_SHOW);
            createBitmapInfo();
            run();
        }
        else
        {
            //invalid
            setRunning(false);
        }
    }
    else
    {
        //invalid
        setRunning(false);
    }
}

void Window::run()
{
    setValid(true);

    while(getRunning())
    {
        //
        MSG message;
        memset(&message, 0, sizeof(MSG));

        PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE);
        
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

bool Window::getValid()
{
    bool v;
    wndMutex.lock();
    v = valid;
    wndMutex.unlock();
    return v;
}

void Window::setValid(bool value)
{
    wndMutex.lock();
    valid = value;
    wndMutex.unlock();
}

bool Window::getRunning()
{
    bool v;
    wndMutex.lock();
    v = running;
    wndMutex.unlock();
    return v;
}

void Window::setRunning(bool value)
{
    wndMutex.lock();
    running = value;
    wndMutex.unlock();
}

void Window::setKeyboardDownFunction( void (*keyboardDown)(unsigned char) )
{
    keyDownFunction = keyboardDown;
}

void Window::setKeyboardUpFunction( void (*keyboardUp)(unsigned char) )
{
    keyUpFunction = keyboardUp;
}

void Window::render(Image* img)
{
    //convert Image pixels
    if(img->getWidth() == width && img->getHeight() == height)
    {
        int k = 0;
        for(int i2=0; i2<height; i2++)
        {
            for(int i=0; i<width; i++)
            {
                Color c = img->getPixel(i, i2);
                pixels[k] = c.b;
                pixels[k+1] = c.g;
                pixels[k+2] = c.r;
                k+=3;
            }
            k+=padding;
        }
    }
    
    //more stuff
    SetDIBits(myHDC, bitmapHandle, 0, height, pixels, &bitInfo, DIB_RGB_COLORS);
    RedrawWindow(windowHandle, NULL, NULL, RDW_INVALIDATE);
}

void Window::paintFunction()
{
    BITMAP bm;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandle, &ps);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, bitmapHandle);
    GetObject(bitmapHandle, sizeof(bm), &bm);

    BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    EndPaint(windowHandle, &ps);
}

void Window::createBitmapInfo()
{
    
    bitInfo.bmiHeader.biBitCount = 24;
    bitInfo.bmiHeader.biClrImportant = 0;
    bitInfo.bmiHeader.biClrUsed = 0;
    bitInfo.bmiHeader.biCompression = BI_RGB;
    bitInfo.bmiHeader.biHeight = -height;
    bitInfo.bmiHeader.biWidth = width;
    bitInfo.bmiHeader.biPlanes = 1;
    bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitInfo.bmiHeader.biSizeImage = 0;
    bitInfo.bmiHeader.biXPelsPerMeter = 0;
    bitInfo.bmiHeader.biYPelsPerMeter = 0;

    bitmapHandle = CreateCompatibleBitmap(myHDC, width, height);

    padding = (3*width) % 4;
    switch(padding)
    {
        case 1:
            padding=3;
            break;
        case 2:
            padding=2;
            break;
        case 3:
            padding=1;
            break;
        default:
            break;
    }

    pixels = new unsigned char[width*height*3];
}

int Window::getWidth()
{
    return width;
}

int Window::getHeight()
{
    return height;
}

std::string Window::getTitle()
{
    return title;
}
