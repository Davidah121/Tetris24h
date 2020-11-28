#pragma once

class Line
{
public:
    Line(double x1, double y1, double x2, double y2)
    {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }

    double getSlope()
    {
        if(x2-x1 != 0)
            return (y2-y1)/(x2-x1);
        else
            return 0;
    }

    double getYInt()
    {
        return -(getSlope()*x1 - y1);
    }

    bool isVertical()
    {
        return (x2-x1 == 0);
    }

    double getMaxX()
    {
        return (x2>x1)?x2:x1;
    }
    double getMaxY()
    {
        return (y2>y1)?y2:y1;
    }
    double getMinX()
    {
        return (x2<x1)?x2:x1;
    }
    double getMinY()
    {
        return (y2<y1)?y2:y1;
    }
    double x1,y1,x2,y2;

};