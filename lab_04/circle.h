#ifndef CIRCLE_H
#define CIRCLE_H

#include "figure.h"

class Circle : public Figure
{
private:
    int R;
    void canonicalDraw(QPainter& painter) override;
    void parametricDraw(QPainter& painter) override;
    void bresenhemDraw(QPainter& painter) override;
    void middleDraw(QPainter& painter) override;
    void libraryDraw(QPainter& painter) override;
    void drawPoint(QPainter& painter, int x, int y) override;
public:
    explicit Circle();
    Circle(QPoint center, int R, Algoritm alg, QColor color);
    ~Circle() override {}
    void draw(QPainter& painter) override;
};

#endif // CIRCLE_H
