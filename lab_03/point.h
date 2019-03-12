#ifndef POINT_H
#define POINT_H

#include <QColor>
#include <QPoint>

class Point
{
public:
    Point(QPoint, QColor);
    QColor getColor();
    QPoint getPoint();
private:
    QColor color;
    QPoint point;
};

#endif // POINT_H
