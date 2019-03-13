#include "line.h"
#include <QDebug>

Line::Line(double sx, double sy, double fx, double fy, QColor c, int a)
{
    start = QPointF(sx, sy);
    finish = QPointF(fx, fy);
    color = c;
    alghoritm = a;
}

QList<Point> Line::draw()
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
        return QList<Point>();
    }
}

QList<Point> Line::CDA()
{
    QList<Point> points;

    double dx = finish.x() - start.x();
    double dy = finish.y() - start.y();

    double l = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double sx = dx / l;
    double sy = dy / l;

    double x = start.x();
    double y = start.y();

    for (int i = 1; i <= l + 1; i++) {
        points.append(Point(QPoint(int(x), int(y)), color));
        x += sx;
        y += sy;
    }

    return points;
}

QList<Point> Line::BFloat()
{
    QList<Point> points;

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        points.append(Point(QPoint(int(start.x()), int(start.y())), color));
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
        points.append(Point(QPoint(x, y), color));

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

QList<Point> Line::BInt()
{
    QList<Point> points;

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        points.append(Point(QPoint(int(start.x()), int(start.y())), color));
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
        points.append(Point(QPoint(x, y), color));

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

QList<Point> Line::BLadder()
{
    double I = 256;

    QList<Point> points;

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        points.append(Point(QPoint(int(start.x()), int(start.y())), color));
        return points;
    }

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

    double e = 1.0 / 2;

    m *= I;
    double W = I - m;

    int x = int(start.x());
    int y = int(start.y());

    int xa = swap ? x - sx : x;
    int ya = swap ? y : y - sy;

    QColor copy = color;
    copy.setAlpha(int(m / 2));
    points.append(Point(QPoint(x, y), copy));

    for (int i = 1; i <= dx; i++) {
        if (e < W) {
            if (swap) {
                y += sy;
                ya += sy;
            } else {
                x += sx;
                xa += sx;
            }
            e += m;
        } else {
            x += sx;
            xa += sx;
            y += sy;
            ya += sy;
            e -= W;
        }

        QColor copy = color;
        copy.setAlpha(int(e));
        points.append(Point(QPoint(x, y), copy));

        copy.setAlpha(int(I - e));
        points.append(Point(QPoint(xa, ya), copy));
    }

    return points;
}

QList<Point> Line::Library()
{
    QList<Point> points;
    points.append(Point(QPoint(int(start.x()), int(start.y())), color));
    points.append(Point(QPoint(int(finish.x()), int(finish.y())), color));
    return points;
}

bool Line::isLibrary()
{
    return alghoritm == 4;
}
