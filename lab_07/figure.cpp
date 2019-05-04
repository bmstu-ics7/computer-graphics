#include "figure.h"

Figure::Figure(int _left, int _right, int _up, int _down) :
    left(_left), right(_right), up(_up), down(_down)
{
    if (left > right) {
        std::swap(left, right);
    }

    if (up > down) {
        std::swap(up, down);
    }
}

void Figure::draw(QPainter& painter)
{
    painter.drawLine(right, up, left, up);
    painter.drawLine(left, up, left, down);
    painter.drawLine(left, down, right, down);
    painter.drawLine(right, down, right, up);
}

void Figure::clipAll(QPainter& painter, QList<Line> lines)
{
    for (Line line : lines) {
        clipOne(painter, line.getA(), line.getB());
    }
}

int Figure::sumCode(QPointF P)
{
    int t1 = P.x() < left ?  1 : 0;
    int t2 = P.x() > right ? 1 : 0;
    int t3 = P.y() > down ?  1 : 0;
    int t4 = P.y() < up ?    1 : 0;
    return t1 + t2 + t3 + t4;
}

int Figure::timesCode(QPointF P1, QPointF P2)
{
    int t11 = P1.x() < left ?  1 : 0;
    int t21 = P1.x() > right ? 1 : 0;
    int t31 = P1.y() > down ?  1 : 0;
    int t41 = P1.y() < up ?    1 : 0;

    int t12 = P2.x() < left ?  1 : 0;
    int t22 = P2.x() > right ? 1 : 0;
    int t32 = P2.y() > down ?  1 : 0;
    int t42 = P2.y() < up ?    1 : 0;

    return t11 * t12 + t21 * t22 + t31 * t32 + t41 * t42;
}

void Figure::clipOne(QPainter& painter, QPointF P1, QPointF P2)
{
    /*
    QPoint P1 = line.getA();
    QPoint P2 = line.getB();
    double eps = 1e-6;

    int i = 2;

l3:

    int S1 = sumCode(P1);
    int S2 = sumCode(P2);

    if (S1 == 0 && S2 == 0) {
        line.draw(painter);
        return;
    }

    if (timesCode(P1, P2) != 0) {
        return;
    }

    QPoint T = P1;

    if (i > 2) {
        if (timesCode(P1, P2) != 0) {
            return;
        } else {
            line.draw(painter);
            return;
        }
    }

    if (S2 == 0) {
        //Res[i] = P1;
        P1 = P2;
        P2 = T;
        ++i;
        goto l3;
    }
l9:
    if (absx(P1.x() - P2.x()) + absx(P1.y() - P2.y()) <= eps) {
        //Res[i] = P1;
        P1 = P2;
        P2 = T;
        ++i;
        goto l3;
    }

    QPoint Pmid = QPoint((P1.x() + P2.x()) >> 1, (P1.y() + P2.y()) >> 1);
    QPoint Pcur = P1;
    P1 = Pmid;

    if (timesCode(P1, P2) != 0) {
        P1 = Pcur;
        P2 = Pmid;
    }

    goto l9;
    */

    double eps = 1;

    if ((P1.x() - P2.x()) * (P1.x() - P2.x()) + (P1.y() - P2.y()) * (P1.y() - P2.y()) <= eps) {
        return;
    }

    if (timesCode(P1, P2) != 0) {
        return;
    }

    int S1 = sumCode(P1);
    int S2 = sumCode(P2);

    if (S1 == 0 && S2 == 0) {
        Line(QPoint(myRound(P1.x()), myRound(P1.y())),
             QPoint(myRound(P2.x()), myRound(P2.y()))).draw(painter);
        return;
    }

    QPointF Pmid((P1.x() + P2.x()) / 2, (P1.y() + P2.y()) / 2);

    clipOne(painter, P1, Pmid);
    clipOne(painter, Pmid, P2);
}
