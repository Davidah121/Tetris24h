#pragma once
#include <iostream>

struct Color
{
    unsigned char r=0;
    unsigned char g=0;
    unsigned char b=0;
    unsigned char a=0;
};

class Image
{
public:
    Image();
    Image(int width, int height);
    Image(const Image& o);
    ~Image();

    Color operator[](int index);
    Color* getPixels();

    Color getPixel(int x, int y);
    void setPixel(int x, int y, Color c);

    int getWidth();
    int getHeight();

    //draw functions
    void clearImage(Color c);
    void drawRect(int x, int y, int width, int height, bool outline, Color c);
    void drawLine(int x1, int y1, int x2, int y2, Color c);
    void drawLineExt(int x1, int y1, int x2, int y2, Color startC, Color endC);
    void drawImage(int x, int y, Image* o);
    void drawImageExt(int x, int y, double scaleX, double scaleY, double rot, double rx, double ry, Image* o);
    void drawPixel(double x, double y, Color c);
    void drawPixelSimple(int x, int y, Color c);

    //load functions
    static Image* loadBMP(std::string filename);
    void setColorToNoAlpha(Color c);

private:
    Color* pixels;
    int width = 0;
    int height = 0;
};