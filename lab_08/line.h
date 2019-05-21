#ifndef LINE_H
#define LINE_H

#include <QPoint>
#include <QPainter>

class Line
{
public:
    Line(QPoint a, QPoint b) : a(a), b(b) {}
    void draw(QPainter& painter);
    QPoint getA();
    QPoint getB();

private:
    QPoint a;
    QPoint b;
};

#endif // LINE_H
