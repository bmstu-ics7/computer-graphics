#include "mymath.h"

int absx(int x)
{
    return x >= 0 ? x : -x;
}

double absx(double x)
{
    return x >= 0 ? x : -x;
}

int myRound(double x)
{
    return int(x + 0.5 * (x / x));
}

int signx(double x)
{
    return x == 0.0 ? 0 : x > 1 ? 1 : -1;
}

int signx(int x)
{
    return x == 0 ? 0 : x > 1 ? 1 : -1;
}

int negative(int cx, int x)
{
    return cx - absx(cx - x);
}
