#include "Col.h"

Col::Col()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

Col::Col(int r, int g, int b)
{
    this->r = checkColor(r);
    this->g = checkColor(g);
    this->b = checkColor(b);
}

int Col::checkColor(int val)
{
    if (val < 0)
    {
        return 0;
    }
    else if (val > 255)
    {
        return 255;
    }
    else
        return val;
}