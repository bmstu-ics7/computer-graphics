#ifndef FIGURE_H
#define FIGURE_H

#include <QList>
#include <cmath>
#include <QVector3D>
#include <QPair>

#include "line.h"

class Figure
{
public:
    Figure(QList<QPoint> points) : points(points) {}
    void draw(QPainter& painter);
    void clipAll(QPainter& painter, QList<Line> lines);
    bool isClose();
    QPair< double, QPair<QPoint, QPoint> > getNear(QPoint point);

private:
    QList<QPoint> points;
    void drawLine(QPainter& painter, QPoint a, QPoint b);
    void clipOne(QPainter& painter, Line line, int move);
};

#endif // FIGURE_H
