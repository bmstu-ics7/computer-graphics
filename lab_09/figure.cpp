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

int sign(float a)
{
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

int myRound(double x)
{
    int minus = 1;
    if (x < 0) {
        minus = -1;
    }
    return int(x + 0.5 * minus);
}

QVector3D multiVector(QVector3D a, QVector3D b)
{
    float x = a.y() * b.z() - a.z() * b.y();
    float y = a.z() * b.x() - a.x() * b.z();
    float z = a.x() * b.y() - a.y() * b.x();
    return QVector3D(x, y, z);
}

void Figure::clipAll(QPainter& painter, QList<Figure> figures)
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

    for (Figure figure : figures) {
        if (figure.isClose())
            clipOne(painter, figure, move);
    }
}

QList<QPoint> Figure::getPoints()
{
    return points;
}

QPair<double, QPoint> Figure::getNearDot(QPoint point)
{
    double minDist = -1;
    QPoint result;
    for (QPoint p : points) {
        double dist = std::sqrt(std::pow((point.x() - p.x()), 2) +
                      std::pow((point.y() - p.y()), 2));

        if (std::abs(minDist + 1) <= 1e-6 || dist < minDist) {
            minDist = dist;
            result = p;
        }
    }

    return QPair<double, QPoint>(minDist, result);
}

QPoint P(QPoint P1, QPoint P2, double t)
{
    if (std::abs(t) <= 1e-3) {
        t = 0;
    }

    return QPoint(P1.x() + myRound((P2.x() - P1.x()) * t),
                  P1.y() + myRound((P2.y() - P1.y()) * t));
}

QVector3D diffVector(QVector3D a, QVector3D b)
{
    return QVector3D(b.x() - a.x(), b.y() - a.y(), b.z() - a.z());
}

double multiScalar(QVector3D a, QVector3D b)
{
    return double(a.x() * b.x() + a.y() * b.y() + a.z() * b.z());
}

double Figure::find(QPoint a, QPoint b, QPoint P1, QPoint P2, int move)
{
    QVector2D D(P2.x() - P1.x(), P2.y() - P1.y());

    int x = 0, y = 0;
    if (move == -1) {
        x = -1;
        y = 1;
    } else {
        x = 1;
        y = -1;
    }

    QVector2D nIn(x * (a.y() - b.y()),
                  y * (a.x() - b.x()));
    QVector2D W(P1.x() - a.x(), P1.y() - a.y());
    double Dscalar = double(QVector2D::dotProduct(nIn, D));
    double Wscalar = double(QVector2D::dotProduct(W, nIn));

    if (std::fabs(Dscalar) <= 1e-3) {
        return -1;
    }

    double t = - Wscalar / Dscalar;
    return t;
}

bool Figure::visible(QPoint a, QPoint b, QPoint S, QPoint f, int move)
{
    int x = 0, y = 0;
    if (move == -1) {
        x = -1;
        y = 1;
    } else {
        x = 1;
        y = -1;
    }

    QVector2D nIn(x * (a.y() - b.y()),
                  y * (a.x() - b.x()));
    QVector2D D(S.x() - f.x(), S.y() - f.y());

    return QVector2D::dotProduct(nIn, D) > 0;
}

void Figure::clipOne(QPainter& painter, Figure figure, int move)
{
    int x = 0;
    int y = 0;

    if (move == -1) {
        x = -1;
        y = 1;
    } else {
        x = 1;
        y = -1;
    }

    QList<QPoint> result = figure.getPoints();
    result.pop_back();

    for (int i = 0; i < points.count() - 1; ++i) {
        QList<QPoint> clip;
        QPoint F;
        QPoint S;
        for (int j = 0; j < result.count(); ++j) {
            if (j == 0) {
                F = result[j];
            } else {
                double t = find(points[i], points[i + 1], S, result[j], move);
                if ((t >= 0 && t <= 1) || std::abs(t) <= 1e-3) {
                    clip.append(P(S, result[j], t));
                }
            }

            S = result[j];
            if (visible(points[i], points[i + 1], S, points[i], move)) {
                clip.append(S);
            }
        }

        if (clip.count() == 0) {
            return;
        }

        double t = find(points[i], points[i + 1], S, F, move);
        if (t >= 0 && t <= 1) {
            clip.append(P(S, F, t));
        }

        result = clip;
    }

    for (int i = 0; i < result.count(); ++i) {
        painter.drawLine(result[i], result[(i + 1) % result.count()]);
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
