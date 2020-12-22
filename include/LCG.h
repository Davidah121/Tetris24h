#pragma once

class LCG
{
public:
    LCG();
    LCG(unsigned long, unsigned long, unsigned long, unsigned long);
    ~LCG();

    unsigned long get();
private:
    unsigned long seed = 0;
    unsigned long mult = 0;
    unsigned long inc = 0;
    unsigned long mod = 1;
};