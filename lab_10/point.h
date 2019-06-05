#ifndef POINT_H
#define POINT_H

#include <cmath>

class Point
{
public:
    Point() : Point(0) { }
    Point(double x) : Point(x, x, x) { }
    Point(double x, double y, double z) : x(x), y(y), z(z) { }

    void rotate(double angleX, double angleY, double angleZ);

    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }

    int getIntX() { return x > 0 ? int(x + 0.5) : int(x - 0.5); }
    int getIntY() { return y > 0 ? int(y + 0.5) : int(y - 0.5); }
    int getIntZ() { return z > 0 ? int(z + 0.5) : int(z - 0.5); }

private:
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    double x;
    double y;
    double z;
};

#endif // POINT_H
