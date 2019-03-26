#include "circle.h"

Circle::Circle(QPoint center, int R, Algoritm alg, QColor color)
{
    this->center = center;
    this->R = abs(R);
    this->alg = alg;
    this->color = color;
}

void Circle::draw(QPainter& painter)
{
    switch(alg) {
    case canonical:
        canonicalDraw(painter);
        break;
    case parametric:
        parametricDraw(painter);
        break;
    case bresenhem:
        bresenhemDraw(painter);
        break;
    case middle:
        middleDraw(painter);
        break;
    case library:
        libraryDraw(painter);
        break;
    }
}

void Circle::drawPoint(QPainter& painter, int x, int y)
{
    painter.drawPoint(x, y);
    painter.drawPoint(x, negative(center.y(), y));
    painter.drawPoint(negative(center.x(), x), y);
    painter.drawPoint(negative(center.x(), x), negative(center.y(), y));
}

void Circle::canonicalDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    int xMax = myRound(R / sqrt(2));
    for (int x = 0; x <= xMax; ++x) {
        int y = myRound(sqrt(R * R - x * x));
        drawPoint(painter, center.x() + x, center.y() + y);
    }

    for (int y = myRound(sqrt(R * R - xMax * xMax)); y >= 0; --y) {
        int x = myRound(sqrt(R * R - y * y));
        drawPoint(painter, center.x() + x, center.y() + y);
    }
}

void Circle::parametricDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    for (double t = 0; t <= M_PI / 2 * 1.1; t += 1.0 / R) {
        int x = myRound(center.x() + R * cos(t));
        int y = myRound(center.y() + R * sin(t));
        drawPoint(painter, x, y);
    }
}

void Circle::bresenhemDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    int x = 0;
    int y = R;

    int err = 2 * (1 - R);
    int err1 = 0;
    int err2 = 0;

    while (y >= 0) {
        drawPoint(painter, center.x() + x, center.y() + y);

        if (err < 0) {
            err1 = 2 * err + 2 * y - 1;

            if (err1 <= 0) {
                ++x;
                err += 2 * x + 1;
            } else {
                ++x;
                --y;
                err += 2 * (x - y + 1);
            }
        } else if (err > 0) {
            err2 = 2 * err - 2 * x - 1;

            if (err2 <= 0) {
                ++x;
                --y;
                err += 2 * (x - y + 1);
            } else {
                --y;
                err += -2 * y + 1;
            }
        } else {
            ++x;
            --y;
            err += 2 * (x - y + 1);
        }
    }
}

void Circle::middleDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    int p = int(1 - R);
    int x = 0;
    int y = R;

    drawPoint(painter, center.x() + x, center.y() + y);
    drawPoint(painter, center.x() + y, center.y() + x);

    while (x < y) {
        x += 1;

        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y -= 1;
            p += 2 * (x - y) + 1;
        }

        drawPoint(painter, center.x() + x, center.y() + y);
        drawPoint(painter, center.x() + y, center.y() + x);
    }
}

void Circle::libraryDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));
    painter.drawEllipse(center.x() - R, center.y() - R, R * 2, R * 2);
}
