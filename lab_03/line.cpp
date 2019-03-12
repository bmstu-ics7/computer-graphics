#include "line.h"
#include <QDebug>

Line::Line(double sx, double sy, double fx, double fy, QColor c, int a)
{
    start = QPointF(sx, sy);
    finish = QPointF(fx, fy);
    color = c;
    alghoritm = a;
}

QList<QPoint> Line::draw()
{
    switch (alghoritm) {
    case 0:
        return CDA();
    case 1:
        return BFloat();
    case 2:
        return BInt();
    case 3:
        return BLadder();
    case 4:
        return Library();
    default:
        return QList<QPoint>();
    }
}

QList<QPoint> Line::CDA()
{
    QList<QPoint> points;

    double dx = finish.x() - start.x();
    double dy = finish.y() - start.y();

    double l = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double sx = dx / l;
    double sy = dy / l;

    double x = start.x();
    double y = start.y();

    for (int i = 1; i <= l + 1; i++) {
        points.append(QPoint(int(x), int(y)));
        x += sx;
        y += sy;
    }

    return points;
}

QList<QPoint> Line::BFloat()
{
    QList<QPoint> points;

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        points.append(QPoint(int(start.x()), int(start.y())));
        return points;
    }

    int x = int(start.x());
    int y = int(start.y());

    int dx = int(finish.x()) - int(start.x());
    int dy = int(finish.y()) - int(start.y());

    int sx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    int sy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    dx = abs(dx);
    dy = abs(dy);

    double m = 0;

    bool swap = false;
    if (dy > dx) {
        swap = true;
        m = double(dx) / dy;
        int t = dx;
        dx = dy;
        dy = t;
    } else {
        swap = false;
        m = double(dy) / dx;
    }

    double e = m - (1.0 / 2);

    for (int i = 1; i <= dx + 1; i++) {
        points.append(QPoint(x, y));

        if (e > 0) {
            if (swap) {
                x += sx;
            } else {
                y += sy;
            }
            e -= 1;
        }

        if (e < 0) {
            if (swap) {
                y += sy;
            } else {
                x += sx;
            }
        }
        e += m;
    }

    return points;
}

QList<QPoint> Line::BInt()
{
    QList<QPoint> points;

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        points.append(QPoint(int(start.x()), int(start.y())));
        return points;
    }

    int x = int(start.x());
    int y = int(start.y());

    int dx = int(finish.x()) - int(start.x());
    int dy = int(finish.y()) - int(start.y());

    int sx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    int sy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    dx = abs(dx);
    dy = abs(dy);

    bool swap = false;
    if (dy > dx) {
        swap = true;
        int t = dx;
        dx = dy;
        dy = t;
    }

    int e = 2 * dy - dx;

    for (int i = 1; i <= dx + 1; i++) {
        points.append(QPoint(x, y));

        if (e > 0) {
            if (swap) {
                x += sx;
            } else {
                y += sy;
            }
            e -= 2 * dx;
        }

        if (e < 0) {
            if (swap) {
                y += sy;
            } else {
                x += sx;
            }
        }

        e += 2 * dy;
    }

    return points;
}

QList<QPoint> Line::BLadder()
{
    QList<QPoint> points;

    int dx = int(finish.x()) - int(start.x());
    int dy = int(finish.y()) - int(start.y());

    int sx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    int sy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    dx = abs(dx);
    dy = abs(dy);

    return points;
}

QList<QPoint> Line::Library()
{
    QList<QPoint> points;
    points.append(QPoint(int(start.x()), int(start.y())));
    points.append(QPoint(int(finish.x()), int(finish.y())));
    return points;
}

QColor Line::getColor()
{
    return color;
}

bool Line::isLibrary()
{
    return alghoritm == 4;
}
