#include "figure.h"
#include <QDebug>

Figure::Figure(QList<QPoint> points)
{
    this->points = points;
    extremums = getExtremum();
}

void Figure::drawFirst(QImage& img, QColor color, QColor bcolor, QList<Border>& borders)
{
    QPoint prev = points[0];
    for (QPoint current : points) {
        drawLineFirst(img, QPoint(prev.x(), prev.y()) , QPoint(current.x(), current.y()), color, borders);
        prev = current;
    }

    for (QPoint p : points) {
        bool find = false;
        for (QPoint extr : extremums) {
            if (extr == p) {
                find = true;
                break;
            }
        }

        if (!find) {
            QRgb b = img.pixel(p.x() + 1, p.y());
            QColor c;
            c.setRgb(b);

            if (c == color) {
                img.setPixelColor(p.x() + 1, p.y(), bcolor);
            }

            b = img.pixel(p.x() - 1, p.y());
            c.setRgb(b);

            if (c == color) {
                img.setPixelColor(p.x() - 1, p.y(), bcolor);
            }
        }
    }
}

void Figure::draw(QImage& img, QColor color)
{
    QPoint prev = points[0];
    for (QPoint current : points) {
        drawLine(img, QPoint(prev.x(), prev.y()) , QPoint(current.x(), current.y()), color);
        prev = current;
    }
}

void Figure::drawPoint(QImage& img, QPoint point, QColor color, QList<Border>& borders)
{
    QRgb b = img.pixel(point.x(), point.y());
    QColor c;
    c.setRgb(b);

    if (c == color) {
        img.setPixelColor(point.x() + 1, point.y(), color);
        borders[point.y()].update(point.x() + 1);
    } else {
        img.setPixelColor(point, color);
        borders[point.y()].update(point.x());
    }
}

void Figure::drawLineFirst(QImage& img, QPoint start, QPoint finish, QColor color, QList<Border>& borders)
{
    int dx = int(absx(finish.x() - start.x()));
    int dy = int(absx((finish.y() - start.y())));
    int stepx = signx(finish.x() - start.x());
    int stepy = signx(finish.y() - start.y());

    int flag;
    if (dy > dx)
    {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        flag = 1;
    }
    else
        flag = 0;

    int f1 = 2 * dy - dx;
    int x = start.x();
    int y = start.y();

    drawPoint(img, QPoint(x, y), color, borders);
    for (QPoint p : extremums) {
        if (p.x() == x && p.y() == y) {
            drawPoint(img, p, color, borders);
            break;
        }
    }

    int prevy = y;
    for (int i = 0; i < dx; ++i) {
        if (prevy != y) {
            bool find = false;
            for (QPoint p : extremums) {
                if (p.y() == y && absx(p.x() - x) <= 10) {
                    find = true;
                    break;
                }
            }
            if (!find) {
                drawPoint(img, QPoint(x, y), color, borders);
                borders[y].update(x);
            }
            prevy = y;
        }

        if (f1 >= 0)
        {
            if (flag == 1)
                x += stepx;
            else
                y += stepy;
            f1 -= 2 * dx;
        }
        if (flag == 1)
            y += stepy;
        else
            x += stepx;
        f1 += 2 * dy;

        if (y == finish.y())
            break;
    }
}

void Figure::drawLine(QImage& img, QPoint start, QPoint finish, QColor color)
{
    int dx = int(absx(finish.x() - start.x()));
    int dy = int(absx((finish.y() - start.y())));
    int stepx = signx(finish.x() - start.x());
    int stepy = signx(finish.y() - start.y());

    int flag;
    if (dy > dx)
    {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        flag = 1;
    }
    else
        flag = 0;

    int f1 = 2 * dy - dx;
    int x = start.x();
    int y = start.y();

    for (int i = 0; i < dx; ++i) {
        img.setPixelColor(x, y, color);

        if (f1 >= 0)
        {
            if (flag == 1)
                x += stepx;
            else
                y += stepy;
            f1 -= 2 * dx;
        }
        if (flag == 1)
            y += stepy;
        else
            x += stepx;
        f1 += 2 * dy;
    }
    img.setPixelColor(x, y, color);
}

QList<QPoint> Figure::getPoints()
{
    return points;
}

QList<QPoint> Figure::getExtremum()
{
    QList<QPoint> extremum;
    int count = points.count() - 1;
    for (int i = 0; i < count; ++i) {
        int y0 = points[i].y();
        int yl = points[(count + i - 1) % count].y();
        int yr = points[(i + 1) % count].y();

        if ((y0 < yl && y0 < yr) || (y0 >= yl && y0 >= yr)) {
            extremum.append(points[i]);
        }
    }
    return extremum;
}
