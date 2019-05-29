#ifndef FIGURE_H
#define FIGURE_H

#include <QPainter>
#include <QList>
#include <cmath>
#include <QVector3D>
#include <QPair>
#include <QDebug>

class Figure
{
public:
    Figure(QList<QPoint> points) : points(points) {}
    void draw(QPainter& painter);
    void clipAll(QPainter& painter, QList<Figure> figures);
    bool isClose();
    QPair<double, QPoint> getNearDot(QPoint point);
    QList<QPoint> getPoints();
    QPair< double, QPair<QPoint, QPoint> > getNear(QPoint point);

private:
    QList<QPoint> points;
    void drawLine(QPainter& painter, QPoint a, QPoint b);
    void clipOne(QPainter& painter, Figure figure, int move);
    double find(QPoint a, QPoint b, QPoint a1, QPoint b1, int move);
    bool visible(QPoint a, QPoint b, QPoint S, QPoint f, int move);
};

#endif // FIGURE_H
