#include "point.h"

void Point::rotate(double angleX, double angleY, double angleZ)
{
    rotateX(angleX);
    rotateY(angleY);
    rotateZ(angleZ);
}

void Point::rotateX(double a)
{
    double ax = (a * M_PI) / 180.0;

    double cy = 0;
    double cz = 0;

    Point p = *this;
    y = cy + (p.y - cy) * cos(ax) + (p.z - cz) * sin(ax);
    z = cz - (p.y - cy) * sin(ax) + (p.z - cz) * cos(ax);
}

void Point::rotateY(double a)
{
    double ay = (a * M_PI) / 180.0;

    double cx = 0;
    double cz = 0;

    Point p = *this;
    x = cx + (p.z - cz) * sin(ay) + (p.x - cx) * cos(ay);
    z = cz + (p.z - cz) * cos(ay) - (p.x - cx) * sin(ay);
}

void Point::rotateZ(double a)
{
    double az = (a * M_PI) / 180.0;

    double cx = 0;
    double cy = 0;

    Point p = *this;
    x = cx + (p.x - cx) * cos(az) + (p.y - cy) * sin(az);
    y = cy - (p.x - cx) * sin(az) + (p.y - cy) * cos(az);
}
