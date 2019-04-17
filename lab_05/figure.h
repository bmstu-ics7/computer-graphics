#ifndef FIGURE_H
#define FIGURE_H

#include <QList>
#include <QPoint>
#include <QPainter>
#include <QPair>

#include "abs.h"

class Border
{
public:
    Border() : l(100000), r(-1), used(false) {}
    void update(int x)
    {
        used = true;
        if (x > r) r = x;
        if (x < l) l = x;
    }
    bool use() {return used;}
    int left() {return l;}
    int right() {return r;}
private:
    int l;
    int r;
    bool used;
};

class Figure
{
public:
    Figure(QList<QPoint> points);
    void drawFirst(QImage& img, QColor edgeColor, QColor bcolor, QList<Border>& borders);
    void draw(QImage& img, QColor edgeColor);
    QList<QPoint> getPoints();
    QList<QPoint> getExtremum();

private:
    QList<QPoint> points;
    QList<QPoint> extremums;
    QList< QPair<int, int> > lines;
    void drawLine(QImage& img, QPoint start, QPoint finish, QColor edgeColor);
    void drawLineFirst(QImage& img, QPoint start, QPoint finish, QColor color, QList<Border>& borders);
    void drawPoint(QImage& img, QPoint point, QColor color, QList<Border>& borders);
};

#endif // FIGURE_H
