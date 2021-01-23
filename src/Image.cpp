#include "Image.h"
#include "System.h"
#include "Mat3f.h"
#include "File.h"
#include "Vec2f.h"
#include "Line.h"

Image::Image()
{

}

Image::Image(int width, int height)
{
    this->width = width;
    this->height = height;
    pixels = new Color[width*height];
}

Image::~Image()
{
    if(pixels!=nullptr)
        delete[] pixels;
}

Image::Image(const Image& o)
{
    width = o.width;
    height = o.height;

    if(pixels!=nullptr)
    {
        delete[] pixels;
    }

    pixels = new Color[width*height];
    memcpy(pixels, o.pixels, sizeof(Color)*width*height);
}

Color Image::operator[](int index)
{
    return pixels[index];
}

Color* Image::getPixels()
{
    return pixels;
}

Color Image::getPixel(int x, int y)
{
    return pixels[y*width + x];
}

void Image::setPixel(int x, int y, Color c)
{
    pixels[y*width + x] = c;
}

int Image::getWidth()
{
    return width;
}

int Image::getHeight()
{
    return height;
}

//Draw functions
void Image::clearImage(Color c)
{
    int size = width*height;
    for(int i=0; i<size; i++)
    {
        pixels[i] = c;
    }
}

void Image::drawPixel(double x, double y, Color c)
{
    int x1 = (int)x;
    int y1 = (int)y;
    int x2 = (int)ceil(x);
    int y2 = (int)ceil(y);
    
    double decimalX = x - x1;
    double decimalY = y - y1;

    double c1Alpha = (1.0 - decimalX) * c.a * (1.0 - decimalY);
    double c2Alpha = (decimalX) * c.a * (1.0 - decimalY);
    double c3Alpha = (decimalX) * c.a * (decimalY);
    double c4Alpha = (1.0 - decimalX) * c.a * (decimalY);

    Color preC1 = getPixel(x1, y1);
    Color preC2 = getPixel(x2, y1);
    Color preC3 = getPixel(x2, y2);
    Color preC4 = getPixel(x1, y2);

    double preC1A = preC1.a / 255;
    double preC2A = preC2.a / 255;
    double preC3A = preC3.a / 255;
    double preC4A = preC4.a / 255;
    
    double c1a,c2a,c3a,c4a;
    double c1r,c2r,c3r,c4r;
    double c1g,c2g,c3g,c4g;
    double c1b,c2b,c3b,c4b;
    
    c1a = c1Alpha + preC1A*(1.0-c1Alpha);
    c2a = c2Alpha + preC2A*(1.0-c2Alpha);
    c3a = c3Alpha + preC3A*(1.0-c3Alpha);
    c4a = c4Alpha + preC4A*(1.0-c4Alpha);
    
    if(c1a!=0)
    {
        c1r = (c.r*c1Alpha + preC1.r*preC1A*(1.0-c1Alpha))/c1a;
        c1g = (c.g*c1Alpha + preC1.g*preC1A*(1.0-c1Alpha))/c1a;
        c1b = (c.b*c1Alpha + preC1.b*preC1A*(1.0-c1Alpha))/c1a;
    }
    if(c2a!=0)
    {
        c2r = (c.r*c2Alpha + preC2.r*preC2A*(1.0-c2Alpha))/c2a;
        c2g = (c.g*c2Alpha + preC2.g*preC2A*(1.0-c2Alpha))/c2a;
        c2b = (c.b*c2Alpha + preC2.b*preC2A*(1.0-c2Alpha))/c2a;
    }
    if(c3a!=0)
    {
        c3r = (c.r*c3Alpha + preC3.r*preC3A*(1.0-c3Alpha))/c3a;
        c3g = (c.g*c3Alpha + preC3.g*preC3A*(1.0-c3Alpha))/c3a;
        c3b = (c.b*c3Alpha + preC3.b*preC3A*(1.0-c3Alpha))/c3a;
    }
    if(c4a!=0)
    {
        c4r = (c.r*c4Alpha + preC4.r*preC4A*(1.0-c4Alpha))/c4a;
        c4g = (c.g*c4Alpha + preC4.g*preC4A*(1.0-c4Alpha))/c4a;
        c4b = (c.b*c4Alpha + preC4.b*preC4A*(1.0-c4Alpha))/c4a;
    }

    Color c1,c2,c3,c4;
    c1 = {(unsigned char)System::clamp<double>(c1r,0,255),
            (unsigned char)System::clamp<double>(c1g,0,255),
            (unsigned char)System::clamp<double>(c1b,0,255),
            (unsigned char)System::clamp<double>(c1a*255,0,255),
    };
    c2 = {(unsigned char)System::clamp<double>(c2r,0,255),
            (unsigned char)System::clamp<double>(c2g,0,255),
            (unsigned char)System::clamp<double>(c2b,0,255),
            (unsigned char)System::clamp<double>(c2a*255,0,255),
    };
    c3 = {(unsigned char)System::clamp<double>(c3r,0,255),
            (unsigned char)System::clamp<double>(c3g,0,255),
            (unsigned char)System::clamp<double>(c3b,0,255),
            (unsigned char)System::clamp<double>(c3a*255,0,255),
    };
    c4 = {(unsigned char)System::clamp<double>(c4r,0,255),
            (unsigned char)System::clamp<double>(c4g,0,255),
            (unsigned char)System::clamp<double>(c4b,0,255),
            (unsigned char)System::clamp<double>(c4a*255,0,255),
    };

    setPixel(x1,y1,c1);
    setPixel(x2,y1,c2);
    setPixel(x2,y2,c3);
    setPixel(x1,y2,c4);
}

void Image::drawPixelSimple(int x, int y, Color c)
{
    Color preC1 = getPixel(x, y);

    double preC1A = (double)preC1.a / 255.0;
    double c1Alpha = (double)c.a / 255.0;
    
    double c1a,c1r,c1g,c1b;
    
    c1a = c1Alpha + preC1A*(1.0-c1Alpha);
    
    if(c1a!=0)
    {
        c1r = ((double)c.r*c1Alpha + (double)preC1.r*preC1A*(1.0-c1Alpha))/c1a;
        c1g = ((double)c.g*c1Alpha + (double)preC1.g*preC1A*(1.0-c1Alpha))/c1a;
        c1b = ((double)c.b*c1Alpha + (double)preC1.b*preC1A*(1.0-c1Alpha))/c1a;
    }

    Color c1;
    c1 = {(unsigned char)System::clamp<double>(c1r,0,255),
            (unsigned char)System::clamp<double>(c1g,0,255),
            (unsigned char)System::clamp<double>(c1b,0,255),
            (unsigned char)System::clamp<double>(c1a*255,0,255),
    };

    setPixel(x,y,c1);
}

void Image::drawRect(int x, int y, int width, int height, bool outline, Color c)
{
    int endX = System::clamp<int>(x+width, 0, this->width);
    int endY = System::clamp<int>(y+height, 0, this->height);

    int startX = System::clamp<int>(x, 0, this->width);
    int startY = System::clamp<int>(y, 0, this->height);

    if(outline==false)
    {
        for(int i2=startY; i2<endY; i2++)
        {
            for(int i=startX; i<endX; i++)
            {
                drawPixelSimple(i, i2, c);
            }
        }
    }
    else
    {
        drawLine(startX, startY, endX, startY, c);
        drawLine(startX, endY, endX, endY, c);
        drawLine(startX, startY, startX, endY, c);
        drawLine(endX, startY, endX, endY, c);
    }
}

void Image::drawLine(int x1, int y1, int x2, int y2, Color c)
{
    int endX = System::clamp<int>(x2, 0, this->width);
    int endY = System::clamp<int>(y2, 0, this->height);

    int startX = System::clamp<int>(x1, 0, this->width);
    int startY = System::clamp<int>(y1, 0, this->height);
    
    //y - y1 = mx - mx1
    //y - y1 = (y2-y1)*(x-x1) /(x2-x1)
    //(x2-x1)y - (x2-x1)y1 = (y2-y1)x - (y2-y1)x1
    //(y2-y1)x + (x1-x2)y = (y2-y1)x1 - (x2-x1)y1

    int A = (y1 - y2);
    int B = (x2 - x1);
    int C = -A*x1 - B*y1;

    if( abs(x2-x1) >= abs(y2-y1))
    {
        //go by x
        if(x2-x1<0)
        {
            int temp = endX;
            endX = startX;
            startX = endX;
        }

        int x = startX;
        do
        {
            int y = (A*x + C) / -B;
            if(y>=0 && y<height)
                drawPixelSimple(x,y,c);
            
            x++;
        } while (x<endX);
        
    }
    else
    {
        //go by y
        if(y2-y1<0)
        {
            int temp = endY;
            endY = startY;
            startY = endY;
        }

        int y = startY;
        do
        {
            int x = (B*y + C) / -A;
            if(x>=0 && x<width)
                drawPixelSimple(x,y,c);
            y++;
        } while (y<endY);

    }
}

void Image::drawLineExt(int x1, int y1, int x2, int y2, Color startC, Color endC)
{
    int endX = System::clamp<int>(x2, 0, this->width);
    int endY = System::clamp<int>(y2, 0, this->height);

    int startX = System::clamp<int>(x1, 0, this->width);
    int startY = System::clamp<int>(y1, 0, this->height);
    
    //y - y1 = mx - mx1
    //y - y1 = (y2-y1)*(x-x1) /(x2-x1)
    //(x2-x1)y - (x2-x1)y1 = (y2-y1)x - (y2-y1)x1
    //(y2-y1)x + (x1-x2)y = (y2-y1)x1 - (x2-x1)y1

    int A = (y1 - y2);
    int B = (x2 - x1);
    int C = -A*x1 - B*y1;
    double lerpVal = 1.0;

    if( abs(x2-x1) >= abs(y2-y1))
    {
        //go by x
        if(x2-x1<0)
        {
            int temp = endX;
            endX = startX;
            startX = endX;
        }
        int x = startX;
        do
        {
            int y = (A*x + C) / -B;
            lerpVal = 1.0 - ((x-startX)/(endX-startX));
            Color c;
            c.r = (unsigned char)(startC.r*lerpVal + endC.r*(1-lerpVal));
            c.g = (unsigned char)(startC.g*lerpVal + endC.g*(1-lerpVal));
            c.b = (unsigned char)(startC.b*lerpVal + endC.b*(1-lerpVal));
            c.a = (unsigned char)(startC.a*lerpVal + endC.a*(1-lerpVal));
            
            if(y>=0 && y<height)
                drawPixelSimple(x,y,c);
            
            x++;
        } while (x<endX);
        
    }
    else
    {
        //go by y
        if(y2-y1<0)
        {
            int temp = endY;
            endY = startY;
            startY = endY;
        }

        int y = startY;
        do
        {
            int x = (B*y + C) / -A;
            lerpVal = 1.0 - ((y-startY)/(endY-startY));
            Color c;
            c.r = (unsigned char)(startC.r*lerpVal + endC.r*(1-lerpVal));
            c.g = (unsigned char)(startC.g*lerpVal + endC.g*(1-lerpVal));
            c.b = (unsigned char)(startC.b*lerpVal + endC.b*(1-lerpVal));
            c.a = (unsigned char)(startC.a*lerpVal + endC.a*(1-lerpVal));
            
            if(x>=0 && x<width)
                drawPixelSimple(x,y,c);
            y++;
        } while (y<endY);

    }
}

void Image::drawImage(int x, int y, Image* o)
{
    if(o!=nullptr)
    {
        int startX = System::clamp<int>(x, 0, this->width);
        int startY = System::clamp<int>(y, 0, this->height);

        int endX = System::clamp<int>(startX + o->width, 0, this->width);
        int endY = System::clamp<int>(startY + o->height, 0, this->height);

        int oX = 0;
        int oY = 0;
        for(int i2=startY, oY=0; i2<endY; i2++, oY++)
        {
            for(int i=startX, oX=0; i<endX; i++, oX++)
            {
                Color c = o->getPixel(oX, oY);
                drawPixelSimple(i, i2, c);
            }
        }
    }
}

void Image::drawImageExt(int x, int y, double scaleX, double scaleY, double rot, double rx, double ry, Image* o)
{
    if(o!=nullptr)
    {
        //different approach. apply transform first to corners and map individual pixels to where they would
        //be on the first image with linear transformations
        Vec3f corner1 = Vec3f(0, 0, 1);
        Vec3f corner2 = Vec3f(o->width, 0, 1);
        Vec3f corner3 = Vec3f(o->width, o->height, 1);
        Vec3f corner4 = Vec3f(0, o->height, 1);
        
        Mat3f transformMatrix = Mat3f::getTranslationMatrix(rx, ry) * ( Mat3f::getRotationMatrix(rot) * Mat3f::getTranslationMatrix(-rx, -ry) );

        transformMatrix = Mat3f::getScaleMatrix(scaleX, scaleY) * transformMatrix;

        transformMatrix = Mat3f::getTranslationMatrix(x, y) * transformMatrix;

        corner1 = transformMatrix * corner1;
        corner2 = transformMatrix * corner2;
        corner3 = transformMatrix * corner3;
        corner4 = transformMatrix * corner4;

        Vec2f p1 = Vec2f(corner1.x, corner1.y);
        Vec2f p2 = Vec2f(corner2.x, corner2.y);
        Vec2f p3 = Vec2f(corner3.x, corner3.y);
        Vec2f p4 = Vec2f(corner4.x, corner4.y);

        Vec2f oP1 = Vec2f(0,0);
        Vec2f oP2 = Vec2f(o->width,0);
        Vec2f oP3 = Vec2f(o->width,o->height);

        double detT = (p2.y-p3.y)*(p1.x-p3.x) + (p3.x-p2.x)*(p1.y-p3.y);
        
        int oX = 0;
        int oY = 0;
        int oWidth = o->width;
        int oHeight = o->height;

        int minY = min(min(min(p1.y, p2.y), p3.y), p4.y);
        int maxY = max(max(max(p1.y, p2.y), p3.y), p4.y);

        Line l1 = Line(p1.x, p1.y, p2.x, p2.y);
        Line l2 = Line(p2.x, p2.y, p3.x, p3.y);
        Line l3 = Line(p3.x, p3.y, p4.x, p4.y);
        Line l4 = Line(p4.x, p4.y, p1.x, p1.y);
        
        for(int i2=minY; i2<maxY; i2++)
        {
            int minX, maxX;
            double v = 0;

            if(l1.getSlope() != 0)
            {
                v = (i2 - l1.getYInt()) / l1.getSlope();
                minX = v;
                maxX = v;
            }
            else
            {
                minX = l1.getMinX();
                maxX = l1.getMaxX();
            }

            if(l2.getSlope() != 0)
            {
                v = (i2 - l2.getYInt()) / l2.getSlope();
                minX = min(minX, v);
                maxX = max(maxX, v);
            }
            else
            {
                minX = min(minX,l2.getMinX());
                maxX = max(maxX,l2.getMaxX());
            }

            if(l3.getSlope() != 0)
            {
                v = (i2 - l3.getYInt()) / l3.getSlope();
                minX = min(minX, v);
                maxX = max(maxX, v);
            }
            else
            {
                minX = min(minX,l3.getMinX());
                maxX = max(maxX,l3.getMaxX());
            }

            if(l4.getSlope() != 0)
            {
                v = (i2 - l4.getYInt()) / l4.getSlope();
                minX = min(minX, v);
                maxX = max(maxX, v);
            }
            else
            {
                minX = min(minX,l4.getMinX());
                maxX = max(maxX,l4.getMaxX());
            }

            for(int i=minX; i<=maxX; i++)
            {
                double w1 = ((p2.y-p3.y)*(i-p3.x) + (p3.x-p2.x)*(i2-p3.y)) / detT;
                double w2 = ((p3.y-p1.y)*(i-p3.x) + (p1.x-p3.x)*(i2-p3.y)) / detT;
                double w3 = 1 - w1 - w2;
                
                Vec2f interpPoint = oP1*w1 + oP2*w2 + oP3*w3;
                if(interpPoint.x < 0 || interpPoint.x > o->width)
                    continue;
                if(interpPoint.y < 0 || interpPoint.y > o->height)
                    continue;
                
                Color c = o->getPixel((int)interpPoint.x, (int)interpPoint.y);
                drawPixelSimple(i, i2, c);
            }
            
        }
        
        /*
        for(int i2=0; i2<height; i2++)
        {
            if(oY>=oHeight)
            {
                break;
            }
            
            Vec2f prePoint = Vec2f(-1,-1);
            oX = 0;
            for(int i=0; i<width; i++)
            {
                if(oX>=oWidth)
                {
                    break;
                }

                //double w1 = ((y2-y3)*(oX-x3) + (x3-x2)*(oY-y3)) / detT;
                //double w2 = ((y3-y1)*(oX-x3) + (x1-x3)*(oY-y3)) / detT;
                //double w3 = 1 - w1 - w2;

                Vec2f finalPoint = p1*w1 + p2*w2 + p3*w3;

                Color c = o->getPixel(oX, oY);
                if(c.a != 0)
                {
                    if(finalPoint.x>=0 && finalPoint.x < width)
                    {
                        if(finalPoint.y>=0 && finalPoint.y < height)
                        {
                            //draw line to next point unless point is invalid
                            if(prePoint.x>=0 && prePoint.y>=0)
                            {
                                drawLine((int)prePoint.x, (int)prePoint.y, (int)finalPoint.x, (int)finalPoint.y, c);
                            }
                            else
                            {
                                drawPixelSimple((int)finalPoint.x, (int)finalPoint.y, c);
                            }
                            prePoint = finalPoint;
                        }
                    }
                }
                oX++;
            }
            oY++;

        }
        */
    }
}

Image* Image::loadBMP(std::string filename)
{
    File f = File(filename);
    std::vector<unsigned char> bytes = f.getFileBytes();
    f.close();

    if(bytes.size()<=0)
    {
        return nullptr;
    }

    if(bytes[0] != 'B' || bytes[1] != 'M')
    {
        return nullptr; 
    }

    int indexOfPixels = ((int)bytes[13]<<24) + ((int)bytes[12]<<16) + ((int)bytes[11]<<8) + bytes[10];

    int width = ((int)bytes[21]<<24) + ((int)bytes[20]<<16) + ((int)bytes[19]<<8) + bytes[18];
    int height = ((int)bytes[25]<<24) + ((int)bytes[24]<<16) + ((int)bytes[23]<<8) + bytes[22];

    int bpp = ((int)bytes[29]<<8) + (int)bytes[28];
    int compression = ((int)bytes[33]<<24) + ((int)bytes[32]<<16) + ((int)bytes[31]<<8) + bytes[30];

    if(bpp!=24 || compression!=0)
    {
        return nullptr;
    }

    if(width<=0 || height<=0)
    {
        return nullptr;
    }

    Image* img = new Image(width, height);
    Color* pixels = img->getPixels();

    int padding = (3*width) % 4;

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

    for(int y=height-1; y>=0; y--)
    {
        for(int x=0; x<width; x++)
        {
            Color c;
            c.b = bytes[indexOfPixels];
            c.g = bytes[indexOfPixels+1];
            c.r = bytes[indexOfPixels+2];
            c.a = 255;
            indexOfPixels+=3;

            img->setPixel(x, y, c);
        }
        indexOfPixels+=padding;
    }

    return img;
}

void Image::setColorToNoAlpha(Color c)
{
    Color* startP = pixels;
    Color* endP = pixels + (width*height);

    while(startP<endP)
    {
        if((*startP).r == c.r && (*startP).g == c.g && (*startP).b == c.b)
        {
            (*startP).a = 0;
        }
        startP++;
    }

}