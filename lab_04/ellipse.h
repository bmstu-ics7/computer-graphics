#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "figure.h"

class Ellipse : public Figure
{
private:
    int a;
    int b;
    void canonicalDraw(QPainter& painter) override;
    void parametricDraw(QPainter& painter) override;
    void bresenhemDraw(QPainter& painter) override;
    void middleDraw(QPainter& painter) override;
    void libraryDraw(QPainter& painter) override;
    void drawPoint(QPainter& painter, int x, int y) override;
public:
    explicit Ellipse();
    Ellipse(QPoint center, int a, int b, Algoritm alg, QColor color);
    ~Ellipse() override {}
    void draw(QPainter& painter) override;
};

#endif // ELLIPSE_H
