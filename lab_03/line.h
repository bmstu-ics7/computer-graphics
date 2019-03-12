#ifndef LINE_H
#define LINE_H

#include <QPoint>
#include <QList>
#include <QColor>
#include <cmath>

#include <point.h>

class Line
{
private:
    QPointF start;
    QPointF finish;
    QColor color;
    int alghoritm;
    QList<Point> CDA();
    QList<Point> BFloat();
    QList<Point> BInt();
    QList<Point> BLadder();
    QList<Point> Library();
public:
    Line(double, double, double, double, QColor, int);
    QList<Point> draw();
    bool isLibrary();
};

#endif // LINE_H
