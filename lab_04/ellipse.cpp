#include "ellipse.h"

Ellipse::Ellipse(QPoint center, int a, int b, Algoritm alg, QColor color)
{
    this->center = center;
    this->a = abs(a);
    this->b = abs(b);
    this->alg = alg;
    this->color = color;
}

void Ellipse::draw(QPainter& painter)
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

void Ellipse::drawPoint(QPainter& painter, int x, int y)
{
    painter.drawPoint(x, y);
    painter.drawPoint(x, negative(center.y(), y));
    painter.drawPoint(negative(center.x(), x), y);
    painter.drawPoint(negative(center.x(), x), negative(center.y(), y));
}

void Ellipse::canonicalDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    double c1 = double(b) / a;
    double c2 = double(a) / b;
    int xMax = myRound(a * a / sqrt(a * a + b * b));

    for (int x = 0; x <= xMax; ++x) {
        int y = myRound(c1 * sqrt(a * a - x * x));
        drawPoint(painter, center.x() + x, center.y() + y);
    }

    for (int y = myRound(c1 * sqrt(a * a - xMax * xMax)); y >= 0; --y) {
        int x = myRound(c2 * sqrt(b * b - y * y));
        drawPoint(painter, center.x() + x, center.y() + y);
    }
}

void Ellipse::parametricDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    double dx = 1.0 / a;
    double dy = 1.0 / b;

    double tMax = acos(double(a * a) / (a * a + b * b));

    for (double t = M_PI / 2; t >= tMax; t -= dx) {
        int x = myRound(a * cos(t));
        int y = myRound(b * sin(t));
        drawPoint(painter, center.x() + x, center.y() + y);
    }

    for (double t = tMax; t >= -0.01; t -= dy) {
        int x = myRound(a * cos(t));
        int y = myRound(b * sin(t));
        drawPoint(painter, center.x() + x, center.y() + y);
    }
}

void Ellipse::bresenhemDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    int ad = 2 * a * a;
    int bd = 2 * b * b;
    int x = 0;
    int y = b;

    int err = a * a + b * b - ad * y;
    int err1 = 0;
    int err2 = 0;

    while (y >= 0) {
        drawPoint(painter, center.x() + x, center.y() + y);

        if (err < 0) {
            err1 = 2 * err + ad * y - a * a;

            if (err1 < 0) {
                ++x;
                err += bd * x + b * b;
            } else {
                --y;
                ++x;
                err += bd * x + b * b + a * a - ad * y;
            }
        } else if (err > 0) {
            err2 = 2*err - bd*x - b * b;

            if (err2 <= 0) {
                --y;
                ++x;
                err += bd * x + b * b + a * a - ad * y;
            } else {
                --y;
                err += a * a - ad * y;
            }
        } else {
            --y;
            ++x;
            err += bd * x + b * b + a * a - ad * y;
        }
    }
}

void Ellipse::middleDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));

    int x = 0;
    int y = b;
    int ad = a * a * 2;
    int bd = b * b * 2;

    double xMax = double(a * a) / sqrt(a * a + b * b);
    int teta = -ad * y;
    int dx = 0;

    double fpr = b * b - a * a * y + a * a * 0.25;

    while (x <= xMax) {
        drawPoint(painter, center.x() + x, center.y() + y);

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
        drawPoint(painter, center.x() + x, center.y() + y);

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

void Ellipse::libraryDraw(QPainter& painter)
{
    painter.setPen(QPen(color, 1));
    painter.drawEllipse(center.x() - a, center.y() - b, a * 2, b * 2);
}
