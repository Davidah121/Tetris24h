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

void System::sleep(int milliseconds, int microseconds)
{
    unsigned long timeNeeded = (milliseconds * 1000 * 1000) + (microseconds*1000);
    unsigned long startTime = System::getNanoTime();

    while( (System::getNanoTime() - startTime) < timeNeeded)
    {
        
    }
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