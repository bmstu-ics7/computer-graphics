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

#include "figure.h"

const int MARGINRIGHT = 220;
const int UP = 32;
const int LEFT = 15;

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

private slots:
    void on_btnColorEdge_clicked();
    void on_btnColorFill_clicked();
    void on_btnColorBackground_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnClear_clicked();
    void on_btnFigure_clicked();
    void on_btnStart_clicked();

private:
    Ui::MainWindow *ui;
    QColor edgeColor;
    QColor fillColor;
    QColor backgroundColor;
    QList<Figure> getFigures();
    QPoint firstPoint;
    bool horizontal;
    bool vertical;
    void fill(QImage& img, QList<Figure> figures, bool delay);
    int canvasWidth;
    int canvasHeight;
    bool stop;
    QGraphicsScene scene;
    QList<QPoint> extremums;
    QList<Border> borders;
};

#endif // MAINWINDOW_H
