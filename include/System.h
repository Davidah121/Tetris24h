#pragma once
#include <Windows.h>
#include <iostream>
#include <chrono>

#define SecondInNanos 1000000000

class System
{
public:
    System();
    ~System();

    static HINSTANCE getHInstance();
    static unsigned long getNanoTime();
    
    template<typename T>
    static T clamp(T value, T min, T max)
    {
        if(value<min)
            return min;
        if(value>max)
            return max;
        return value;
    }

    static double roundToDecimal(double value, int decimals);
};