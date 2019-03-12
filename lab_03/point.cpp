#include "point.h"

Point::Point(QPoint p, QColor c)
{
    point = p;
    color = c;
}

QColor Point::getColor()
{
    return color;
}

QPoint Point::getPoint()
{
    return point;
}
