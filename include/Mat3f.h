#pragma once
#include "Vec3f.h"

class Mat3f
{
public:
    Mat3f();
    Mat3f(const Mat3f& o);
    void operator=(const Mat3f& o);
    ~Mat3f();

    double* operator[](int row);

    Mat3f operator*(Mat3f m);
    Vec3f operator*(Vec3f v);

    static Mat3f getIdentity();
    static Mat3f getRotationMatrix(double degs);
    static Mat3f getRotationMatrix(double degs, double x, double y);
    static Mat3f getTranslationMatrix(double x, double y);
    static Mat3f getScaleMatrix(double x, double y);

private:
    double** values = nullptr;
    void copyMatrix(Mat3f& o);
    void dispose();
};