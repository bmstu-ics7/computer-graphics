#ifndef LINE_H
#define LINE_H

#include <QPoint>
#include <QPainter>

#include "mymath.h"

class Line
{
public:
    Line() : a(QPoint(0, 0)), b(QPoint(0, 0)) {}
    Line(QPoint a, QPoint b) : a(a), b(b) {}
    Line(QPointF a, QPointF b);
    QPoint getA() {return a;}
    QPoint getB() {return b;}
    void draw(QPainter& painter);

private:
    QPoint a;
    QPoint b;
};

#endif // LINE_H
