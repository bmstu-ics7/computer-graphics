#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>
#include <QList>
#include <QInputDialog>
#include <QDebug>
#include <QImage>
#include <QApplication>
#include <QPixmap>
#include <cmath>
#include <QGraphicsView>
#include <QStack>
#include <QPair>

#include "figure.h"

const int MARGINRIGHT = 220;
const int UP = 35;
const int LEFT = 17;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void mouseMoveEvent(QMouseEvent*);

private slots:
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnClear_clicked();
    void on_btnFigure_clicked();
    void on_btnColorEdge_clicked();
    void on_btnColorFill_clicked();
    void on_btnColorBackground_clicked();
    void on_btnStart_clicked();
    void on_btnSeed_clicked();

private:
    Ui::MainWindow *ui;
    QColor edgeColor;
    QColor fillColor;
    QColor backgroundColor;
    QList<Figure> getFigures();
    QPoint firstPoint;
    bool horizontal;
    bool vertical;
    void fill(QImage& img, bool delay);
    int canvasWidth;
    int canvasHeight;
    QGraphicsScene scene;
    QPoint seedPixel;
    bool seed;
    QPair<int, int> drawY(QImage& img, QPoint point);
    void findPixel(QStack<QPoint>& stack, QImage img, QPair<int, int> border, int y);
};

#endif // MAINWINDOW_H
