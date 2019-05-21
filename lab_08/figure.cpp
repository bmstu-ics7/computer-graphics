#include "figure.h"

void Figure::draw(QPainter &painter)
{
    QPoint prev = points[0];
    for (QPoint point : points) {
        if (prev != point)
            drawLine(painter, point, prev);
        prev = point;
    }
}

void Figure::drawLine(QPainter &painter, QPoint a, QPoint b)
{
    painter.drawLine(a, b);
}

bool Figure::isClose()
{
    return points[0] == points[points.count() - 1] && points.count() > 1;
}

QVector3D vector(QPoint a, QPoint b)
{
    return QVector3D(b.x() - a.x(), b.y() - a.y(), 0);
}

QVector3D multiVector(QVector3D a, QVector3D b)
{
    float x = a.y() * b.z() - a.z() * b.y();
    float y = a.z() * b.x() - a.x() * b.z();
    float z = a.x() * b.y() - a.y() * b.x();
    return QVector3D(x, y, z);
}

int sign(float a)
{
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

void Figure::clipAll(QPainter& painter, QList<Line> lines)
{
    QVector3D prev = vector(points[0], points[1]);
    QVector3D cur;
    QVector3D result;

    int move = 0;

    for (int i = 0; i < points.count() - 1; ++i) {
        cur = vector(points[i], points[i + 1]);
        result = multiVector(prev, cur);

        if (move == 0) {
            move = sign(result.z());
        }

        if (move != sign(result.z())) {
            return;
        }

        prev = cur;
    }

    for (Line line : lines) {
        clipOne(painter, line, move);
    }
}

int myRound(double x)
{
    return int(x + 0.5 * (x / x));
}

void Figure::clipOne(QPainter &painter, Line line, int move)
{
    QPoint P1 = line.getA();
    QPoint P2 = line.getB();

    double tDown = 0;
    double tUp = 1;

    int x = 0;
    int y = 0;

    if (move == -1) {
        x = -1;
        y = 1;
    } else {
        x = 1;
        y = -1;
    }

    QVector2D D(P2.x() - P1.x(), P2.y() - P1.y());

    for (int i = 0; i < points.count() - 1; ++i) {
        // points[i], points[i + 1] - граница
        QVector2D nIn(x * (points[i].y() - points[i + 1].y()),
                      y * (points[i].x() - points[i + 1].x()));
        QVector2D W(P1.x() - points[i].x(), P1.y() - points[i].y());
        double Dscalar = double(QVector2D::dotProduct(nIn, D));
        double Wscalar = double(QVector2D::dotProduct(W, nIn));

        if (std::fabs(Dscalar) <= 1e-3) {
            if (Wscalar >= 0) {
                continue;
            } else {
                return;
            }
        }

        double t = - Wscalar / Dscalar;

        if (Dscalar > 0) {
            if (t > 1) {
                return;
            } else {
                tDown = std::max(tDown, t);
            }
        } else {
            if (t < 0) {
                return;
            } else {
                tUp = std::min(tUp, t);
            }
        }
    }

    if (tDown <= tUp) {
        QPoint A(myRound(P1.x() + (P2.x() - P1.x()) * tUp),
                 myRound(P1.y() + (P2.y() - P1.y()) * tUp));
        QPoint B(myRound(P1.x() + (P2.x() - P1.x()) * tDown),
                 myRound(P1.y() + (P2.y() - P1.y()) * tDown));
        painter.drawLine(A, B);
    }
}

QPair< double, QPair<QPoint, QPoint> > Figure::getNear(QPoint point)
{
    QPair<QPoint, QPoint> result;
    double min = -1;
    int x0 = point.x();
    int y0 = point.y();

    for (int i = 0; i < points.count() - 1; ++i) {
        int x1 = points[i].x();
        int x2 = points[i + 1].x();
        int y1 = points[i].y();
        int y2 = points[i + 1].y();

        double distance = std::fabs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) /
                std::sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

        if (std::fabs(min + 1) < 1e-3 || distance < min) {
            min = distance;
            result = QPair<QPoint, QPoint>(points[i], points[i + 1]);
        }
    }

    return QPair< double, QPair<QPoint, QPoint> >(min, result);
}
