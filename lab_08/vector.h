#ifndef VECTOR_H
#define VECTOR_H

#include "line.h"

class Vector
{
public:
    Vector(QPoint a, QPoint b) : a(a), b(b) {}
    Vector(Line line) : a(line.getA()), b(line.getB()) {}
    int operator* (const Vector& vector);

private:
    QPoint a;
    QPoint b;
};

#endif // VECTOR_H
