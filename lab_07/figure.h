#ifndef FIGURE_H
#define FIGURE_H

#include <QList>
#include <QDebug>
#include <math.h>
#include <QApplication>

#include <line.h>

class Figure
{
public:
    Figure() : left(0), right(0), up(0), down(0) {}
    Figure(int left, int right, int up, int down);
    void draw(QPainter& painter);
    void clipAll(QPainter& painter, QList<Line> lines);
    int Left();
    int Right();
    int Up();
    int Down();
private:
    void clipOne(QPainter& painter, QPointF P1, QPointF P2);
    int sumCode(QPointF P);
    int timesCode(QPointF P1, QPointF P2);
    int left;
    int right;
    int up;
    int down;
};

#endif // FIGURE_H
