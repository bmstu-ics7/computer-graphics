#include "figure.h"
#include <QDebug>

Figure::Figure(QList<QPoint> points) : points(points) {}

Figure::Figure(QPoint center, int a, int b) : center(center), a(a), b(b) {}

void Figure::draw(QImage& img, QColor color)
{
    if (points.count() > 0) {
        QPoint prev = points[0];
        for (QPoint current : points) {
            drawLine(img, QPoint(prev.x(), prev.y()) , QPoint(current.x(), current.y()), color);
            prev = current;
        }
    } else {
        drawEllipce(img, color);
    }
}

void Figure::drawLine(QImage& img, QPoint start, QPoint finish, QColor color)
{
    double dx = finish.x() - start.x();
    double dy = finish.y() - start.y();

    double l = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double sx = dx / l;
    double sy = dy / l;

    double x = start.x();
    double y = start.y();

    for (int i = 1; i <= l + 1; i++) {
        img.setPixelColor(myRound(x), myRound(y), color);
        x += sx;
        y += sy;
    }
}

void Figure::drawPoint(QImage& img, int x, int y, QColor color)
{
    img.setPixelColor(x, y, color);
    img.setPixelColor(x, negative(center.y(), y), color);
    img.setPixelColor(negative(center.x(), x), y, color);
    img.setPixelColor(negative(center.x(), x), negative(center.y(), y), color);
}

void Figure::drawEllipce(QImage& img, QColor color)
{
    int x = 0;
    int y = b;
    int ad = a * a * 2;
    int bd = b * b * 2;

    double xMax = double(a * a) / sqrt(a * a + b * b);
    int teta = -ad * y;
    int dx = 0;

    double fpr = b * b - a * a * y + a * a * 0.25;

    while (x <= xMax) {
        drawPoint(img, center.x() + x, center.y() + y, color);

        if (fpr > 0) {
            --y;
            teta += ad;
            fpr += teta;
        }

        dx += bd;
        ++x;
        fpr += dx + b * b;
    }

    fpr += 0.75 * (a * a - b * b) - (b * b * x + a * a * y);

    teta = bd * x;
    int dy = -ad*y;

    while (y >= 0) {
        drawPoint(img, center.x() + x, center.y() + y, color);

        if (fpr < 0) {
            ++x;
            teta += bd;
            fpr += teta;
        }

        dy += ad;
        fpr += dy + a * a;
        --y;
    }
}

QList<QPoint> Figure::getPoints()
{
    return points;
}
