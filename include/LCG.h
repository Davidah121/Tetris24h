#pragma once

class LCG
{
public:
    LCG();
    LCG(unsigned long a, unsigned long b, unsigned long c, unsigned long d);
    LCG(const LCG& o);
    void operator=(const LCG& o);
    ~LCG();

    unsigned long get();
private:
    unsigned long seed;
    unsigned long mult;
    unsigned long inc;
    unsigned long mod;
};