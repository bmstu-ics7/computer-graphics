#include "line.h"

void Line::draw(QPainter &painter)
{
    painter.drawLine(a, b);
}

QPoint Line::getA()
{
    return a;
}

QPoint Line::getB()
{
    return b;
}
