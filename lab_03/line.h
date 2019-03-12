#ifndef LINE_H
#define LINE_H

#include <QPoint>
#include <QList>
#include <QColor>
#include <cmath>

class Line
{
private:
    QPointF start;
    QPointF finish;
    QColor color;
    int alghoritm;
    QList<QPoint> CDA();
    QList<QPoint> BFloat();
    QList<QPoint> BInt();
    QList<QPoint> BLadder();
    QList<QPoint> Library();
public:
    Line(double, double, double, double, QColor, int);
    QList<QPoint> draw();
    QColor getColor();
    bool isLibrary();
};

#endif // LINE_H
