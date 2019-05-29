#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QInputDialog>

#include "figure.h"

namespace Ui {
class MainWindow;
}

const int MARGINRIGHT = 220;

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
    void on_btnClipFigureColor_clicked();
    void on_btnFigureColor_clicked();
    void on_btnClipColor_clicked();

    void on_btnFinishClip_clicked();
    void on_btnAddClip_clicked();

    void on_btnFinishFigure_clicked();
    void on_btnAddFigure_clicked();

    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;
    QList<Figure> getFigures();
    QList<Figure> getClip();

    QColor backgroundColor;
    QColor clipFigureColor;
    QColor figureColor;
    QColor clipColor;

    QList<Figure> clip;
    QList<Figure> figures;

    QPoint firstForFigure;
    QPoint firstForClip;

    bool pressCmd;
    bool pressShift;
    bool pressAlt;
};

#endif // MAINWINDOW_H
