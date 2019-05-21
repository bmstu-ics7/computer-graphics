#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColorDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

#include "figure.h"

const int MARGINLEFT = 220;

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
    void on_btnBackgroundColor_clicked();
    void on_btnFigureColor_clicked();
    void on_btnEdgeColor_clicked();
    void on_btnClipColor_clicked();
    void on_btnFinish_clicked();

    void on_btnClear_clicked();
    void on_btnAdd_clicked();

    void on_btnAddLine_clicked();

private:
    Ui::MainWindow *ui;

    QList<Figure> getFigures();
    QList<Line> lines;

    QColor backgroundColor;
    QColor figureColor;
    QColor clipColor;
    QColor edgeColor;

    QPoint firstForFigure;
    QPoint firstForLine;

    bool pressCmd;
    bool pressShift;
    bool pressAlt;

    int altState;
    QPair<QPoint, QPoint> altLine;
    QPoint altStart;
    QPoint altFinish;
};

#endif // MAINWINDOW_H
