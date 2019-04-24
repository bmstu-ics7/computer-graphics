#ifndef FIGURE_H
#define FIGURE_H

#include <QList>
#include <QPoint>
#include <QPainter>
#include <QPair>
#include <cmath>

#include "abs.h"

class Figure
{
public:
    Figure(QList<QPoint> points);
    Figure(QPoint center, int a, int b);
    void draw(QImage& img, QColor edgeColor);
    QList<QPoint> getPoints();

private:
    QPoint center;
    int a;
    int b;
    QList<QPoint> points;
    void drawLine(QImage& img, QPoint start, QPoint finish, QColor edgeColor);
    void drawEllipce(QImage& img, QColor color);
    void drawPoint(QImage& painter, int x, int y, QColor color);
};

#endif // FIGURE_H
