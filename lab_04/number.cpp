#include "number.h"

int myRound(double num)
{
    return int(num + 0.5 * num / num);
}

int negative(int cx, int x)
{
    return cx - abs(cx - x);
}
