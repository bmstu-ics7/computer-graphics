#ifndef FIGURE_H
#define FIGURE_H

#include <QPoint>
#include <QPainter>

#include <QDebug>

#include "number.h"

enum Algoritm
{
    canonical,
    parametric,
    bresenhem,
    middle,
    library
};

class Figure
{
protected:
    QPoint center;
    Algoritm alg;
    QColor color;
    virtual void canonicalDraw(QPainter&) = 0;
    virtual void parametricDraw(QPainter&) = 0;
    virtual void bresenhemDraw(QPainter&) = 0;
    virtual void middleDraw(QPainter&) = 0;
    virtual void libraryDraw(QPainter&) = 0;
    virtual void drawPoint(QPainter&, int, int) = 0;
public:
    Figure() {}
    virtual ~Figure() = default;
    virtual void draw(QPainter&) = 0;
};

#endif // FIGURE_H
