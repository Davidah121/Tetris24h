#include "Mat3f.h"
#include <math.h>
#include "System.h"

#define PI 3.1415926
#define degToRad(a) (a*PI/180)
#define radToDeg(a) (a*180/PI)

Mat3f::Mat3f()
{
    values = new double*[3];
    for(int i=0; i<3; i++)
    {
        values[i] = new double[3];
        values[i][0]=0;
        values[i][1]=0;
        values[i][2]=0;
        
    }
}

Mat3f::Mat3f(const Mat3f& o)
{
    copyMatrix((Mat3f&)o);
}

void Mat3f::operator=(const Mat3f& o)
{
    copyMatrix((Mat3f&)o);
}

void Mat3f::copyMatrix(Mat3f& o)
{
    dispose();
    values = new double*[3];
    for(int i=0; i<3; i++)
    {
        values[i] = new double[3];
    }

    for(int i=0; i<3; i++)
    {
        for(int i2=0; i2<3; i2++)
        {
            values[i][i2] = o[i][i2];
        }
    }
}

void Mat3f::dispose()
{
    if(values!=nullptr)
    {
        for(int i=0; i<3; i++)
        {
            delete[] values[i];
        }
        delete[] values;
    }
}
Mat3f::~Mat3f()
{
    dispose();
}

double* Mat3f::operator[](int row)
{
    return values[row];
}

Vec3f Mat3f::operator*(Vec3f v)
{
    Vec3f other = Vec3f();
    other.x = values[0][0] * v.x + values[0][1] * v.y + values[0][2] * v.z;
    other.y = values[1][0] * v.x + values[1][1] * v.y + values[1][2] * v.z;
    other.z = values[2][0] * v.x + values[2][1] * v.y + values[2][2] * v.z;

    return other;
}

Mat3f Mat3f::operator*(Mat3f m)
{
    Mat3f other = Mat3f();

    for(int i=0; i<3; i++)
    {
        for(int i2=0; i2<3; i2++)
        {
            for(int i3=0; i3<3; i3++)
            {
                other[i][i2] += values[i][i3] * m[i3][i2];
            }
        }
    }

    return other;
}

Mat3f Mat3f::getIdentity()
{
    Mat3f m = Mat3f();
    m[0][0] = 1;    m[0][1] = 0;    m[0][2] = 0;
    m[1][0] = 0;    m[1][1] = 1;    m[1][2] = 0;
    m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;

    return m;
}

Mat3f Mat3f::getRotationMatrix(double degs)
{
    Mat3f m = Mat3f();
    m[0][0] = System::roundToDecimal(cos( degToRad(degs) ), 8);
    m[0][1] = System::roundToDecimal(-sin( degToRad(degs) ), 8);
    m[0][2] = 0;

    m[1][0] = System::roundToDecimal(sin( degToRad(degs) ), 8);
    m[1][1] = System::roundToDecimal(cos( degToRad(degs) ), 8);
    m[1][2] = 0;

    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    return m;
}

Mat3f Mat3f::getTranslationMatrix(double x, double y)
{
    Mat3f m = Mat3f();
    m[0][0] = 1;    m[0][1] = 0;    m[0][2] = x;
    m[1][0] = 0;    m[1][1] = 1;    m[1][2] = y;
    m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;

    return m;
}

Mat3f Mat3f::getScaleMatrix(double x, double y)
{
    Mat3f m = Mat3f();
    m[0][0] = x;    m[0][1] = 0;    m[0][2] = 0;
    m[1][0] = 0;    m[1][1] = y;    m[1][2] = 0;
    m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;

    return m;
}