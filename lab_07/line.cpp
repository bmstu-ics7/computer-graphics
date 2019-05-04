#include "line.h"

void Line::draw(QPainter& painter)
{
    /*
    double dx = b.x() - a.x();
    double dy = b.y() - a.y();

    double l = absx(dx) > absx(dy) ? absx(dx) : absx(dy);

    double sx = dx / l;
    double sy = dy / l;

    double x = a.x();
    double y = a.y();

    for (int i = 1; i <= l + 1; i++) {
        painter.drawPoint(myRound(x), myRound(y));
        x += sx;
        y += sy;
    }
    */
    painter.drawLine(a, b);
}
