#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColorDialog>
#include <cmath>
#include <QDebug>

#include "point.h"

const int MARGINRIGHT = 240;

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

private slots:
    void on_btnBackgroundColor_clicked();
    void on_btnForegroundColor_clicked();

    void on_sliderAngleX_sliderMoved(int position);
    void on_sliderAngleY_sliderMoved(int position);
    void on_sliderAngleZ_sliderMoved(int position);

    void on_cmbChangeGraph_activated(int index);

    void on_spinXStart_valueChanged(double arg1);
    void on_spinXFinish_valueChanged(double arg1);
    void on_spinXDelta_valueChanged(double arg1);
    void on_spinZStart_valueChanged(double arg1);
    void on_spinZFinish_valueChanged(double arg1);
    void on_spinZDelta_valueChanged(double arg1);

    void on_sliderAngleX_valueChanged(int value);

    void on_sliderAngleY_valueChanged(int value);

    void on_sliderAngleZ_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    void drawLines(QPainter& painter,
                   const int canvasWidth, const int canvasHeight);
    void drawLine(QPainter& painter, const int widht, const int height,
                  Point start, Point end,
                  QList< double >& top, QList< double >& bottom);

    QColor backgroundColor;
    QColor foregroundColor;

    int angleX;
    int angleY;
    int angleZ;

    double (*function) (double, double);

    double xStart;
    double zStart;
    double xEnd;
    double zEnd;
    double xDelta;
    double zDelta;
};

#endif // MAINWINDOW_H
