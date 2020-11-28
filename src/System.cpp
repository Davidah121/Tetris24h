#include "System.h"
#include <math.h>

System::System()
{

}

System::~System()
{

}

HINSTANCE System::getHInstance()
{
    return GetModuleHandle(NULL);
}

unsigned long System::getNanoTime()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

double System::roundToDecimal(double value, int decimals)
{
    double finalValue = value;
    for(int i=0; i<decimals; i++)
    {
        finalValue *= 10;
    }
    finalValue = round(finalValue);
    for(int i=0; i<decimals; i++)
    {
        finalValue /= 10;
    }

    return finalValue;
}