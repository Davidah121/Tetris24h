#include "LCG.h"

LCG::LCG()
{

}

LCG::LCG(unsigned long a, unsigned long b, unsigned long c, unsigned long d)
{
    seed = a;
    mult = b;
    inc = c;

    if(d!=0)
        mod = d;
    else
        mod = 1;
}

LCG::LCG(const LCG& o)
{
    seed = o.seed;
    mult = o.mult;
    inc = o.inc;
    mod = o.mod;
}

void LCG::operator=(const LCG& o)
{
    seed = o.seed;
    mult = o.mult;
    inc = o.inc;
    mod = o.mod;
}

LCG::~LCG()
{
    
}

unsigned long LCG::get()
{
    seed = ((seed*mult) + inc) % mod;
    return seed;
}