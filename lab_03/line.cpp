#include "line.h"
#include <QDebug>

Line::Line(double sx, double sy, double fx, double fy, QColor c, int a)
{
    start = QPointF(sx, sy);
    finish = QPointF(fx, fy);
    color = c;
    alghoritm = a;
}

void Line::draw(QPainter& p, int w, int h)
{
    switch (alghoritm) {
    case 0:
        CDA(p, w, h);
        break;
    case 1:
        BFloat(p, w, h);
        break;
    case 2:
        BInt(p, w, h);
        break;
    case 3:
        BLadder(p, w, h);
        break;
    case 4:
        Wu(p, w, h);
        break;
    case 5:
        Library(p, w, h);
        break;
    default:
        break;
    }
}

void Line::CDA(QPainter& p, int w, int h)
{
    double dx = finish.x() - start.x();
    double dy = finish.y() - start.y();

    double l = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double sx = dx / l;
    double sy = dy / l;

    double x = start.x();
    double y = start.y();

    p.setPen(QPen(color, 1));
    for (int i = 1; i <= l + 1; i++) {
        p.drawPoint(w / 2 + int(x + 0.5 * int(x / x)), h / 2 - int(y + 0.5 * int(y / y)));
        x += sx;
        y += sy;
    }
}

void Line::BFloat(QPainter& p, int w, int h)
{
    p.setPen(QPen(color, 1));

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        p.drawPoint(w / 2 + int(start.x()), h / 2 - int(start.y()));
        return;
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
        p.drawPoint(w / 2 + x, h / 2 - y);

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
}

void Line::BInt(QPainter& p, int w, int h)
{
    p.setPen(QPen(color, 1));

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        p.drawPoint(w / 2 + int(start.x()), h / 2 - int(start.y()));
        return;
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
        p.drawPoint(w / 2 + x, h / 2 - y);

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
}

void Line::BLadder(QPainter& p, int w, int h)
{
    double I = 256;

    p.setPen(QPen(color, 1));

    if (int(start.x()) == int(finish.x()) && int(start.y()) == int(finish.y())) {
        p.drawPoint(w / 2 + int(start.x()), h / 2 - int(start.y()));
        return;
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

    double e = I / 2;

    m *= I;
    double W = I - m;

    int x = int(start.x());
    int y = int(start.y());

    int xa = swap ? x - sx : x;
    int ya = swap ? y : y - sy;

    QColor copy = color;
    copy.setAlpha(int(m / 2));
    p.setPen(QPen(copy, 1));
    p.drawPoint(w / 2 + x, h / 2 - y);

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
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + x, h / 2 - y);

        //copy.setAlpha(int(I - e));
        //p.setPen(QPen(copy, 1));
        //p.drawPoint(w / 2 + xa, h / 2 - ya);
    }
}

void Line::Wu(QPainter& p, int w, int h)
{
    auto frac = [](double num) { return num - int(num); };
    QColor copy = color;

    double x1 = start.x();
    double x2 = finish.x();
    double y1 = start.y();
    double y2 = finish.y();

    if (abs(x2 - x1) > abs(y2 - y1)) {
        int one = 1;
        if (y2 < y1) one = -1;

        if (x2 < x1) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        double dx = x2 - x1;
        double dy = y2 - y1;
        double m = dy / dx;

        int xend = int(x1 + 0.5);
        double yend = y1 + m * (xend - x1);
        double xgap = 1 - frac(x1 + 0.5);

        int xpxl1 = xend;
        int ypxl1 = int(yend);

        copy.setAlphaF((1 - frac(abs(yend))) * xgap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl1, h / 2 - ypxl1);

        copy.setAlphaF(frac(abs(yend)) * xgap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl1, h / 2 - ypxl1 - one);

        double intery = yend + m;

        xend = int(x2 + 0.5);
        yend = y2 + m * (xend - x2);

        xgap = frac(x2 + 0.5);

        int xpxl2 = xend;
        int ypxl2 = int(yend);

        copy.setAlphaF((1 - frac(abs(yend))) * xgap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl2, h / 2 - ypxl2);

        copy.setAlphaF(frac(abs(yend)) * xgap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl2, h / 2 - ypxl2 - one);

        for (int x = xpxl1 + 1; x < xpxl2; x++) {
            copy.setAlphaF(1 - frac(abs(intery)));
            p.setPen(QPen(copy, 1));
            p.drawPoint(w / 2 + x, h / 2 - int(intery));

            copy.setAlphaF(frac(abs(intery)));
            p.setPen(QPen(copy, 1));
            p.drawPoint(w / 2 + x, h / 2 - int(intery) - one);

            intery += m;
        }
    } else {
        int one = 1;
        if (x2 < x1) one = -1;

        if (y2 < y1) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        double dx = x2 - x1;
        double dy = y2 - y1;
        double m = dx / dy;

        int yend = int(y1 + 0.5);
        double xend = x1 + m * (yend - y1);
        double ygap = 1 - frac(y1 + 0.5);

        int ypxl1 = yend;
        int xpxl1 = int(xend);

        copy.setAlphaF((1 - frac(abs(xend)) * ygap));
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl1, h / 2 - ypxl1);

        copy.setAlphaF(frac(abs(xend)) * ygap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl1 + one, h / 2 - ypxl1);

        double intery = xend + m;

        yend = int(y2 + 0.5);
        xend = x2 + m * (yend - y2);

        ygap = frac(y2 + 0.5);

        int ypxl2 = yend;
        int xpxl2 = int(xend);

        copy.setAlphaF((1 - frac(abs(xend))) * ygap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl2, h / 2 - ypxl2);

        copy.setAlphaF(frac(abs(xend)) * ygap);
        p.setPen(QPen(copy, 1));
        p.drawPoint(w / 2 + xpxl2 + one, h / 2 - ypxl2);

        for (int y = ypxl1 + 1; y < ypxl2; y++) {
            copy.setAlphaF(1 - frac(abs(intery)));
            p.setPen(QPen(copy, 1));
            p.drawPoint(w / 2 + int(intery), h / 2 - y);

            copy.setAlphaF(frac(abs(intery)));
            p.setPen(QPen(copy, 1));
            p.drawPoint(w / 2 + int(intery) + one, h / 2 - y);

            intery += m;
        }
    }
}

void Line::Library(QPainter& p, int w, int h)
{
    p.setPen(QPen(color, 1));
    p.drawLine(w / 2 + int(start.x()), h / 2 - int(start.y()),
               w / 2 + int(finish.x()), h / 2 - int(finish.y()));
}
