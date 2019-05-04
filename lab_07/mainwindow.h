#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPainter>
#include <QColorDialog>
#include <QList>
#include <QMessageBox>

#include "figure.h"

const int LEFTMARGIN = 220;
const int UPMARGIN = 12;

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

    void on_btnClear_clicked();
    void on_btnAdd_clicked();

    void on_btnAddLine_clicked();

private:
    Ui::MainWindow *ui;

    int canvasWidth;
    int canvasHeight;

    QColor backgroundColor;
    QColor figureColor;
    QColor edgeColor;
    QColor clipColor;

    QList<Line> lines;
    QList<Figure> figures;

    QPoint firstForLine;
    QPoint firstForFigure;

    bool pressShift;
    bool pressCmd;
};

#endif // MAINWINDOW_H
