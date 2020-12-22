#pragma once
class Vec2f
{
public:
    Vec2f()
    {

    }
    Vec2f(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    ~Vec2f()
    {

    }

    double x = 0;
    double y = 0;

    Vec2f operator*(double a)
    {
        return Vec2f(a*x, a*y);
    }
    Vec2f operator+(Vec2f a)
    {
        return Vec2f(x+a.x, y+a.y);
    }
    Vec2f operator-(Vec2f a)
    {
        return Vec2f(x-a.x, y-a.y);
    }
};